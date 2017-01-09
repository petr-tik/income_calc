#ifndef _PARSER_H
#define _PARSER_H

#ifndef TEST_CASES_ISNUMBER
#define TEST_CASES_ISNUMBER 5
#endif

int test_isnumber(char *string);

void test_isnumber_harness(char *arr_of_strings[TEST_CASES_ISNUMBER],
			   int *expected_results[TEST_CASES_ISNUMBER]);

#endif
