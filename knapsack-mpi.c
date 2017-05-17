////////////////////////////////////////////////////////////////////////////////
//
// Computes a solution vector for the 0/1 knapsack problem.
//
// Uses the memory-efficient divide-and-conquer algorithm.
//
// Parallelized using MPI.  Each process computes a stripe of rows
// for find_last_row.  The last process (which calculates the last row)
// is the master process and runs the divide-and-conquer algorithm.
// Uses tiling to reduce communications. Uses wavefront parallelization.
//
// The parallelized version will provide little optimization when number of items < number of processors.
// The sequential version is used when the subproblems no longer satisfy this.
// Performance wise this is okay because for large problems the time spent
// on the larger subproblems dominates the time spent on smaller subproblems.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include <stdarg.h>
#include "timer.h"
#include "simple_profile.h"
#include "ringbuffer.h"

int MAX(x, y) { return (x>y)?x:y; }
int MIN(x, y) { return (x<y)?x:y; }

int *weights_and_profits; // make weights and profits a contiguous array to simplify the broadcast of the problem description.
int *weights; // pointer into weights_and_profits
int *profits; // pointer into weights_and_profits
int N; // number of items
int C; // capacity program is solving for
int max_weight; // highest value in weights array
int *solution;

FILE *fp;
FILE *debug_out;
char *problem_filename;
int CAPACITY_BLOCK_SIZE = 8000; // Optimal for k1000 against 16 nodes on Carver

// Only malloc these buffers once.  They are never used at different
// levels in the recursion at the same time.
int *row_buffers;
int *cur_recycled;
int *prev_recycled;
int *last_row_a_recycled;
int *last_row_b_recycled;
int *send_recv_buf_recycled;

int processor_id;   // MPI variables
int num_processors; //

/** Last processor is master because it is the one that calculates the last row. */
#define master (num_processors-1)
#define is_master (processor_id == master)

/**
 * Indexes of control messages.
 */
#define MSG_ACTION 0
#define MSG_BEGIN 1
#define MSG_END 2
#define MSG_CAPACITY 3
#define MSG_LEN 4

/**
 * Values for msg_buffer[MSG_ACTION].
 */
#define ACTION_FIND_LAST_ROW 1
#define ACTION_FINALIZE 2

#define TAG_PIPELINE 3

#define SWAP(a, b) {tmp = a; a = b; b = tmp;}
#define N_BLOCK_SIZE(begin, end) ((int)(ceil(((double)end-begin+1.0)/num_processors )))


void find_last_row(int begin, int end, int capacity, int *result);
void finalize();

void find_last_row_partial(int begin, int end, int capacity, int *master_result);
void find_last_row_sequential(int begin, int end, int capacity, int *master_result);
void solve(int begin, int end, int capacity, ProfileListItem *profile_item);
void master_main();

void init(int argc, char **argv);
void init_mpi(int *argc, char ***argv);
void init_debug();
void init_block_size(int argc, char **argv);
void alloc_weights_and_profits();
void read_problem_size(int argc, char **argv);
void read_weights_and_profits(FILE *fp);
void alloc_row_buffers();
void delete_problem();
void problem_file_readline(FILE *fp, int *a, int *b);
void my_exit(int code);

void slave_loop();

void debug_printf(char *fmt, ...);
void debug_print_row(char *label, int *row, int length);


void find_last_row(int begin, int end, int capacity, int *result) {
	int msg[MSG_LEN];

	if (end-begin+1 < num_processors) {
		debug_printf("[ master ] find_last_row(%d, %d, %d) (sequential) \n", begin, end, capacity);
		find_last_row_sequential(begin, end, capacity, result);

	} else {
		debug_printf("[ master ] find_last_row(%d, %d, %d) (parallelized) \n", begin, end, capacity);

		// Corresponding MPI_Bcast for the slaves is in slave_loop().
		msg[MSG_ACTION] = ACTION_FIND_LAST_ROW;
		msg[MSG_BEGIN] = begin;
		msg[MSG_END] = end;
		msg[MSG_CAPACITY] = capacity;
		MPI_Bcast(msg, MSG_LEN, MPI_INT, master, MPI_COMM_WORLD);

		find_last_row_partial(begin, end, capacity, result);
	}
}

