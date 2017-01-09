#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "test_parser.h"

#ifndef ALNUM_TEST_STRING_LEN
#define ALNUM_TEST_STRING_LEN 20
#endif

#ifndef TEST_CASES_ISNUMBER
#define TEST_CASES_ISNUMBER 5
#endif

int test_isnumber_harness(char *arr_of_strings[TEST_CASES_ISNUMBER],
			  int *expected_results[TEST_CASES_ISNUMBER])
{
	/* Given an pointer to an array of string and a pointer to an array of
	 * ints, run isnumber on each of the strings and compare with the
	 * expected results  */

	for (int idx = 0; idx < TEST_CASES_ISNUMBER; idx++) {
		printf("%d \t %s \n", expected_results[idx],
		       arr_of_strings[idx]);
		printf("%d\n", isnumber(arr_of_strings[idx]));
		int res = isnumber(arr_of_strings[idx]);
		if (res != expected_results[idx]) {
			printf("error - %s isn't a numeric string\n",
			       arr_of_strings[idx]);
			return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	char *tests[TEST_CASES_ISNUMBER] = {"2", "2lsds2", "obvs not an alnum",
					    "1210000012", "111111100000OOOOO"};
	int *test_results[TEST_CASES_ISNUMBER] = {1, 0, 0, 1, 0};
	printf("running test_parser suite\n");
	/* for (int idx = 0; idx < TEST_CASES_ISNUMBER; idx++) { */
	/* 	strcpy(arr_of_strings[idx], tests[idx]); */
	/* } */
	int outcome = test_isnumber_harness(tests, test_results);
	if (outcome == 0) {
		printf("tests passed\n");
		exit(EXIT_SUCCESS);
	}
}
