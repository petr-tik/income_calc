#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/income_calc.h"
#include "../include/debug.h"

/*
  This file contains the general formulae for calculating progressive tax
  contributions and printing net income.

  Tax calculator methods take a pointer to salary float value and a struct with
  tax info. Most countries have progressive taxation, so we the tax_t struct
  includes salary thresholds and tax rate arrays of equal length such that
  tax_rate[i] is the rate applied to income earned between salary_limits[i] and
  salary_limits[i+1]. eg.
  tax_t UK = {.TAXRATES = {0, 20, 40, 45, MAX_PERC},
  .SALARY_LIMITS = {0, 11000, 43000, 150000, MAX_FLOAT}
  in the UK, you pay 0% of your income in the 0..11,000 GBP bracket.

  It also contains country-specific income calculators, which internally process
  income tax, sales tax, national insurance payments and other country-specific
  contributions to add to the taxes_paid pointer.

  The main func uses the parser func (which fills in the options struct) to
  decide, what to do along 2 dimensions:
  salary and/or taxes
  country
*/

salary_n_taxes_t *salary_n_taxes_init()
{
	/* Mallocs and returns pointer to struct, which will carry information
	 * about taxes paid from stock and salary and how much money you have
	 * afterwards */
	salary_n_taxes_t *salary_n_taxes_ptr =
	    malloc(sizeof(struct salary_n_taxes_t));
	if (salary_n_taxes_ptr == NULL) {
		exit(1);
	}
	salary_n_taxes_ptr->salary_taxes_paid = 0;
	salary_n_taxes_ptr->salary_after_taxes = 0;
	salary_n_taxes_ptr->stock_taxes_paid = 0;
	salary_n_taxes_ptr->stock_profit_after_taxes = 0;
	return salary_n_taxes_ptr;
}

/*
   ------------------------------
   ------------------------------
   Reusable functions to calculate taxes from stock and salary
   ------------------------------
*/

/*
  Universal helper function to calculate amount of tax paid, given
  the rules and the salary.  Can be applied for any contribution: NI
  in the UK, taxes elsewhere.  Takes pointers to: salary, amount of
  tax paid and the struct of tax rules return 0 if no errors.  The
  value on ptr taxes_paid will be incremented by the value of tax
  you pay at every bracket
*/
int calc_taxes(float *salary_ptr, float *taxes_paid, tax_t tax_rules)
{
	float salary = *salary_ptr;
	int idx = 0;
	float taxes_paid_at_bracket = 0;
	for (; salary > tax_rules.SALARY_LIMITS[idx + 1]; ++idx) {
	}; // clang-format error.
	   // If I close the for loop with a semicolon, it will be indented on a
	   // newline

	// starting from the top bracket, increment the taxes_paid ptr
	// by
	// the amount of tax at this tax bracket
	for (; idx > -1; idx--) {
		taxes_paid_at_bracket = tax_rules.TAXRATES[idx] / 100 *
					(salary - tax_rules.SALARY_LIMITS[idx]);
#if (DEBUG_LVL == 3)
		printf("At bracket %.0f I pay %.2f\n", tax_rules.TAXRATES[idx],
		       taxes_paid_at_bracket);
#endif
		*taxes_paid += taxes_paid_at_bracket;
		salary = tax_rules.SALARY_LIMITS[idx];
	}
	// TODO - does this need to return at all ? What could go wrong?
	return 0;
}

int pay_taxes_from_salary(options_t *arg_options,
			  salary_n_taxes_t *salary_n_taxes_ptr)
{
	int err_code = 0;
	int location = arg_options->location;
	float *salary_taxes_paid_ptr = &(salary_n_taxes_ptr->salary_taxes_paid);
	switch (location) {
	case 1:
		// first field in struct is pointer to struct
		// the first element of the struct is ptr to salary amount
		err_code = UK_full(arg_options, salary_taxes_paid_ptr);
		break;

	case 2:
		// NYC
		err_code = NYC_full(arg_options, salary_taxes_paid_ptr);
		break;

	case 3:
		// Seattle
		break;

	case 4:
		// California
		break;

	default:
		err_code = 1;
		break;
	}
	return err_code;
}

int pay_taxes_from_stock(options_t *arg_options,
			 salary_n_taxes_t *salary_n_taxes_ptr)
{
	/* Given pointers to:
		  struct with cli args
		  float value of after tax profit from stocks
	   Calculate how much money you make, if you liquidate your stock at
	   the current (or stored) price and pay taxes on it in your country
	*/
	float *stock_taxes_paid = &(salary_n_taxes_ptr->stock_taxes_paid);
	float *stock_profit_after_taxes =
	    &(salary_n_taxes_ptr->stock_profit_after_taxes);
	if (arg_options->stock_amount == 0) {
		*stock_profit_after_taxes = 0;
		return 0;
	}

	return 0;
}