void finalize() {
	int msg[MSG_LEN];
#ifdef DEBUG
	printf("[ master ] bringing down slaves\n");
#endif

	// Corresponding MPI_Bcast for the slaves is in slave_loop().
	msg[MSG_ACTION] = ACTION_FINALIZE;
	MPI_Bcast(msg, MSG_LEN, MPI_INT, master, MPI_COMM_WORLD);
}


/*
 * Calculates an array containing the optimal profit for each capacity c
 * between 0 and capacity (inclusive) using only items from begin
 * to end (inclusive).
 *
 * Each processor is responsible for a stripe of rows.  This uses
 * wavefront parallelization.  It uses blocking/tiling to reduce number of
 * communications between processors.  The last processor is the master
 * processor.  After it calculates the last row it returns it to the
 * divide-and-conquer algorithm.
 *
 * Each row is implemented as a ring buffer.  This makes it so the saved
 * values in each row do not have to be moved to different memory locations.
 * Instead, the ring wraps access around indefinetely.  Each ring can hold
 * at least capacity_block_size + max_weight items, so the entire tile
 * can be calculated without overwriting values that it depends on from
 * the previous tile.
 */
void find_last_row_partial(int begin, int end, int capacity, int *master_result) {
	int mask;
	int t_edge;
	MPI_Status status;
	int *send_recv_buf = send_recv_buf_recycled;

	// Each processor is responsible for a range of items in the table.
	// Each processor fills in the entire range of capacities for those items.
	//
	//        0 -------------------j----------------------> capacity+1
	//
	// begin  +--------------------------------------------------+
	//     |  |                                                  |
	//     |  |                    0----t--->capacity_block_size |
	//     |  |               +----+........                     |
	//     |  +---------------+    +--------+--------------------+
	//     |  |             0 |    |        |                    |
	//     |  |             | |    |        |                    |
	//   i |  |           s | |    |        |                    |
	//     |  |             | |    |        |                    |
	//     |  |            \/ |    |        |                    |
	//     |  |  n_block_size +----+        |                    |
	//     |  +---------------+ - -+,,,,,,,,+--------------------+
	//     |  |  dependencuies__/      \__tile, filled using     |
	//     |  |  from previous           cur/prev row algorithm  |
	//    \/  |  tile                                            |
	//   end  +--------------------------------------------------+
	//
	// ..... is line received from previous processor in wavefront pipeline
	// ,,,,, is line sent to next processor
	// saved_prev is the lines "sent" from a processor to itself for wavefront parallelization
	//
	// All dependencies within the block are satisfied by the block, saved_prev, and the previous
	// line received from the previous processor in the wavefront pipeline.
	int n_block_begin;
	int n_block_size;
	int capacity_block_size;
	int capacity_block_begin;
	int i, j;
	int s, t;
	RingBuffer buffer;
	Ring prev, cur;

	n_block_begin = begin + processor_id*N_BLOCK_SIZE(begin, end);
	n_block_size = MIN( N_BLOCK_SIZE(begin, end), end - n_block_begin + 1);
	// For processors != 0, the previous row (-1) is received from the previous processor.
	// But for processor 0, the first row (0) is "received" by being filled in programmatically.
	if (processor_id == 0) {
		n_block_begin += 1;
		n_block_size -= 1;
	}

	// there are two conditions where n_block_size < 1.  1) the first processor is
	// sending only the generated line. 2) the problem size does not allow the last
	// processor to do any work given a fixed capacity_block_size for all processors
	// (example, end-begin=125, num_processors=15)
	// in either case, make sure at least one ring is allocated for recieving/sending
	// from/to the previous/next processor.

	new_ring_buffer(&buffer,
			CAPACITY_BLOCK_SIZE+max_weight,
			MAX(0, n_block_size)+1);
	if (buffer.begin == NULL) {
		printf("[ERROR] Failed to allocate tile memory (%d bytes)\n", buffer.bounding_buffer_len);fflush(stdout);
		exit(1);
	}
	mask = buffer.mask; // for RING macro


	//debug_printf("find_last_row(%d, %d, %d)\n", begin, end, capacity);
	//debug_printf("I am responsible for items %d to %d inclusive\n", n_block_begin, n_block_begin + n_block_size - 1);


	j=0;
	while(j<capacity+1) {
		capacity_block_begin = j;
		capacity_block_size = MIN( CAPACITY_BLOCK_SIZE, capacity-j+1 );

		prev = buffer.begin - buffer.ring_len; // not valid pointer until first increment.
		cur = buffer.begin;

		if (processor_id == 0) {
			i = begin;
			j = capacity_block_begin;
			t_edge = MIN(capacity_block_size, weights[i]-capacity_block_begin);
			for(t=0; t<t_edge;t++, j++) {
					RING(cur, j) = 0;
			}	
			t_edge = MAX(0, weights[i]-capacity_block_begin);
			for(t=t_edge;t<capacity_block_size;t++, j++) {
					RING(cur, j) = profits[begin];
			}
		} else {
			MPI_Recv(send_recv_buf, capacity_block_size, MPI_INT,
			         processor_id-1, TAG_PIPELINE, MPI_COMM_WORLD, &status);
			array_to_ring(&buffer,
			              send_recv_buf,
			              cur,
			              capacity_block_begin,
			              capacity_block_size);
		}

		//debug_printf("BEGIN BLOCK n_block_size=%d capacity_block_size=%d\n", n_block_size, capacity_block_size);

		i = n_block_begin;
		for(s=0; s<n_block_size; s++, i++) {

			prev = NEXT_RING(&buffer, prev);
			cur = NEXT_RING(&buffer, cur);

			j = capacity_block_begin;
			t_edge = MIN(capacity_block_size, weights[i]-capacity_block_begin);
			for(t=0; t<t_edge;t++, j++) {
					RING(cur, j) = RING(prev, j);
			}	
			t_edge = MAX(0, weights[i]-capacity_block_begin);
			for(t=t_edge;t<capacity_block_size;t++, j++) {
					RING(cur, j) = MAX(RING(prev, j),
					                   profits[i]+RING(prev, j-weights[i]));
			}
		}


		if (processor_id == num_processors-1) {
			ring_to_array(&buffer,
					cur,
					&master_result[capacity_block_begin],
					capacity_block_begin,
					capacity_block_size);
			//debug_print_row("master_result after block", master_result, capacity_block_begin+capacity_block_size);
		} else {
			ring_to_array(&buffer,
					cur,
					send_recv_buf,
					capacity_block_begin,
					capacity_block_size);
			MPI_Send(send_recv_buf, capacity_block_size, MPI_INT,
			         processor_id+1, TAG_PIPELINE, MPI_COMM_WORLD);
		}

		//debug_printf("-----------------\n\n");

		// ensure loop progress when n_block_size==0 (certain edge cases)
		j = capacity_block_begin + capacity_block_size;
	}

	delete_ring_buffer(&buffer);
}

