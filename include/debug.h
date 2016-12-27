#ifndef _DEBUG_H
#define _DEBUG_H
/*
   ------------------------------
   Helper functions for debugging
   ------------------------------
*/

#define STR(x) #x
#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

#ifndef DEBUG_LVL
#define DEBUG_LVL 1
#endif

extern int print_salary_stats(options_t *arg_options, float *salary_after_tax)
{
	/* Given a pointer to salary after tax value (annual),
	   print out weekly and monthly allowance */
	char sign[3] = "";
	int location = arg_options->location;
	switch (location) {
	case 1:
		strncpy(sign, "£", 3);
		break;

	case 2 ... 4:
		strncpy(sign, "$", 3);
		break;

	default:
		return 1;
	}
	printf("Salary before taxes: %s%.2f\n", sign, arg_options->amount);
	printf("Salary after tax: %s%.2f\n", sign, *salary_after_tax);
	printf("Your monthly allowance: %s%.2f\n", sign,
	       *salary_after_tax / 12);
	printf("Your weekly allowance: %s%.2f\n", sign, *salary_after_tax / 52);
	return 0;
}

extern void print_options(struct options_t *options)
{
	// ONLY FOR DEBUGGING - REMOVE FROM END PRODUCT
	printf("\n\n\nI am a DEBUGGING function. If you see me, it's in debug "
	       "mode\n");
	printf("Salary is: %f\n", options->amount);
	printf("You have %.1f shares in %s\n", options->stock_amount,
	       options->stock_quote);
	if (options->married == 0) {
		printf("Not married\n");
	} else {
		printf("married\n");
	}
	printf("The location code is: %d\n", options->location);
}

#endif
