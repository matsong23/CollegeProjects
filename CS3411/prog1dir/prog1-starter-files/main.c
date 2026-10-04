#include "hmalloc.h"
#include <stdlib.h>
#include <stdio.h>
/*You may include any other relevant headers here.*/


/*	main()
 *	Use this function to develop tests for hmalloc. You should not 
 *	implement any of the hmalloc functionality here. That should be
 *	done in hmalloc.c
 *	This file will not be graded. When grading I will replace main 
 *	with my own implementation for testing.*/
int main(){
	printf("Testing basic hmalloc\n");
	int *num = (int*)hmalloc(sizeof(int));
	*num = 10;
	printf("num: %d\n", *num);
	hfree(num);	

	char *string = (char *)hmalloc(27 * sizeof(char));
	char *up = (char*) hmalloc(28 * sizeof(char));

	if (string == NULL || up == NULL) {
		printf("Memory allocation failed\n");
		return 0;
	}

	for (int i = 0; i < 26; i++) {
		string[i] = 'a' + i;
		up[i] = 'A' + i;
	}
	string[26] = '\0';
	up[26] = '!';
	up[27] = '\0';
	
	printf("Lowercase: %s\n", string);
	printf("Uppercase: %s\n", up);
	printf("running traverse should be empty\n");
	traverse();
	hfree(string);
	printf("running traverse should have 1 node of len %lu\n", (27 * sizeof(char)));
	traverse();
	hfree(up);
	printf("traverse n1 len %lu, n2 len %lu\n", (28 * sizeof(char)), (27 * sizeof(char)));
	traverse();

	printf("Testing hmalloc for getting freed memory\n");
	char *allZ = (char *)hmalloc(27 * sizeof(char));

	if (allZ == NULL) {
		printf("Mem allocation failed\n");
		return 0;
	}
	
	for (int i = 0; i < 26; i++) {
		allZ[i] = 'z';
	}
	allZ[26] = '\0';
	printf("allZ: %s\n", allZ);
	printf("traverse n1 len %lu\n", (27 * sizeof(char)));
	traverse();
	hfree(allZ);
	printf("traverse n1 len %lu, n2 len %lu\n", (28 * sizeof(char)), (27 * sizeof(char)));
	traverse();
	
	printf("testing hcalloc\n");
	char *s = (char*)hcalloc(8 * sizeof(char));
	printf("Mem given: ");	
	for (int i = 0; i < 8; i++) {
		printf("%c", s[i]);
		if (s[i] == '\0') {
			s[i] = '1';
		} else {
			s[i] = '0';
		}
	}
	s[7] = '\0';	
	
	printf("\n");
	printf("dat check stirng: %s\n", s);
	printf("traverse n1 len %lu\n", (27 * sizeof(char)));
	traverse();
	hfree(s);
	printf("traverse n1 len %lu, n2 len %lu\n", (28 * sizeof(char)), (27 * sizeof(char)));
	traverse();
	return 1;
}
