#include <assert.h>
#include <stdio.h>
#include "ringbuffer.h"

void main(int argc, char **argv) {
	RingBuffer buffer;
	int *ring;
	int i, j;

	buffer.mask = 0x3; // 0b11
	ring = 0;
	for(i=0;i<10;i++) {
		assert(PTR_TO_RING_ADDR(&buffer, &ring[i]) == i%4);
	}


	new_ring_buffer(&buffer, 1, 1);
	assert(buffer.ring_len == 0x1); // ring of size 1
	assert(buffer.mask == 0x0);    // No addresses, so mask away all differences
	printf("5 & 0 = %d\n", (5 & 0));
	printf("buffer.begin & buffer.mask = %lld\n", ((ptr_mask)buffer.begin & buffer.mask));
	assert(PTR_TO_RING_ADDR(&buffer, buffer.begin) == 0);// tautalogy
	buffer.begin[0] = 1; // test write
	delete_ring_buffer(&buffer);

	new_ring_buffer(&buffer, 2, 1);
	assert(buffer.ring_len == 2); // ring of size 2
	assert(buffer.mask == 0x1); // 1 bit specifies address within ring
	assert(PTR_TO_RING_ADDR(&buffer, buffer.begin) == 0);// rings begin at address ______0
	for(i=0;i<2;i++) {
		buffer.begin[i] = i; // test write
	}
	delete_ring_buffer(&buffer);

	new_ring_buffer(&buffer, 3, 1);
	assert(buffer.ring_len == 4); // ring of size 4
	assert(buffer.mask == 0x3); // 2 bits specify address within ring
	assert(PTR_TO_RING_ADDR(&buffer, buffer.begin) == 0);// rings begin with address ___00
	for(i=0;i<4;i++) { buffer.begin[i] = i; }
	delete_ring_buffer(&buffer);

	new_ring_buffer(&buffer, 18000, 1);
	assert(buffer.ring_len == 32768); // ring of size 2^15
	assert(buffer.mask == 0x7fff); // 15 bits specify ring address
	assert(PTR_TO_RING_ADDR(&buffer, buffer.begin) == 0); // begin with ___000000000000000
	for(i=0;i<32768;i++) {buffer.begin[i] = i;}
	delete_ring_buffer(&buffer);

	new_ring_buffer(&buffer, 3, 7);
	assert(buffer.ring_len == 4); // each ring has size 4
	assert(buffer.mask == 0x3); // 2 bits specify address within ring
	assert(((ptr_mask)buffer.begin & buffer.mask) == 0x0); // begin with ___00
	for(i=0;i<7;i++) {
		ring = &buffer.begin[i*buffer.ring_len];
		assert(PTR_TO_RING_ADDR(&buffer, ring) == 0);
		for(j=0;j<4;j++) {
			ring[j] = i+j;
		}
	}
	delete_ring_buffer(&buffer);
}
