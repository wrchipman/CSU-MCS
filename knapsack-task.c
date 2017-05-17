///////////////////////////////////////////////////////////////////////////////
//
// Computes a solution vector for the 0/1 knapsack problem.
//
// Uses the memory-efficient divide-and-conquer algorithm.
//
// Parallelized using MPI.  A master process runs the divide and conquer
// algorithm.  All other processes are slaves and compute a column
// of the table, gathering the results back to the master.
//
// Each slave depends on the previous rows from the slave before it so
// wavefront parallelization is used.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include "timer.h"
#include "simple_profile.h"

#define    MAX(x,y)   ((x)>(y) ? (x) : (y))
#define    MIN(x,y)   ((x)<(y) ? (x) : (y))
#define    int64        long long

FILE *fp;
char *problem_filename;
int *weights_and_profits; // make weights and profits a contiguous array to simplify the broadcast of the problem description.
int *weights; // pointer into weights_and_profits
int *profits; // pointer into weights_and_profits
int N; // number of items
int C; // capacity program is solving for
int *solution;

typedef struct _Task {
	int begin;
	int end;
	int capacity;
	struct _Task *next;
} Task;

Task *current_task_queue_head;
Task *current_task_queue_iterator;
Task *next_task_queue_head;


// Only malloc these buffers once.  They are never used at different
// levels in the recursion at the same time.
// The 'a' and 'b' variants of cur and prev are for the two instances of find_last_row
// in each task.
typedef struct {
	int *row_buffers;
	int *cur_recycled_a;
	int *prev_recycled_a;
	int *cur_recycled_b;
	int *prev_recycled_b;
} ThreadData;


int *find_last_row(int begin, int end, int capacity, int *cur_recycled, int *prev_recycled);
void solve_task(Task *task, ThreadData *thread_data);
void task_loop();
int one_task(ThreadData *thread_data);

void begin_next_task_queue();
void delete_task_queue(Task *task_queue_head);

void init(int argc, char **argv);
void init_task_queue();
void alloc_weights_and_profits();
void read_problem_size(int argc, char **argv);
void read_weights_and_profits(FILE *fp);
ThreadData *new_thread_data();
void delete_thread_data(ThreadData *thread_data);
void delete_problem();
void problem_file_readline(FILE *fp, int *a, int *b);
void new_solve_task_unprotected(Task *task);
void my_exit(int code);

/*
 * Calculates an array containing the optimal profit for each capacity c
 * between 0 and capacity (inclusive) using only items from begin
 * to end (inclusive).
 *
 * Callers provides the two row buffers used for cur and prev.  Returns a pointer
 * to whichever of these contains the last row after the function completes.
 */
int *find_last_row(int begin, int end, int capacity, int *cur_recycled, int *prev_recycled) {
	int i, j;
	int j_end;
	int *cur = cur_recycled;
	int *prev = prev_recycled;
	int *tmp;

	// first row is simple: all 0's until the first object fits in the knapsack,
	// then the profit of the first object for all larger capacities.
	j_end = MIN(weights[begin], capacity+1);
	for (j = 0; j < j_end; j++) {
		cur[j] = 0;
	}
	j_end = capacity+1;
	for (; j < j_end; j++) {
		cur[j] = profits[begin];
	}

	// all remaining rows
	for (i = begin + 1; i <= end; i++) {

		tmp = prev;
		prev = cur;
		cur = tmp; // Swap arrays

		// Any capacity less than the weight of the object will never contain the object
		j_end = MIN(weights[i], capacity+1);
		for (j = 0; j < j_end; j++) {
			cur[j] = prev[j];
		}
		// Capacities larger than the weight of the object MAY contain the object
		j_end = capacity+1;
		for (j = weights[i]; j < j_end; j++) {
			cur[j] = MAX(prev[j],profits[i]+prev[j-weights[i]]);
		}
	}

	return cur;
}

/**
 * Adds a new task to next_task_queue.
 */
void new_solve_task(int begin, int end, int capacity) {
	Task *task = (Task*)malloc(sizeof(Task));
	task->begin = begin;
	task->end = end;
	task->capacity = capacity;
	task->next = NULL;

#pragma omp critical
	{
		if (next_task_queue_head == NULL) {
			next_task_queue_head = task;
		} else {
			task->next = next_task_queue_head;
			next_task_queue_head = task;
		}
	}
}