void find_last_row_sequential(int begin, int end, int capacity, int *master_result) {
	int *cur = cur_recycled;
	int *prev = prev_recycled;
	int *tmp;
	int i, j;
	int j_end;

	// first row...
	for(j=0;j<MIN(weights[begin], capacity+1);j++) {
		cur[j] = 0;
	}
	for(j=weights[begin];j<capacity+1;j++) {
		cur[j] = profits[begin];
	}

	// remaining rows...
	for(i=begin+1;i<=end;i++) {

		SWAP(cur, prev);

		j_end = MIN(weights[i], capacity+1);
		for(j=0;j<j_end;j++) {
			cur[j] = prev[j];
		}
		for(j=weights[i];j<capacity+1;j++) {
			cur[j] = MAX(prev[j],profits[i]+prev[j-weights[i]]);
		}
	}

	memcpy(master_result, cur, (capacity+1)*sizeof(int));
}

/**
 * Recursively calculates the solution vector for the items to pick from begin to end (inclusive) that
 * fit within capacity.
 *
 * Used by the master process only.
 */
void solve(int begin, int end, int capacity, ProfileListItem *profile_item) {
	int i;
	int mid;
	int *profits_a = last_row_a_recycled;
	int *profits_b = last_row_b_recycled;
	int profit;
	int optimal_profit;
	int optimal_capacity_a;
#ifdef DEBUG
	Timer timer;
#endif

	// Base case: remaining problem is trivial.
	if (begin == end) {
		if (weights[begin] > capacity) {
			solution[begin] = 0;
		} else {
			solution[begin] = 1;
		}
	} else {

#ifdef DEBUG
		reset_timer(&timer);
		start_timer(&timer);
#endif

		mid = begin + (end - begin) / 2;

		// first, split the items into two sets and figure out all possible profits for each set.
		find_last_row(begin, mid, capacity, profits_a);
		find_last_row(mid + 1, end, capacity, profits_b);

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

#ifdef DEBUG
		stop_timer(&timer);
		if (profile_item) {
			profile_item->time += elapsed_time(&timer);
		}
#endif


		//debug_printf("[ master ] solve(%d, %d, %d): optimal capacity for a is %d, profit %d\n",
		//		begin, end, capacity, optimal_capacity_a, optimal_profit);

		// third, now that we know how much capacity to allocate to each sub problem, solve for it.
		solve(begin, mid, optimal_capacity_a, profile_item->next);
		solve(mid + 1, end, capacity - optimal_capacity_a, profile_item->next);
	}
}



