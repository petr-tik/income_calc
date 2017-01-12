#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ALNUM_TEST_STRING_LEN
#define ALNUM_TEST_STRING_LEN 20
#endif

#ifndef TEST_CASES_ISNUMBER
#define TEST_CASES_ISNUMBER 5
#endif

int test_isnumber_harness()
{
	/* Given an pointer to an array of string and a pointer to an array of
	 * ints, run isnumber on each of the strings and compare with the
	 * expected results  */
	printf("Testing isnumber method\n");
	char *tests[TEST_CASES_ISNUMBER] = {"2", "2lsds2", "obvs not an alnum",
					    "1210000012", "111111100000OOOOO"};
	int expected_results[TEST_CASES_ISNUMBER] = {1, 0, 0, 1, 0};
	for (int idx = 0; idx < TEST_CASES_ISNUMBER; idx++) {
		int res = isnumber(tests[idx]);
		if (res != expected_results[idx]) {
			printf("error - %s isn't a numeric string\n",
			       tests[idx]);
			return 1;
		}
	}
	return 0;
}

int test_parse_location()
{
	printf("Testing parse_location method\n");
	char *london_locations[5] = {"UK", "uk", "Uk", "London", "london"};
	for (int idx = 0; idx < 5; idx++) {
		short int res = parse_location(london_locations[idx]);
		if (res != 1) {
			printf("ERROR. %s gives wrong location code: %d\n",
			       london_locations[idx], res);
			return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	printf("running test_parser suite\n");
	int outcome = 0;
	outcome |= test_isnumber_harness();
	outcome |= test_parse_location();
	if (outcome == 0) {
		printf("All tests passed\n");
		exit(EXIT_SUCCESS);
	} else {
		printf("Tests failed\n");
		exit(EXIT_FAILURE);
	}
}