/**
 * Returns the next task in the iteration through current_task_queue.
 * Returns NULL if there are no more tasks in the iteration.
 */
Task *get_next_task() {
	Task *task;

#pragma omp critical
	{
		task = current_task_queue_iterator;
		if (current_task_queue_iterator != NULL) {
			current_task_queue_iterator = current_task_queue_iterator->next;
		}
	}

	return task;
}

void delete_task_queue(Task *task_queue_head) {
	Task *t = task_queue_head;
	Task *next;

	while(t!=NULL) {
		next = t->next;
		free(t);
		t = next;
	}
}

/**
 * Recursively calculates the solution vector for the items to pick from begin to end (inclusive) that
 * fit within capacity.
 */
void solve_task(Task *task, ThreadData *thread_data) {
	int begin = task->begin;
	int end = task->end;
	int capacity = task->capacity;
	int i;
	int mid;
	int *profits_a;
	int *profits_b;
	int profit;
	int optimal_profit;
	int optimal_capacity_a;


#ifdef DEBUG
	printf("[%d] solve(%d, %d, %d)\n", omp_get_thread_num(), begin, end, capacity);
#endif

	// Base case: remaining problem is trivial.
	if (begin == end) {
		if (weights[begin] > capacity) {
			solution[begin] = 0;
		} else {
			solution[begin] = 1;
		}
	} else {

		mid = begin + (end - begin) / 2;

		// first, split the items into two sets and figure out all possible profits for each set.
		profits_a = find_last_row(begin, mid, capacity, thread_data->cur_recycled_a, thread_data->prev_recycled_a);
		profits_b = find_last_row(mid + 1, end, capacity, thread_data->cur_recycled_b, thread_data->prev_recycled_b);

		// second, add all the possible combinations together and keep the highest.
		optimal_capacity_a = 0;
		optimal_profit = profits_a[0] + profits_b[capacity];
		for (i = 1; i <= capacity; i++) {
			profit = profits_a[i] + profits_b[capacity - i];
			if (profit > optimal_profit) {
				optimal_capacity_a = i;
				optimal_profit = profit;
			}
		}

//#ifdef DEBUG
//		printf("[ master ] solve(%d, %d, %d): optimal capacity for a is %d, profit %d\n",
//				begin, end, capacity, optimal_capacity_a, optimal_profit);
//#endif

		// third, now that we know how much capacity to allocate to each sub problem, solve for it.
		new_solve_task(begin, mid, optimal_capacity_a);
		new_solve_task(mid + 1, end, capacity - optimal_capacity_a);
	}
}



int main(int argc, char **argv) {
	int optimal_profit;
	int i;
	double time;
	Timer timer;

	init(argc, argv);

	initialize_timer(&timer, "main timer");
	start_timer(&timer);

	new_solve_task(0, N - 1, C);

	task_loop();

	stop_timer(&timer);
	time = elapsed_time(&timer);

	optimal_profit = 0;
	for (i = 0; i < N; i++) {
		if (solution[i])
			optimal_profit += profits[i];
	}

	printf("The optimal profit is %d Time taken : %lf.\n", optimal_profit, time);
	// End of "Solve for the optimal profit"

	// Backtracking (only in debug mode)
#ifdef DEBUG

	printf("Solution vector is: \n --> ");
	for (i=0; i<N; i++ ) {
		printf("%d ", solution[i]);
	}
	printf("\n");
#endif

	delete_problem();
	my_exit(0);
	return 0;
}

void task_loop_thread();

void task_loop() {
	int num_tasks;
	Task *t;
	Profile p;
	char stripped_problem_filename[256];
	char profile_seq_filename[256];
	char profile_filename[256];

	simple_profile_init(&p);

	// all threads must agree on whether they are done
	// before master can make the next job queue the current one
	// and all threads must wait for master to make it so before continueing work.
	while(next_task_queue_head) {
		num_tasks = 0;
		for(t=next_task_queue_head; t!=NULL; t=t->next) { num_tasks += 1; }
		simple_profile_record(&p, num_tasks);

		begin_next_task_queue();

#pragma omp parallel
		task_loop_thread();
	}

	if (strlen(problem_filename) < 256) {
		filename_strip(problem_filename, stripped_problem_filename);
		snprintf(profile_seq_filename, 256, "profile.%s.p1.out", stripped_problem_filename);
		snprintf(profile_filename, 256, "profile.%s.p%d.out",
				stripped_problem_filename, omp_get_max_threads());
		simple_profile_record(&p, -1);
		simple_profile_write_speedup(&p, profile_seq_filename, profile_filename);
	}

	simple_profile_delete(&p);
}

