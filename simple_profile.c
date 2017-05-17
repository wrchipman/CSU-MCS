#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_profile.h"

static void _read_profile(Profile *p, char *filename);

void simple_profile_init(Profile *p) {
	char name[256];
	snprintf(name, 256, "Profile%p", p);
	initialize_timer(&p->timer, name);
	p->head = NULL;
	p->tail = NULL;
}
void simple_profile_record(Profile *p, int num_tasks) {
	ProfileListItem *x = (ProfileListItem *)malloc(sizeof(ProfileListItem));
	x->num_tasks = num_tasks;
	x->time = -1;
	x->next = NULL;

	if (p->tail == NULL) {
		p->head = x;
		p->tail = x;
	} else {
		stop_timer(&p->timer);
		p->tail->time = elapsed_time(&p->timer);
		p->tail->next = x;
		reset_timer(&p->timer);
	}

	start_timer(&p->timer);
	p->tail = x;
}

void simple_profile_write(Profile *p, char *filename) {
	ProfileListItem *x;
	int i;

	FILE *f = fopen(filename, "w");
	if (!f) {
		printf("Failed to open profile record file '%s'!\n", filename);
		return;
	}

	fprintf(f, "#iteration\tnum_tasks\ttime\n");
	i = 0;
	for(x=p->head; x!=NULL && x->next!=NULL; x=x->next) {
		fprintf(f, "%d\t%d\t%f\n", i++, x->num_tasks, x->time);
	}

	fclose(f);
}

void simple_profile_write_speedup(Profile *p, char *sequential_filename, char *filename) {
	ProfileListItem *iter, *sequential_iter;
	int i;
	Profile sequential_profile;
	FILE *f;

	if (strcmp(sequential_filename, filename) == 0) {
		simple_profile_write(p, filename);
	} else {
		_read_profile(&sequential_profile, sequential_filename);

		f = fopen(filename, "w");
		if (!f) {
			printf("Failed to open profile record file '%s'!\n", filename);
			simple_profile_delete(&sequential_profile);
			return;
		}

		fprintf(f, "#iteration\t" "num_tasks_1\t" "num_tasks_%d\t"
				   "time_1\t" "time_%d\n",
				   omp_get_num_threads(), omp_get_num_threads());
		i = 0;
		iter = p->head;
		sequential_iter = sequential_profile.head;
		while( iter!=NULL && iter->next!=NULL ) {
			fprintf(f, "%d\t%d\t%d\t%f\t%f\n",
					i++,
					(sequential_iter?sequential_iter->num_tasks:-1),
					iter->num_tasks,
					(sequential_iter?sequential_iter->time:-1),
					iter->time);

			iter=iter->next;
			if (sequential_iter!=NULL && sequential_iter->next!=NULL) {
				sequential_iter = sequential_iter->next;
			} else {
				sequential_iter = NULL;
			}
		}

		simple_profile_delete(&sequential_profile);
		fclose(f);
	}
}

void simple_profile_delete(Profile *p) {
	ProfileListItem *x, *tmp;

	free(p->timer.name);

	x = p->head;
	while(x!=NULL) {
		tmp = x;
		x = x->next;
		free(tmp);
	}
}


void filename_strip(char *filename, char *result) {
	int len = strlen(filename);
	int i;
	int index_extension;
	int index_dir;

	index_extension = len;
	while(index_extension>0 && filename[index_extension]!='.') {
		index_extension -= 1;
	}

	index_dir = len;
	while(index_dir>0 && filename[index_dir]!='/') {
		index_dir -= 1;
	}

	if (index_dir < 0 && index_extension < 0) {
		memmove(result, filename, len);
	} else if (index_dir > index_extension) {
		memmove(result, &filename[index_dir + 1], len-index_dir);
	} else {
		memmove(result, &filename[index_dir + 1], index_extension-index_dir-1);
		result[index_extension-index_dir-1] = '\0';
	}
}


/**
 * Initializes the profile and loads it with the contents of a file genarated
 * by simple_profile_write().
 */
void _read_profile(Profile *p, char *filename) {
	FILE *f = fopen(filename, "r");
	char buf[1024];
	char *r;
	int rs;
	int expected_i;
	int read_i;
	int num_tasks;
	double time;
	ProfileListItem *x;

	simple_profile_init(p);

	if (!f) {
		fprintf(stderr, "Failed to read profile %s\n", filename);
		return;
	}

	r = fgets(buf, 1024, f); // read first "#iteration num_tasks time_0" line
	if (r!=buf) {
		fprintf(stderr, "Syntax error in line 1 of %s\n", filename);
		fclose(f);
		return;
	}

	expected_i = 0;
	for(;;) {
		rs = fscanf(f, "%d\t%d\t%lf\n", &read_i, &num_tasks, &time);
		if (rs == EOF) {
			break;
		} else if (rs!=3 || read_i!=expected_i) {
			fprintf(stderr, "Syntax error in line %d of %s\n", expected_i+2, filename);
			fclose(f);
			return;
		}
		expected_i += 1;

		x = (ProfileListItem *)malloc(sizeof(ProfileListItem));
		x->num_tasks = num_tasks;
		x->time = time;
		x->next = NULL;

		if (p->tail == NULL) {
			p->head = x;
			p->tail = x;
		} else {
			p->tail->next = x;
			p->tail = x;
		}
	}
}