/*
   ------------------------------
   Country-specific tax calculator functions
   ------------------------------
*/

int Cali_full(options_t *arg_options_ptr, float *salary_taxes_paid_ptr)
{
	/* Helper function that calculates all tax contributions
	   including healthcare charges.
	   The value of all taxes paid will be added to the taxes_paid ptr  */
	int married = arg_options_ptr->married;
	return 0;
}

int NYC_full(options_t *arg_options_ptr, float *salary_taxes_paid_ptr)
{
	return 0;
}

int UK_full(options_t *arg_options_ptr, float *salary_taxes_paid_ptr)
{
	/* Given pointers to salary and taxes, applies respective tax rates and
	 * full costs of national insurance and assign it to respective
	 pointers.
	 Returns errcode and increments salary_taxes_paid_ptr value by the total
	 amount of taxes paid
	 */
	float salary = arg_options_ptr->amount;
	int err_code;
	tax_t taxes = UK;
#if (DEBUG_LVL == 3)
	for (int idx = 0; idx < 10; idx++) {
		printf("%i, %f\n", taxes.SALARY_LIMITS[idx],
		       taxes.TAXRATES[idx]);
	}
#endif
	if (salary > 100000) {
		// https://www.gov.uk/income-tax-rates/income-over-100000
		int new_personal_allowance;
		int old_personal_allowance = taxes.SALARY_LIMITS[1];
		new_personal_allowance = (salary - 100000) / 2;
		UK.SALARY_LIMITS[1] =
		    old_personal_allowance - new_personal_allowance;
	}
	tax_t NI = UK_NI;
	// apply taxes
	err_code = calc_taxes(&salary, salary_taxes_paid_ptr, taxes);
	if (err_code == 0) {
		// apply national insurance
		err_code = calc_taxes(&salary, salary_taxes_paid_ptr, NI);
	} // if end

	return err_code;
}

int main(int argc, char *argv[])
{
	/* the main initialises the necessary pointers,
	   all functions take at least pointers to salary and taxes_paid.
	   US tax funcs take pointer to married parameter */
	salary_n_taxes_t *salary_n_taxes_ptr = salary_n_taxes_init();
	// parser module - create and init an options struct
	options_t *arg_options_ptr = options_init();
	if (salary_n_taxes_ptr == NULL || arg_options_ptr == NULL) {
		free(salary_n_taxes_ptr);
		free(arg_options_ptr);
		exit(1);
	}
	int err_code = parser(argc, argv, arg_options_ptr);
	int options_validity = check_options(arg_options_ptr);
	switch (err_code) {
	case -1:
		show_version();
		free(salary_n_taxes_ptr);
		free(arg_options_ptr);
		exit(1);

	case -2:
		show_help();
		free(salary_n_taxes_ptr);
		free(arg_options_ptr);
		exit(1);
	}

	if (err_code != 0 || options_validity == 0) {
		/* If either the parser had difficulty,
		   or the options were checked to be missing information - break
		   now */
		printf("ERROR! Invalid arguments\n");
		show_help();
		free(salary_n_taxes_ptr);
		free(arg_options_ptr);
		exit(1);
	}
// DEBUGGING ONLY
#if (DEBUG_LVL == 3)
	printf("Parsed: \n\tsalary amount: %f\n\tlocation: %d\n\tmarried: %d\n",
	       arg_options_ptr->amount, arg_options_ptr->location,
	       arg_options_ptr->married);
#endif
	err_code = pay_taxes_from_salary(arg_options_ptr, salary_n_taxes_ptr);
	if (err_code == 0) {
		salary_n_taxes_ptr->salary_after_taxes =
		    arg_options_ptr->amount -
		    salary_n_taxes_ptr->salary_taxes_paid;
		err_code =
		    pay_taxes_from_stock(arg_options_ptr, salary_n_taxes_ptr);
	} else {
		free(salary_n_taxes_ptr);
		free(arg_options_ptr);
		return 1;
	} // end else

#if (DEBUG_LVL == 1)
	if (err_code == 0) { // after paying taxes from stock
		print_salary_stats(arg_options_ptr, salary_n_taxes_ptr);
	}
#elif(DEBUG_LVL == 2)
	printf("%.2f\n", salary_n_taxes_ptr->salary_after_taxes);
#elif(DEBUG_LVL == 3)
	print_salary_stats(arg_options_ptr, salary_n_taxes_ptr);
#endif
	free(salary_n_taxes_ptr);
	free(arg_options_ptr);
	return 0;
}
