// -----------------------------------------------------------
// NAME : Gunnar Matson					   User ID :  gjmatson
// DUE DATE : 02/16/2026
// PROGRAM ASSIGNEMENT #3
// FILE NAME : thread-main.c
// PROGRAM PURPOSE :
// This program runs a concurrent even odd sorting alorithum
// -----------------------------------------------------------

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread.h"

// Argument structure for threads
typedef struct {
	int i; 		// Index to be compared and swapped
	int *arr;
} args;

/** 
 * This function will create the threads to run the swaps for both
 * even and odd passes and return 1 if there was any swaps else 0
 * pass:	0 for even 1 for odd
 * x: 		array of integers
 * n:		length of array
 * returns:	1 if there were any swaps else 0
 */
int sort(int pass, int *x, int n) {
	char s[256];
	int l;

	// Check pass argument
	if (pass != 1 && pass != 0) {
		l = sprintf(s, "Error running sort function argument pass = %d should be 1 or 0 only\n", pass);
		write(1, s, l);
		exit(1);
	}

	// Create threads
	int threadCount = (n - (1 + pass) + 1) / 2;
	pthread_t threads[threadCount];		// Array of threads
	int swapped = 0;	// Tracks if there is a swap
	int id = 0; 		// Thread counter
	for (int i = 1 + pass; i < n; i += 2) {
		args *arg = malloc(sizeof(args));
		if (arg == NULL) {
			l = sprintf(s, "Argument allocation failed\n");
			write(1, s, l);
			exit(1);
		}
		arg->i = i;
		arg->arr = x;

		if (pthread_create(&threads[id], NULL, compareAndSwap, arg) != 0) {
			l = sprintf(s, "Thread create failed\n");
			write(1, s, l);
			exit(1);
		}
		id++;
	}
	
	int *exitStatus = NULL;
	for (int i = 0; i < threadCount; i++) {
		if (pthread_join(threads[i], (void **)&exitStatus) != 0) {
			l = sprintf(s, "Thread join failed\n");
			write(1, s, l);
			exit(1);
		}

		if (exitStatus == NULL) {
			exit(1);
		} 
		swapped = swapped || *exitStatus;
		free(exitStatus);
	}
	return swapped;
}

int main() {
	char s[256];
	int l;

	l = sprintf(s, "Concurrent Even-Odd Sort\n\n");
	write(1, s, l);

	// Get array length
	int intCount;
	if (scanf("%d", &intCount) == EOF) {
		l = sprintf(s, "Error reading in array length\n");
		write(1, s, l);
		return 1;
	}
	
	// Read in array
	int *arr = malloc(intCount * sizeof(int));
	if (!arr) {
		l = sprintf(s, "Failed to allocat array\n");
		write(1, s, l);
		return 1;
	}
	for (int i = 0; i < intCount; i++) {
		if (scanf("%d ", &arr[i]) == EOF) {
			l = sprintf(s, "Error reading array element\n");
			write(1, s, l);
			return 1;
		}
	}

	// Print array and length
	l = sprintf(s, "Number of input data = %d\nInput array:\n", intCount);
	write(1, s, l);
	for (int i = 0; i < intCount; i++) {
		l = sprintf(s, "%4d", arr[i]);
		write(1, s, l);
	}
	write(1, "\n", 1);

	// Start sort
	int interation = 0;
	int swapped = 1;
	while (swapped) {
		interation++;	// Increment the interation counter
		l =sprintf(s, "Iteration %d:\n", interation);
		write(1, s, l);

		// Run even pass
		l = sprintf(s, "    Even Pass:\n");
		write(1, s, l);
		swapped = sort(0, arr, intCount);

		// Run odd pass
		l = sprintf(s, "    Odd Pass:\n");
		write(1, s, l);
		int temp = sort(1, arr, intCount);
		swapped = swapped || temp;

		// Print after iteration
		l = sprintf(s, "Result after iteration %d:\n", interation);
		write(1, s, l);
		for (int i = 0; i < intCount; i++) {
			l = sprintf(s, "%4d", arr[i]);
			write(1, s, l);
		}
		write(1, "\n", 1);
	}

	// Print sorted array
	l = sprintf(s, "Final result after iteration %d:\n", interation);
	write(1, s, l);
	for (int i = 0; i < intCount; i++) {
		l = sprintf(s, "%4d", arr[i]);
		write(1, s, l);
	}
	write(1, "\n", 1);

	return 0;
}
