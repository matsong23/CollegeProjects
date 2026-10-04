// -----------------------------------------------------------
// NAME : Gunnar Matson User ID: gjmatson
// DUE DATE : 01/16/2025
// PROGRAM ASSIGNMENT 1
// FILE NAME : prog1.c
// PROGRAM PURPOSE :
// This program will run three sub programs concurrently
// nth Catalan Number, Buffon's Needle, and Pinball Game
// -----------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>

// get the nth Catalan number
int catalanNumber(int n) {
	if (n == 1 || n == 0) return 1;
	int val = 0;
	for (int i = 1; i <= n; i++) {
		val += catalanNumber(i - 1) * catalanNumber(n - i); // do Ci-1 * Cn-i
	}
	return val;
}

// simulate the prediction
int buffonsNeedle(int l, int g, int r) {
	int hits = 0; // save the number of hits
	for (int i = 0; i < r; i++) {
		double d = g * ((double) rand() / RAND_MAX);
		double a = (2 * M_PI) * ((double) rand() / RAND_MAX);
		double temp = d + (l * sin(a)); // calcualte if it hit
		if (temp < 0 || temp > g) hits++; 
	}
	
	return hits;
}

// simulate a ball drop to see which bin it goes in
int dropBall(int x) {
	double pos = x / 2.0; // which bin the ball is currently above starting in the center
	for (int i = 0; i < x - 1; i++) {
		int lr = rand() % 2; // 0 = left 1 = right
		if (lr == 0) pos -= 0.5;
		else pos += 0.5;
	}
	return (int)pos; // return as an int as it will always be a whole number
}

int main(int argc, char *argv[]) {
	srand(time(0)); // Set seed so rand() is random each timne the program is run
	int len;		// length for printing buffer
	char s[256];	// printing buf

	len = sprintf(s, "Main Process Started\n");
	write(1, s, len);
	// Get all inputs
	int n = atoi(argv[1]);
	int l = atoi(argv[2]);
	int g = atoi(argv[3]);
	int r = atoi(argv[4]);
	int x = atoi(argv[5]);
	int y = atoi(argv[6]);

	len = sprintf(s, "Catalan Input             = %d\n", n);
	write(1, s, len);
	len = sprintf(s, "Needle Length             = %d\n", l);
	write(1, s, len);
	len = sprintf(s, "Gap Length                = %d\n", g);
	write(1, s, len);
	len = sprintf(s, "Total Random Number Pairs = %d\n", r);
	write(1, s, len);
	len = sprintf(s, "Number of Bins            = %d\n", x);
	write(1, s, len);
	len = sprintf(s, "Number of Ball Droppings  = %d\n", y);
	write(1, s, len);

	int ps[3]; // array of processes
	ps[0] = fork();
	if (ps[0] < 0) {
		len = sprintf(s, "Failed to create fork\n");
		write(1, s, len);
		exit(1);
	}
	if (ps[0] == 0) {
		len = sprintf(s, "   Catalan Process Started\n");
		write(1, s, len);
		len = sprintf(s, "   Input Number %d\n", n);
		write(1, s, len);

		int res = catalanNumber(n); // get value for nth catalan number
		len = sprintf(s, "   Catalan Number C(%d) is %d\n", n, res);
		write(1, s, len);
		len = sprintf(s, "   Catalan Process Exits\n");
		write(1, s, len);
		exit(0);
	} else {
		len = sprintf(s, "Catalan Process Created\n");
		write(1, s, len);
	}
	
	ps[1] = fork();
	if (ps[1] < 0) {
		len = sprintf(s, "Failed to create fork\n");
		write(1, s, len);
		exit(1);		
	}
	if (ps[1] == 0) {
		len = sprintf(s, "         Buffon Process Started\n");
		write(1, s, len);
		len = sprintf(s, "         Needle Length %d\n", l);
		write(1, s, len);
		len = sprintf(s, "         Gap Length %d\n", g);
		write(1, s, len);
		len = sprintf(s, "         Total Random Number Pairs %d\n", r);
		write(1, s, len);
		
		int hits = buffonsNeedle(l, g, r); // Get number of simulated hits
		len = sprintf(s, "         Total Hits %d\n", hits);
		write(1, s, len);
		len = sprintf(s, "         Estimated Probbility is %f\n", ((double)hits/r));
		write(1, s, len);
		len = sprintf(s, "         Actual Probability is %f\n", ((2.0/M_PI) * ((double)l / g)));
		write(1, s, len);
		len = sprintf(s, "         Buffon Process Exits\n");
		write(1, s, len);
		exit(0);
	} else {
		len = sprintf(s, "Buffon Process Created\n");
		write(1, s, len);
	}

	ps[2] = fork();
	if (ps[2] < 0){
		len = sprintf(s, "Failed to create fork\n");
		write(1, s, len);
		exit(1);
	}
	if (ps[2] == 0) {
		len = sprintf(s, "Simple Pinball Process Started\n");
		write(1, s, len);
		len = sprintf(s, "Number of Bins %d\n", x);
		write(1, s, len);
		len = sprintf(s, "Number of Ball Droppings %d\n", y);
		write(1, s, len);
		
		int bins[x]; // array of bins
		for (int i = 0; i < x; i++) bins[i] = 0; // zero out array
		for (int i = 0; i < y; i++) {
			int binNum = dropBall(x); // get what bin ball droped in
			bins[binNum]++; // increase the bin it dropped in by one
		}
	
		// Get the max number of balls that dropped in one of the bins
		int max = bins[0];
		for (int i = 1; i < x; i++) {
			if (max < bins[i]) max = bins[i];
		}
		// Print out each row of the histagram
		for (int i = 0; i < x; i++) {
			char bar[51]; // string of the bar of the histgram
			for (int j = 0; j < 51; j++) bar[j] = ' '; // make string empty
			bar[50] = '\0'; // set the null terminator
			int length = (int)(50 * bins[i] / max + 0.5); // get the length of the bar
			// set the *'s in the bar
			for (int j = 0; j < length; j++) {
				bar[j] = '*'; 
			}

			len = sprintf(s, "%3d-(%7d)-(%5.2f)]|%s\n", (i + 1), bins[i], ((double)bins[i]/y), bar);
			write(1, s, len);	
		}
		len = sprintf(s, "Simple Pinball Process Exits\n");
		write(1, s, len);
		exit(0);
	} else {
		len = sprintf(s, "Pinball Process Created\n");
		write(1, s, len);
	}
	
	// Wait for child processes
	len = sprintf(s, "Main Process Waits\n");
	write(1, s, len);
	waitpid(ps[0], NULL, 0);
	waitpid(ps[1], NULL, 0);
	waitpid(ps[2], NULL, 0);
	
	len = sprintf(s, "Main Process Exits\n");
	write(1, s, len);
	return 0;
}
