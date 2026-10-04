#include "hmalloc.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
/*You may include any other relevant headers here.*/


/*Add additional data structures and globals here as needed.*/
typedef struct {
	long long len;
	void *next;
	char segment_start;
}Node; 

void *free_list = NULL;

/* traverse
 * Start at the free list head, visit and print the length of each
 * area in the free pool. Each entry should be printed on a new line.
 */
void traverse(){
    /* Printing format:
     * "Index: %d, Address: %08x, Length: %d\n"
     *    -Index is the position in the free list for the current entry.
     *     0 for the head and so on
     *    -Address is the pointer to the beginning of the area.
     *    -Length is the length in bytes of the free area.
     */

    Node *temp = (Node*) free_list;
    for(int i = 0; temp != NULL; i++) {
        printf("Index: %d, Address: %p, Length: %lld\n", i, (void*)&temp->segment_start, temp->len);
		temp = (Node*) temp->next;
	}
}

/* hmalloc
 * Allocation implementation.
 *    -will not allocate an initial pool from the system and will not 
 *     maintain a bin structure.
 *    -permitted to extend the program break by as many as user 
 *     requested bytes (plus length information).
 *    -keeps a single free list which will be a linked list of 
 *     previously allocated and freed memory areas.
 *    -traverses the free list to see if there is a previously freed
 *     memory area that is at least as big as bytes_to_allocate. If
 *     there is one, it is removed from the free list and returned 
 *     to the user.
 */
void *hmalloc(int bytes_to_allocate) {
	Node *temp = free_list;
	Node *beforeTemp = NULL;
	while (temp != NULL) {
		if (temp->len >= bytes_to_allocate) break;
		beforeTemp = temp;
		temp = temp->next;
	}	
 	
	Node *m;
	if (temp != NULL) {
		m = temp;
		if (beforeTemp == NULL) {
			free_list = temp->next;
		} else {
			beforeTemp->next = temp->next;
		}
	} else {
		void *newMem = (void *)sbrk(bytes_to_allocate + (sizeof(Node)));
		// Check if sbrk failed
    	if (newMem == (void *)-1) {
        	return (void *)-1;
    	}

		m = (Node*)newMem;
		m->len = bytes_to_allocate;
    	m->next = NULL;
	}
	
	return (&m->segment_start);
}

/* hcalloc
 * Performs the same function as hmalloc but also clears the allocated 
 * area by setting all bytes to 0.
 */
void *hcalloc(int bytes_to_allocate){
	char *m = (char*)hmalloc(bytes_to_allocate);
	for (int i = 0; i < bytes_to_allocate; i++) {
		m[i] = '\0';
	}	
    return (void*)m; //placeholder to be replaced by proper return value
}

/* hfree
 * Responsible for returning the area (pointed to by ptr) to the free
 * pool.
 *    -simply appends the returned area to the beginning of the single
 *     free list.
 */
void hfree(void *ptr){
	if (ptr == NULL) return;
	Node *m = (Node*)((char*) ptr - ((sizeof(long long)) + (sizeof(void*))));
	m->next = free_list;
	free_list = m;
}

/* For the bonus credit implement hrealloc. You will need to add a prototype
 * to hmalloc.h for your function.*/

/*You may add additional functions as needed.*/
