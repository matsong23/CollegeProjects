// -----------------------------------------------------------
// NAME : Gunnar Matson						 User ID: gjmatson
// DUE DATE : 02/16/2026
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.c
// PROGRAM PURPOSE :
// Contains compare and swap thread function for concurrent
// even odd sort
// -----------------------------------------------------------

#include "thread.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Input argument structer
typedef struct {
	int i;		// Index to compare and swap
	int *arr;	// Array of ints
} args;

void *compareAndSwap(void *arg) {
	char s[256];
	int l;

	args *a = (args*)arg;

	l = sprintf(s, "     Thread %d Created\n", a->i);
	write(1, s, l);

	// Allocate swap value for exit status to main thread
	int *swapVal = malloc(sizeof(int));
	if (swapVal == NULL) {
		sprintf(s, "Malloc failed in thread\n");
		write(1, s, l);
		pthread_exit(NULL);
	}
	
	// Check if swap needed
	l = sprintf(s, "     Thread %d compares x[%d] = %d and x[%d] = %d\n", a->i, a->i - 1, a->arr[a->i - 1], a->i, a->arr[a->i]);
	write(1, s, l);
	*swapVal = 0;
	if (a->arr[a->i - 1] > a->arr[a->i]) {
		int t = a->arr[a->i - 1];
		a->arr[a->i - 1] = a->arr[a->i];
		a->arr[a->i] = t;
		*swapVal = 1;
		l = sprintf(s, "     Thread %d swaps x[%d] = %d and x[%d] = %d\n"    , a->i, a->i - 1, a->arr[a->i - 1], a->i, a->arr[a->i]);
		write(1, s, l);
	}

	l = sprintf(s, "     Thread %d exits\n", a->i);
	write(1, s, l);
	
	free(a);	// Free the allocated argument for thread

	// Exit Thread with swap value as exit status
	pthread_exit(swapVal);
}
