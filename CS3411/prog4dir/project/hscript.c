#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

// globals for tracting child process
static int exited = 0;
static int cStat = 0;

// helper for child status updating
static void helper() {
	int temp;
	pid_t id = waitpid(-1, &temp, WNOHANG);
	if (id > 0) {
		exited = 1; // child exited
		if (WIFEXITED(temp)) cStat = WEXITSTATUS(temp);
		else cStat = 127; // exit value
	}
}

int main(int argc, char *argv[]) {
	char buf[256];
	int len;
	
	// verify enough arguments for program
	if (argc < 3) {
		len = sprintf(buf, "Not enough arguments provided\n");
		write(2, buf, len);
		return 1;
	}

	// Check for dir
	struct stat t;
	if (stat(argv[argc - 1], &t) == 0 && S_ISDIR(t.st_mode)) {
		// Dir already made
	} else {
		// Make dir
		if (mkdir(argv[argc - 1], (S_IRWXU | S_IRWXG | S_IRWXO))) {
			len = sprintf(buf, "Error creating directory\n");
			write(2, buf, len);
		}
	}

	// create files
	char fName[256];
	sprintf(fName, "%s/0", argv[argc - 1]);
	int f0 = open(fName, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	sprintf(fName, "%s/1", argv[argc - 1]);
	int f1 = open(fName, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	sprintf(fName, "%s/2", argv[argc - 1]);
	int f2 = open(fName, O_WRONLY | O_TRUNC | O_CREAT, 0644);

	// Check for file creation failures
	if (f0 == -1) {
		len = sprintf(buf, "Error creating file 0\n");
		write(2, buf, len);
		return 0;
	}
	if (f1 == -1) {
		len = sprintf(buf, "Error creating file 0\n");
		write(2, buf, len);
		return 0;
	}
	if (f2 == -1) {
		len = sprintf(buf, "Error creating file 0\n");
		write(2, buf, len);
		return 0;
	}	

	// Creat pipes and check for errors
	int p0[2], p1[2], p2[2];
	if (pipe(p0) == -1) {
		len = sprintf(buf, "Error creating pipe for stdin\n");
		write(2, buf, len);
		return 0;
	}
	if (pipe(p1) == -1) {
		len = sprintf(buf, "Error creating pipe for stdout\n");        
		write(2, buf, len);
		return 0;
	}
	if (pipe(p2) == -1) {
		len = sprintf(buf, "Error creating pipe for stderr\n");
		write(2, buf, len);
		return 0;
	}

	// Set up signal connect for child and parent
	struct sigaction sigA = {0};
	sigA.sa_handler = helper;
	sigemptyset(&sigA.sa_mask);
	sigA.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigaction(SIGCHLD, &sigA, NULL);

	// Create the child
	int id = fork();
	if (id == -1) { // verify child created
		close(f0);
		close(f1);
		close(f2);
		len = sprintf(buf, "Error creating child process\n");
		write(2, buf, len);
		return 0;
	}
	
	// Child process
	if (id == 0 ) {
		// Close unused
		close(p0[1]);
		close(p1[0]);
		close(p2[0]);
		close(f0);
		close(f1);
		close(f2);

		// Redirections for in out err
		dup2(p0[0], 0);
		close(p0[0]);
		dup2(p1[1], 1);
		close(p1[1]);
		dup2(p2[1], 2);
		close(p2[1]);

		// Build and execute command
		char *cmd[argc - 1];
		for (int i = 0; i < argc - 2; i++) {
			cmd[i] = argv[i + 1];
		}
		cmd[argc - 2] = NULL;
		execvp(argv[1], cmd);
		len = sprintf(buf, "Failed to run command\n");
		write(2, buf, len);
		_exit(127);
	}

	// close unused
	close(p0[0]);
	close(p1[1]);
	close(p2[1]);
	
	// temps
	int p0w = p0[1];
	int p1r = p1[0];
	int p2r = p2[0];
	char r[1024]; // read var
	fd_set fset; // fd set for read files
	int nfds = 1;
	if (p1r > p2r) nfds += p1r;
	else nfds += p2r;
	
	// Write loop
	while((p0w != -1 || p1r != -1 || p2r != -1) && !exited) {
		// Set up for select
		FD_ZERO(&fset);
		FD_SET(0, &fset);
		FD_SET(p1r, &fset);
		FD_SET(p2r, &fset);
		struct timeval time = {5, 0}; // 5 seconds
		int readyFiles = select(nfds, &fset, NULL, NULL, &time);
		if (readyFiles == -1) break;
		if (readyFiles == 0) continue;

		// write to STDIN
		if (FD_ISSET(0, &fset)) {
			int amt = read(0, r, sizeof(r));
			if (amt <= 0) { // error or done reading
				close(p0w);
				p0w = -1;
			} else {
				write(p0w, r, amt);
				write(f0, r, amt);
			}
		}

		// write to STDOUT
		if (FD_ISSET(p1r, &fset)) {
			int amt = read(p1r, r, sizeof(r));
			if (amt <= 0) { // error or done reading
				close(p1r);
				p1r = -1;
			} else {
				write(1, r, amt);
				write(f1, r, amt);
			}
		}

		// write to STDERR
		if (FD_ISSET(p2r, &fset)) {
			int amt = read(p2r, r, sizeof(r));
			if (amt <= 0) { // error reading or done
				close(p2r);
				p2r = -1;
			} else {
				write(2, r, amt);
				write(f2, r, amt);
			}
		}
	}


	// End of program clean up	
	if (p0w != -1) close(p0w);
	if (p1r != -1) close(p1r);
	if (p2r != -1) close(p2r);
	close(f0);
	close(f1);
	close(f2);
	return cStat;
}
