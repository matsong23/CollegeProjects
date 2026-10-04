// -----------------------------------------------------------
// NAME: Gunnar Matson						 User ID: gjmatson
// DUE DATE: 01/30/2026
// PROGRAM ASSIGNMENT #2
// FILE NAME: main.c
// PROGRAM PURPOSE:
// Reads in an array and creates a child to run merge program
// -----------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SHM_NAME "/gjmatson_shm"
#define SHM_SIZE 4096

// If an error occurs this function will clean up the shared memory
void leave(int *mem) {
	munmap(mem, SHM_SIZE);
	shm_unlink(SHM_NAME);
	exit(1);
}

int main() {
	int len;		// Length of string for printing	
	char s[255];	// String for printing
	len = sprintf(s, "Merge Sort with Multiple Processes:\n\n");
	write(1, s, len);
	
	// Set up shared memory
	int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	ftruncate(fd,SHM_SIZE);
	int *ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) leave(ptr);

	// get Number of elements to be in the array
	int inputLen;
	if (scanf("%d", &inputLen) == EOF) leave(ptr);

	// add elements it shared memory array
	for (int i = 0; i < inputLen; i++) {
		if (scanf("%d ", &ptr[i]) == EOF) leave(ptr);
	}

	// print out given array
	len  = sprintf(s, "Input array for mergesort has %d elements:\n", inputLen);
	write(1, s, len);
	for (int i = 0; i < inputLen; i++) {
		len = sprintf(s, "%5d", ptr[i]);
		write(1, s, len);
	}
	len = sprintf(s, "\n");
	write(1, s, len);

	// Create child to run the merge program
	len = sprintf(s, "*** MAIN: about to spawn the merge sort process\n");
	write(1, s, len);
	int cStatus;
	pid_t c = fork();
	if (c == -1) leave(ptr);
	if (c == 0) {
		// Get args
		char idxs[128];
		char idxe[128];
		char shmSize[128];
		char firstc[128];
		char pid[128];
		snprintf(idxs, sizeof(idxs), "%d", 0);
		snprintf(idxe, sizeof(idxs), "%d", inputLen - 1);
		snprintf(shmSize, sizeof(shmSize), "%d", SHM_SIZE);
		snprintf(firstc, sizeof(firstc), "%d", 1);
		snprintf(pid, sizeof(pid), "%d", getpid());

		// start idx, end idx, shm name, shm size, 1 for if the process was created by main process
		char *args[] = {"./merge", idxs, idxe, SHM_NAME, shmSize, pid, firstc, NULL};
		execvp(args[0], args);

		exit(1);
	}
	waitpid(c, &cStatus, 0);
	// Check if child exited with and error and handle error if so
	if (WEXITSTATUS(cStatus) == 1) leave(ptr);
	
	// Print out sorted array
	len = sprintf(s, "*** MAIN: merged array:\n");
	write(1, s, len);
	for (int i = 0; i < inputLen; i++) {
		len = sprintf(s, "%5d", ptr[i]);
		write(1, s, len);
	}
	len = sprintf(s, "\n");
	write(1, s, len);

	leave(ptr); 	// exit program
}

