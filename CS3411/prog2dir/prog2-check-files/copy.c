#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	char str[256];
	int length = 0;

	// Verify all the arguements are given for the program
	if (argc != 4) {
		length = sprintf(str, "Please follow the following format: copy <infile> <outfile> <blocksize>\n");
		write(2, str, length);
		return 0;
	}
	
	char* rFileName = argv[1];
	char* wFileName = argv[2];
	int blockSize = atoi(argv[3]);
	
	// Get blocksize
	if (blockSize <= 0) {
		blockSize = 4;
	}
	if (blockSize % 4 != 0) {
		blockSize += 4 - (blockSize % 4);
	}
	
	// Open files
	int fr = open(rFileName, O_RDONLY);
	if (fr == -1) {
		length = sprintf(str, "Invalid infile file.\n");
		write(2, str, length);
		return 0;
	}
	int fw = open(wFileName, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fw == -1) {
		close(fr);
		length = sprintf(str, "Error opening outfile file.\n");
		write(2, str, length);
		return 0;
	}

	// copy file and calculate checksome
	char block[blockSize];
	unsigned int checksome = 0;
	int byteCount;
	while((byteCount = read(fr, block, blockSize)) > 0) {
		if (write(fw, block, byteCount) != byteCount) {
			length = sprintf(str, "\nError writing to the outfile.\n");
			write(2, str, length);
			close(fr);
			close(fw);
		}

		// Calculate checksome
		checksome = 0;
		// Pad block
		if (byteCount != blockSize) {
			for (int i = 0; i < blockSize; i++) {
				if (i >= byteCount) {
					block[i] = 0;
				}
			}
		}
		for (int i = 0; i < blockSize; i++) {
			checksome ^= block[i];
		}
		length = sprintf(str, "%08x ", checksome);
		write(1, str, length);
	}
	length = sprintf(str, "\n");
	write(1, str, length);

	// Close files
	close(fr);
	close(fw);
	return 1;
}