void task_loop_thread() {
	ThreadData *thread_data = new_thread_data();
	while(one_task(thread_data)) {
		// intentionally empty loop
	}
	delete_thread_data(thread_data);
}

void begin_next_task_queue() {
	delete_task_queue(current_task_queue_head);
	current_task_queue_head = next_task_queue_head;
	next_task_queue_head = NULL;

	current_task_queue_iterator = current_task_queue_head;
}

int one_task(ThreadData *thread_data) {
	Task *task;

	task = get_next_task();
	if (task != NULL) {
		solve_task(task, thread_data);
		return 1;
	} else {
		return 0;
	}
}









/*
 * Initialization functions
 */

void init(int argc, char **argv) {
	read_problem_size(argc, argv);
	alloc_weights_and_profits();
	read_weights_and_profits(fp);
	init_task_queue();
}

void alloc_weights_and_profits() {
	int size = N * sizeof(int);

	weights_and_profits = (int*) malloc(size*3);
	weights  = &weights_and_profits[0];
	profits  = &weights_and_profits[N];
	solution = &weights_and_profits[N*2];

	if (weights_and_profits == NULL) {
		printf("[ERROR] : Failed to allocate memory for weights/profits.\n");
		my_exit(1);
	}
}

void read_problem_size(int argc, char **argv) {
	if (argc < 1) {
		printf("USAGE : %s [filename].\n", argv[0]);
		my_exit(1);
	}

	problem_filename = argv[1];
	fp = fopen(problem_filename, "r");
	if (fp == NULL) {
		printf("[ERROR] : Failed to read file named '%s'.\n", problem_filename);
		my_exit(1);
	}

	problem_file_readline(fp, &N, &C);
	printf("The number of objects is %d, and the capacity is %d.\n", N, C);
}

void read_weights_and_profits(FILE *fp) {
	int i;

	for (i = 0; i < N; i++) {
		problem_file_readline(fp, &(weights[i]), &(profits[i]));
	}

	fclose(fp);
}

/*
 * Allocates buffers that are private to each thread.
 */
ThreadData *new_thread_data() {
	ThreadData *thread_data;
	int row_size = C+1;

	thread_data = (ThreadData *)malloc(sizeof(ThreadData));
	if (thread_data == NULL) {
		printf("[ERROR] : Failed to allocate thread private data\n");
		exit(1);
	}

	thread_data->row_buffers = (int*)malloc(4 * row_size * sizeof(int));
	thread_data->cur_recycled_a  = &thread_data->row_buffers[0*row_size];
	thread_data->prev_recycled_a = &thread_data->row_buffers[1*row_size];
	thread_data->cur_recycled_b  = &thread_data->row_buffers[2*row_size];
	thread_data->prev_recycled_b = &thread_data->row_buffers[3*row_size];

	if (thread_data->row_buffers == NULL) {
		printf("[ERROR] : Failed to allocate memory for recursive solve\n");
		exit(1);
	}

	return thread_data;
}

void delete_problem() {
	free(weights_and_profits);
}

void delete_thread_data(ThreadData *thread_data) {
	free(thread_data->row_buffers);
	free(thread_data);
}

void problem_file_readline(FILE *fp, int *a, int *b) {
	int count;

	count = fscanf(fp, "%d %d", a, b);
	if (count != 2) {
		printf("[ERROR] : Input file is not well formatted.\n");
		my_exit(1);
	}
}

void init_task_queue() {
	current_task_queue_head = NULL;
	current_task_queue_iterator = NULL;
	next_task_queue_head = NULL;
}

void my_exit(int code) {
	exit(code);
}


void DEBUG_print_task_queues_unprotected() {
	Task *task;
	printf("\tCurrent: ");
	for(task=current_task_queue_iterator; task!=NULL; task = task->next) {
		printf("%p:(%d, %d, %d), ", task, task->begin, task->end, task->capacity);
	}
	printf("\n");
	printf("\tNext:    ");
	for(task=next_task_queue_head; task!=NULL; task = task->next) {
		printf("%p:(%d, %d, %d), ", task, task->begin, task->end, task->capacity);
	}
	printf("\n");
}

