// -------------------------------------------------------------------------------------
// NAME: Gunnar Matson												   USER ID: gjmatson
// DUE DATE: 01/30/2026
// PROGRAM ASSIGNMENT #2
// FILE NAME: merge.c
// PROGRAM PURPOSE: 
// This program will take create child processes to concurrently sort an array in shared
// memory. it is run by the main process created in the main.c file
// -------------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int len;
	char s[4096];

	// get args
	if (argc != 7) return 1;
	int idxs = atoi(argv[1]);
	int idxe = atoi(argv[2]);
	char *shmName = argv[3];
	int shmSize = atoi(argv[4]);
	int parentId = atoi(argv[5]);
	int firstc = atoi(argv[6]);

	// open shared memory
	int fd = shm_open(shmName, O_RDWR, 0666);
	if (fd == -1) return 1;
	int *ptr = mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) return 1;
	
	// print out process info
	if (firstc) {
		len = sprintf(s, "   ### M-PROC(%d): entering with a[%d..%d]\n", getpid(), idxs, idxe);
	} else {
		len = sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d]\n",
			getpid(), parentId, idxs, idxe);
	} 
	
	// print out section of the array
	for (int i = idxs; i <= idxe; i++) {
		len = sprintf(s, "%s%5d", s, ptr[i]);
	}
	len = sprintf(s, "%s\n", s);
	write(1, s, len);	

	// Create child process
	int middle = idxs + (idxe - idxs) / 2;
	int totalInts = idxe - idxs + 1;
	// Get indexs for left and right children
	int ls = idxs;
	int le = middle;
	int rs = middle +1;
	int re = idxe;
	if (totalInts != 2) {
		// Get process id for children	
		char pid[128];
		snprintf(pid, sizeof(pid), "%d", getpid());
	
		// Create left child
		int lchild = fork();
		if (lchild == -1) return 1;
		if (lchild == 0) {
			// Get agrs
			char start[128];
			char end[128];
			char notf[128];
			snprintf(start, sizeof(start), "%d", ls);
			snprintf(end, sizeof(end), "%d", le);
			snprintf(notf, sizeof(notf), "%d", 0);

			// Run merge on child
			char *args[] = {"./merge", start, end, shmName, argv[4], pid, notf};
			execvp(args[0], args);
			exit(1);
		}
	
		// Create right child
		int rchild = fork();
		if (rchild == -1) return 1;
		if (rchild == 0) {	
			// get args		
			char start[128];
	        char end[128];
	        char notf[128];
	        snprintf(start, sizeof(start), "%d", rs);
	        snprintf(end, sizeof(end), "%d", re);
	        snprintf(notf, sizeof(notf), "%d", 0);

			// run merge on child
    	    char *args[] = {"./merge", start, end, shmName, argv[4], pid, notf};
        	execvp(args[0], args);
        	exit(1);
		}
	
		// wait for both children
		int lstatus;
		int rstatus;
		waitpid(lchild, &lstatus, 0);
		waitpid(rchild, &rstatus, 0);
		// Check the exit status of children to see if an error occur and if so exit with same status
		if (WEXITSTATUS(lstatus) == 1 || WEXITSTATUS(rstatus) == 1) return 1;
	}

	// Base case array of 2. Sort array
	if (totalInts == 2) {
		if (ptr[idxs] > ptr[idxe]) {
			int temp = ptr[idxs];
			ptr[idxs] = ptr[idxe];
			ptr[idxe] = temp;
		}
		
		// Print array now that it is sorted
		len = sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): entring a[%d..%d] -- sorted\n", 
			getpid(), parentId, idxs, idxe);
		len = sprintf(s, "%s%5d%5d\n", s, ptr[idxs], ptr[idxe]);
		write(1, s, len);
		return 0; 
	}

	// Merge
	len = sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): both array sections sorted. start merging\n", 
		getpid(), parentId);
	write(1, s, len);
	
	// temp array of elements a[idxs..idxe]
	int tempArr[totalInts];
	for (int i = idxs; i <= idxe; i++) {
		tempArr[i-idxs] = ptr[i];
	}
	// create a process for each number in the array 
	// have them find the correct index for their number and put their number there 	
	int pArr[totalInts];
	for (int i = idxs; i <= idxe; i++) {
		int val = tempArr[i - idxs]; // Child process's assigned value
		
		int pid = getpid();
		pArr[i - idxs] = fork();
		if (pArr[i - idxs] == -1) return 1;
		if (pArr[i - idxs] == 0 ) {
			len = sprintf(s, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n",
				getpid(), pid, i, val);
			write(1, s, len);

			int inLeft = 1;
			if (i > le) inLeft = 0;
			if (inLeft) {
				if (val < tempArr[rs - idxs]) {
					len = sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d, move to a[%d]\n", 
						getpid(), i, val, rs, tempArr[rs - idxs], i);
					write(1, s, len);
					ptr[(i)] = val;
				} else if (val >= tempArr[re - idxs]) {
					len = sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d, move to a[%d]\n",
						getpid(), i, val, re, tempArr[re - idxs], (le - ls + 1) + (i));
					write(1, s, len);
					ptr[(le - ls + 1) + (i)] = val;
				} else {
					int k;
					int low = rs - idxs;
					int up = re - idxs;
					while (up >= low) {
						k = low + ((up - low)/2 );
						
						if (tempArr[k] < val) {
							low = k  + 1;
						} else up = k -1;
					}

					len = sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d, move to a[%d]\n",
						getpid(), i, val, idxs + low - 1, tempArr[low - 1], low +idxs, tempArr[low], idxs +(i-idxs) + low);
					write(1, s, len);
					ptr[idxs +(i-idxs) + low] = val;
				}
			} else {
				if (val < tempArr[ls - idxs]) {
					len = sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d, move to a[%d]\n",
						getpid(), i, val, ls, tempArr[ls - idxs], idxs + (i - (middle +1)));
					write(1, s, len);
					ptr[idxs + (i - (middle +1))] = val;
				} else if (val >= tempArr[le - idxs]) {
					len = sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d, move to a[%d]\n", 
						getpid(), i, val, le, tempArr[le - idxs], idxs + (le - ls + 1) + (i - (middle + 1)));
					write(1, s, len);
					ptr[idxs + (le - ls + 1) + (i - (middle + 1))] = val;
				} else {
					int k;
					int low = ls - idxs;
					int up = le - idxs;
					while (up >= low) {
						k = low + ((up- low) / 2);

						if (tempArr[k] < val) {
							low = k + 1;
						} else up = k - 1;
					}

					len = sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d, move to a[%d]\n",
						getpid(), i, val, idxs + low -1 - (ls -idxs), tempArr[low - 1], low +idxs, tempArr[low], idxs +(i - (middle+1) ) +low);
					write(1, s, len);
					ptr[idxs +(i - (middle+1) ) +low] = val;
				}
			}

			len  = sprintf(s, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is terminated\n", 
				getpid(), pid, i, val);
			write(1, s, len);
			exit(0);
		}
	}
	// Wait for children
	for (int i = 0; i < totalInts; i++) {
		waitpid(pArr[i], NULL, 0);
	}

	// Print that the merge is complete and the array
	len = sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): merge sort a[%d..%d] complete:\n", 
		getpid(), parentId, idxs, idxe);
	for (int i = idxs; i <= idxe; i++) {
		len = sprintf(s, "%s%5d", s, ptr[i]);
	}
	len = sprintf(s, "%s\n", s);
	write(1, s, len);

	return 0;
}
