#include "bitsy.h" // This header includes prototypes for the proposed bit abstractions
#include <stdlib.h>
#include <stdio.h>

/* Include any additional headers you require */

/* You may use any global variables/structures that you like */
typedef struct Node {
	unsigned short byte;
	struct Node *next;
} Node;

Node *head = NULL;

unsigned short getByte(unsigned short idx) {
	Node *t = head;
	for (int i = 0; i < idx && t != NULL; i++) {
		t = t->next;
	}
	if (t != NULL) {
		return t->byte;
	}
	return -1;
}

void addNode(unsigned short byte) {
	Node *n = (Node *)malloc(sizeof(Node));
	if (head == NULL) {
		n->byte = byte;
		n->next = NULL;
		head = n;
	} else {
		n->byte = byte;
		n->next = head;
		head = n;
	}
	Node *t = head;
	for (int i = 0; i < 7 && t != NULL; i++) {
		t = t->next;
	}
	if (t != NULL && t->next != NULL) {
		free(t->next);
		t->next = NULL;
		return;
	}
	return;
}

unsigned short getIdx() {
	unsigned short idx = 0;
	unsigned short b = read_bit();
	if (b == 1) idx |= 1 << 0; 
	else idx &= ~(1 << 0);

	b = read_bit();
	if (b == 1) idx |= 1 << 1; 
	else idx &= ~(1 << 1);

	b = read_bit();
	if (b == 1) idx |= 1 << 2;
	else idx &= ~(1 << 2);

	return idx;
}

/* main - dzy de-compression implementation
 * This program decompresses a compressed stream directed at its standard input 
 * and writes decompressed data to its standard output.
 *
 * To compile dzy: make dzy
 * To execute: ./dzy < somefile.encoded > somefile_decoded.txt
 */
int main() {
    // The implementation of your decoder should go here.

    // It is recommeded that you implement the bit abstractions in bitsy.c and
    // utilize them to implement your decoder.
    // If so, do NOT call read/write here. Instead rely exclusively on 
    // read_bit(), read_byte(), write_bit(), write_byte(), and flush_write_buffer().
	
	unsigned short bit;
	unsigned short byte;
	unsigned short idx; 
	for (int i = 0; (bit = read_bit()) != 255; i++) {
		if (bit == 1) {
			byte = read_byte();
			if (byte == 256) return -1;
			if (byte == 250) break;
			write_byte(byte);
		} else {
			idx = getIdx();
			byte = getByte(idx);
			write_byte(byte);
		}
		addNode(byte);
	}
	
	flush_write_buffer();
		
	// Free list
	if (head == NULL) return 0;
	Node *t = head->next;
	while (t != NULL) {
		free(head);
		head = t;
		t = t->next;
	}
	free(head); 

    return 0; // exit status. success = 0, error = -1
}
