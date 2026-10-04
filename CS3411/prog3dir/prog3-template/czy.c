#include "bitsy.h" // This header includes prototypes for the proposed bit abstractions
/* Include any additional headers you requirei */
#include <stdio.h>
#include <stdlib.h>

/* You may use any global variables/structures that you like */
typedef struct Node {
	unsigned short byte;
	struct Node * next;
} Node;

Node *head = NULL;

int getByteIdx(unsigned short byte) {
	Node *temp = head;
	for (int i = 0; temp != NULL; i++) {
		if (temp->byte == byte) return i;
		temp = temp->next;
	} 
	return -1;
}

unsigned short getBinary(int i) {
	unsigned short b = 0;
	if (i >= 4) {
		b |= 1 << 2;
		i -= 4;
	}
	if (i >= 2) {
		b |= 1 << 1;
		i -= 2;
	}
	if (i >= 1) {
		b |= 1 << 0;
		i -= 1;
	}
	return b;
}

void addNode(unsigned short byte){
	Node *node = (Node *)malloc(sizeof(Node));
	node->byte = byte;
	node->next = head;
	head = node;
	
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


/* main - czy compression implementation
 * Develop a program called czy which compresses the data stream directed 
 * at its standard input and writes the compressed stream to its standard 
 * output.
 *
 * The compression algorithm reads the input one symbol (i.e., byte) at a 
 * time and compares it with each of the 8 bytes previously seen. It also 
 * checks to see if the following n characters are the same as the current 
 * symbol. If the byte has been previously seen, it outputs the position of 
 * the previous byte relative to the current position. Otherwise, the symbol 
 * is output as is by prefixing it with a binary one.
 *
 * To compile czy: make czy
 * To execute: ./czy < somefile.txt > somefile.encoded
 */
int main() {
    // The implementation of your encoder should go here.

    // It is recommeded that you implement the bit abstractions in bitsy.c and
    // utilize them to implement your encoder. 
    // If so, do NOT call read/write here. Instead rely exclusively on 
    // read_bit(), read_byte(), write_bit(), write_byte(), and flush_write_buffer().	

	unsigned short byte;
	for (int i = 0; (byte = read_byte()) != 256; i++) {
		int idx = getByteIdx(byte);
		addNode(byte);
		if (idx == -1) {
			write_bit(1);
			write_byte(byte);
		} else {
			unsigned short b = getBinary(idx);
			write_bit(0);
            write_bit((b >> 0) & 1);
            write_bit((b >> 1) & 1);
            write_bit((b >> 2) & 1);
		}
	}
	
	// Add out of range value to determine when the end is
	unsigned short eof = 250;
	write_bit(1);
	write_byte(eof);

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
