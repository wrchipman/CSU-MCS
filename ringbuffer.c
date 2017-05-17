#include "ringbuffer.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>

double log2(double x);

void new_ring_buffer(RingBuffer *buffer, int min_length, int num_rings) {
	ptr_mask offset_to_ring;
	buffer->num_rings = num_rings;
	buffer->ring_len = 1 << (int)(ceil(log2(min_length)));
	buffer->mask = buffer->ring_len-1;
	buffer->bounding_buffer_len = buffer->ring_len*(num_rings+1)*sizeof(int);
	buffer->bounding_buffer = (int *)malloc(buffer->bounding_buffer_len);
	if (buffer->bounding_buffer == NULL) {
		printf("Failing new_ring_buffer: buffer->ring_len = %d, mask = %lld, buffer_len = %d\n",
				buffer->ring_len, buffer->mask, buffer->bounding_buffer_len);
		buffer->begin = NULL;
		return;
	}
	offset_to_ring = (buffer->ring_len - PTR_TO_RING_ADDR(buffer, buffer->bounding_buffer)) & buffer->mask;
	buffer->begin = &buffer->bounding_buffer[offset_to_ring];
}

void delete_ring_buffer(RingBuffer *buffer) {
	free(buffer->bounding_buffer);
}

/**
 * Copies the contents of an array into a ring buffer.
 */
void array_to_ring(RingBuffer *buffer, int *array, Ring ring, int begin, int count) {
	int i;
	int mask = buffer->mask;

	for(i=0;i<count;i++) {
		RING(ring, begin+i) = *array;
		array += 1;
	}
}

void ring_to_array(RingBuffer *buffer, Ring ring, int *array, int begin, int count) {
	int i;
	int mask = buffer->mask;

	for(i=0;i<count;i++) {
		*array = RING(ring, begin+i);
		array += 1;
	}
}