int main(int argc, char **argv) {

	init(argc, argv);

	MPI_Barrier(MPI_COMM_WORLD); // wait for all processes to be ready before starting timer.

	if (is_master) {
		master_main();
	} else {
		slave_loop();
	}

	delete_problem();
	my_exit(0);
	return 0;
}

void master_main() {
	int optimal_profit;
	int i;
	Timer timer;
	Profile profile;
#ifdef DEBUG
	char stripped_problem_filename[256];
	char profile_seq_filename[256];
	char profile_filename[256];
#endif

	simple_profile_init_depthfirst(&profile, log2(N));

	initialize_timer(&timer, "timer");
	start_timer(&timer);

	solve(0, N - 1, C, profile.head);

	stop_timer(&timer);

	optimal_profit = 0;
	for (i = 0; i < N; i++) {
		if (solution[i])
			optimal_profit += profits[i];
	}

	printf("The optimal profit is %d Time taken : %lf.\n", optimal_profit, elapsed_time(&timer));
	// End of "Solve for the optimal profit"

#ifdef DEBUG
	if (strlen(problem_filename) < 256) {
		filename_strip(problem_filename, stripped_problem_filename);
		snprintf(profile_seq_filename, 256, "profile.%s.seq.out", stripped_problem_filename);
		snprintf(profile_filename, 256, "profile.%s.p%d.out",
	                stripped_problem_filename, num_processors);
        simple_profile_write_speedup(&profile, profile_seq_filename, profile_filename);
    }

	printf("Solution vector is: \n --> ");
	for (i=0; i<N; i++ ) {
		printf("%d ", solution[i]);
	}
	printf("\n");
#endif

	finalize();
}









/*
 * Initialization functions
 */

void init(int argc, char **argv) {
	init_mpi(&argc, &argv);
	init_debug();

	read_problem_size(argc, argv);
	alloc_weights_and_profits();
	read_weights_and_profits(fp);
	alloc_row_buffers();

	init_block_size(argc, argv);
}

void init_mpi(int *argc, char ***argv) {
	MPI_Init(argc, argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &processor_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processors);
}

void init_debug() {
#ifdef VERBOSEDEBUG
	char filename[256];
	snprintf(filename, 256, "verbose.%d.out", processor_id);
	debug_out = fopen(filename, "w");
	if (debug_out == NULL) {
		printf("error: failed to open verbose output file %s\n", filename);
		exit(1);
	}
#endif
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
	int bcast_buf[2];

	if (is_master) {
		// Read input file (# of objects, capacity, (per line) weight and profit )
		if (argc > 1) {
			problem_filename = argv[1];
			fp = fopen(problem_filename, "r");
			if (fp == NULL) {
				printf("[ERROR] : Failed to read file named '%s'.\n", problem_filename);
				my_exit(1);
			}
		} else {
			printf("USAGE : %s [filename].\n", argv[0]);
			my_exit(1);
		}

		problem_file_readline(fp, &N, &C);
		printf("The number of objects is %d, and the capacity is %d.\n", N, C);

		bcast_buf[0] = N;
		bcast_buf[1] = C;
	}

	MPI_Bcast(bcast_buf, 2, MPI_INT, master, MPI_COMM_WORLD);
	N = bcast_buf[0];
	C = bcast_buf[1];
}

void init_block_size(int argc, char **argv) {
	int blocksize;
	char *endptr;
	if (argc > 2) {
		blocksize = strtol(argv[2], &endptr, 10);
		if (*argv[2]!='\0' && *endptr=='\0') {
			CAPACITY_BLOCK_SIZE = blocksize;
		} else {
			if (is_master) {
				printf("warning: failed to set blocksize '%s'\n", argv[2]);
			}
		}
	}
}

