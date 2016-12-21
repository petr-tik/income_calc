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
#if (DEBUG_LVL == 2)
		printf("At bracket %f, I pay %.2f\n", tax_rules.TAXRATES[idx],
		       taxes_paid_at_bracket);
#endif
		*taxes_paid += taxes_paid_at_bracket;
		salary = tax_rules.SALARY_LIMITS[idx];
	}
	// TODO - does this need to return at all ? What could go wrong?
	return 0;
}

int pay_taxes_from_salary(options_t *arg_options, float *salary_taxes_paid_ptr)
{
	int err_code = 0;
	int location = arg_options->location;
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

int pay_taxes_from_stock(options_t *arg_options, float *netto_stock_profit_ptr)
{
	/* Given pointers to:
		  struct with cli args
		  float value of after tax profit from stocks
	   Calculate how much money you make, if you liquidate your stock at
	   the current (or stored) price and pay taxes on it in your country
	*/
	if (arg_options->stock_amount == 0) {
		*netto_stock_profit_ptr = 0;
		return 0;
	}

	return 0;
}

/*
   ------------------------------
   Country-specific tax calculator functions
   ------------------------------
*/

int Cali_full(options_t *arg_options, float *salary_taxes_paid_ptr)
{
	/* Helper function that calculates all tax contributions
	   including healthcare charges.
	   The value of all taxes paid will be added to the taxes_paid ptr  */
	int married = arg_options->married;
	return 0;
}

int NYC_full(options_t *arg_options, float *salary_taxes_paid_ptr) { return 0; }

int UK_full(options_t *arg_options, float *salary_taxes_paid_ptr)
{
	/* Given pointers to salary and taxes, applies respective tax rates and
	 * full costs of national insurance and assign it to respective pointers
	 */
	float salary;
	salary = arg_options->amount;
	int err_code;
	tax_t taxes = UK;
	tax_t NI = UK_NI;
	// apply taxes
	err_code = calc_taxes(&salary, salary_taxes_paid_ptr, taxes);
	if (err_code == 0) {
		// apply national insurance
		err_code = calc_taxes(&salary, salary_taxes_paid_ptr, NI);
		if (err_code == 0) {
			return err_code;
		}
	} // if end

	return err_code;
}

int main(int argc, char *argv[])
{
	/* the main initialises the necessary pointers,
	   all functions take at least pointers to salary and taxes_paid.
	   US tax funcs take pointer to married parameter */
	float salary_after_tax;
	float *salary_after_tax_ptr = &salary_after_tax;
	float salary_taxes_paid = 0;
	float *salary_taxes_paid_ptr = &salary_taxes_paid;
	float netto_stock_profit = 0;
	float *netto_stock_profit_ptr = &netto_stock_profit;

	// parser module - create and init an options struct
	options_t *arg_options = options_init();
	if (salary_after_tax_ptr == NULL || salary_taxes_paid_ptr == NULL ||
	    arg_options == NULL) {
		exit(1);
	}
	int err_code = parser(argc, argv, arg_options);
	int options_validity = check_options(arg_options);
	switch (err_code) {
	case -1:
		show_version();
		exit(1);

	case -2:
		show_help();
		exit(1);
	}

	if (err_code != 0 || options_validity == 0) {
		/* If either the parser had difficulty,
		   or the options were checked to be missing information - break
		   now */
		printf("ERROR! Not enough arguments\n");
		exit(1);
	}
// DEBUGGING ONLY
#if (DEBUG_LVL == 2)
	printf("Parsed: \n\tsalary amount: %f\n\tlocation: %d\n\tmarried:\
		   %d\n ",
	       arg_options->amount, arg_options->location,
	       arg_options->married);
#endif
	err_code = pay_taxes_from_salary(arg_options, salary_taxes_paid_ptr);
	if (err_code == 0) {
		*salary_after_tax_ptr =
		    arg_options->amount - *salary_taxes_paid_ptr;
		err_code =
		    pay_taxes_from_stock(arg_options, netto_stock_profit_ptr);
	} else {
		free(arg_options);
		return 1;
	} // end else
#if (DEBUG_LVL == 2)
	printf("%d", DEBUG_LVL);
	print_salary_stats(arg_options, salary_after_tax_ptr);
#elif(DEBUG_LVL == 1)
	printf("%d", DEBUG_LVL);
	printf("%.2f\n", *salary_after_tax_ptr);
#endif
	free(arg_options);
	return 0;
}