void read_weights_and_profits(FILE *fp) {
	int i;

	if (is_master) {
		for (i = 0; i < N; i++) {
			problem_file_readline(fp, &(weights[i]), &(profits[i]));
		}

		fclose(fp);
	}

	MPI_Bcast(weights_and_profits, N*2, MPI_INT, master, MPI_COMM_WORLD);

	max_weight = 0;
	for(i=0;i<N;i++) {
		max_weight = MAX(max_weight, weights[i]);
	}
}
/*
 * Each processor in the wavefront pipeline processes at least max-weight
 * columns.  This means some processes are operating on data "beyond" the
 * normal row length.  Almost no computation is done in this case but memory addresses
 * are accessed.  This causes segfaults if the row buffers are not extended sufficiently.
 * To prevent this the length used is (max_block_width * num_processors) instead of
 * (C+1).
 *
 * Note that this depends on max_weight, which means read_weights_and_profits must
 * be called before alloc_row_buffers.
 */
void alloc_row_buffers() {
	int row_size = C+1;

#ifdef DEBUG
	if (is_master) {
		printf("[ master ] row_size = %d\n", row_size);
	}
#endif
	row_buffers = (int*)malloc(4 * row_size * sizeof(int));
	cur_recycled        = &row_buffers[0*row_size];
	prev_recycled       = &row_buffers[1*row_size];
	last_row_a_recycled = &row_buffers[2*row_size];
	last_row_b_recycled = &row_buffers[3*row_size];

	if (row_buffers == NULL) {
		printf("[%d] [ERROR] : Failed to allocate memory for recursive solve\n", processor_id);
		exit(1);
	}

	//saved_prev_recycled = (int*)malloc(N_BLOCK_SIZE(1,N)*max_weight*sizeof(int));
	//if (saved_prev_recycled == NULL) {
	//	printf("[%d] [ERROR] : Failed to allocate memory for tiling\n", processor_id);
	//	exit(1);
	//}

	send_recv_buf_recycled = (int*)malloc(CAPACITY_BLOCK_SIZE*sizeof(int));
	if (send_recv_buf_recycled == NULL) {
		printf("[%d] [ERROR] : Failed to allocate memory for wavefront communication\n", processor_id);
		exit(1);
	}
}

void delete_problem() {
	free(weights_and_profits);
	free(row_buffers);
}

void problem_file_readline(FILE *fp, int *a, int *b) {
	int count;

	count = fscanf(fp, "%d %d", a, b);
	if (count != 2) {
		printf("[ERROR] : Input file is not well formatted.\n");
		my_exit(1);
	}
}

void my_exit(int code) {
#ifdef VERBOSEDEBUG
	if (debug_out) { fclose(debug_out); }
#endif
	MPI_Finalize();
	exit(code);
}






/*
 * Receives commands broadcast by the master.
 */
void slave_loop() {
	int msg_buffer[MSG_LEN];

	for(;;) {
		MPI_Bcast(msg_buffer, MSG_LEN, MPI_INT, master, MPI_COMM_WORLD);
		if (msg_buffer[MSG_ACTION] == ACTION_FIND_LAST_ROW) {

			find_last_row_partial(msg_buffer[MSG_BEGIN],
			              msg_buffer[MSG_END],
			              msg_buffer[MSG_CAPACITY],
			              NULL);
		} else if (msg_buffer[MSG_ACTION] == ACTION_FINALIZE) {
			my_exit(0);
		} else {
			printf("%d: received invalid MSG_ACTION %d\n", processor_id, msg_buffer[MSG_ACTION]);
			my_exit(1);
		}
	}
}

void debug_printf(char *fmt, ...) {
	va_list argptr;
	va_start(argptr,fmt);
#ifdef VERBOSEDEBUG
	vfprintf(debug_out, fmt, argptr);
	fflush(debug_out);
#endif
	va_end(argptr);
}

void debug_print_row(char *label, int *row, int length) {
#ifdef VERBOSEDEBUG
	int i;
	debug_printf("%s ::: ", label);
	for(i=0;i<length;i++) {
		debug_printf("%d ", row[i]);
	}
	debug_printf("\n");
#endif
}

