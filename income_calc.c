#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include "income_calc.h"
#include "parser.h"

/*
This file contains the general formulae for calculating progressive tax contributions and printing net income. 

Tax calculator methods take a pointer to salary float value and a struct with tax info. Most countries have progressive taxation, so we the tax_t struct includes salary thresholds and tax rate arrays of equal length such that tax_rate[i] is the rate applied to income earned between salary_limits[i] and salary_limits[i+1]. eg. 
tax_t UK = {.TAXRATES = {0, 20, 40, 45, MAX_PERC}, 
            .SALARY_LIMITS = {0, 11000, 43000, 150000, MAX_FLOAT}
in the UK, you pay 0% of your income in the 0..11,000 GBP bracket.

It also contains country-specific income calculators, which internally process income tax, sales tax, national insurance payments and other country-specific contributions to add to the taxes_paid pointer. 

The main func uses the parser func (which fills in the options struct) to decide, what to do along 2 dimensions:
    salary and/or taxes
    country
*/

int calc_taxes(float *salary_ptr, float *taxes_paid, tax_t tax_rules) {
  /* Universal helper function to calculate amount of tax paid, 
     given the rules and the salary. 
     Can be applied for any contribution: NI in the UK, taxes elsewhere. 
     Takes pointers to: salary, amount of tax paid and the struct of tax rules 
     return 0 if no errors.
     The value on ptr taxes_paid will be incremented 
     by the value of tax you pay at every bracket
  */

  float salary = *salary_ptr;
  int idx = 0;
  float taxes_paid_at_bracket = 0;
  while (salary > tax_rules.SALARY_LIMITS[idx]) 
  // find out the top tax bracket the salary crosses
{
    idx += 1;
}
  idx -= 1; // incremented one too many times
  for(idx; idx > -1; idx--) 
  // starting from the top bracket, 
  // increment the taxes_paid ptr by the amount of tax at this tax bracket
{
    taxes_paid_at_bracket = tax_rules.TAXRATES[idx]/\
                            100*(salary - tax_rules.SALARY_LIMITS[idx]);
    printf("At bracket %f, I pay %.2f\n", tax_rules.TAXRATES[idx], taxes_paid_at_bracket); // debugging
    *taxes_paid += taxes_paid_at_bracket;
    salary = tax_rules.SALARY_LIMITS[idx];
}
  return 0;
}


int print_salary_stats(float *salary_before_tax, float *salary_after_tax, short int location) {
  /* Given a pointer to salary after tax value (annual), 
     print out weekly and monthly allowance */
  char sign[3] = "";
  switch(location)
{
    case 1:
      strncpy(sign, "£", 3);
      break;

    case 2 ... 4:
      strncpy(sign, "$", 3);
      break;

    default:
      return 1;
}      
  printf("Salary before taxes: %s%.2f\n", sign, *salary_before_tax); 
  printf("Salary after tax: %s%.2f\n", sign, *salary_after_tax); 
  printf("Your monthly allowance: %s%.2f\n", sign, *salary_after_tax/12);
  printf("Your weekly allowance: %s%.2f\n", sign, *salary_after_tax/52);
  return 0;
}

int Cali_full(options_t *arg_options, float *taxes_paid) {
  /* Helper function that calculates all tax contributions 
     including healthcare charges. 
     The value of all taxes paid will be added to the taxes_paid ptr  */ 
  int married = arg_options->married;
  return 0;
}

int NYC_full(float *arg_options, float *taxes_paid) {
  return 0;
}

int UK_full(options_t *arg_options, float *taxes_paid) {
  /* Given pointers to salary and taxes, applies respective tax rates and full costs of national insurance and assign it to respective pointers 
*/
  float *salary_ptr = arg_options;
  int errno;
  tax_t taxes = UK;
  tax_t NI = UK_NI;
  // apply taxes
  errno = calc_taxes(salary_ptr, taxes_paid, taxes);
  if (errno == 0) 
{
    // apply national insurance
    errno = calc_taxes(salary_ptr, taxes_paid, NI);
    if (errno == 0) {
      return errno;
} 
} // if end

  return errno;
}

int main(int argc, char *argv[]) {
  /* the main initialises the necessary pointers, 
     all functions take at least pointers to salary and taxes_paid. 
     US tax funcs take pointer to married parameter */
  float salary_after_tax;
  float *salary_after_tax_ptr = &salary_after_tax; 
  float taxes_paid = 0;
  float *taxes_paid_ptr = &taxes_paid;
  
  // parser module - create and init an options struct 
  options_t * arg_options = options_init();
  if (salary_after_tax_ptr == NULL || taxes_paid_ptr == NULL 
      || arg_options == NULL) 
{
  exit(1);
}
  int errno = parser(argc, argv, arg_options);
  int options_validity = check_options(arg_options);
  switch (errno) {
    case -1:
      show_version();
      exit(1);
 
    case -2:
      show_help();
      exit(1);
}

  if (errno != 0 || options_validity == 0)
{
  /* If either the parser had difficulty, 
     or the options were checked to be missing information - break now */
  printf("ERROR! Not enough arguments\n");
  exit(1);
}
  int scenario = (arg_options->location) * options_validity;
  // DEBUGGING ONLY
  printf("scenario: %d\n", scenario);
  printf("Parsed: \n\tsalary amount: %f\n\tlocation: %d\n\tmarried: %d\n", 
             arg_options->amount, arg_options->location, arg_options->married);

  // logic again
  switch(scenario){    
    case 1:
      // first field in struct is pointer to struct
      // the first element of the struct is ptr to salary amount
      errno = UK_full(arg_options, taxes_paid_ptr); 
      break;

    case 2:
      // UK and shares
      break;
 
    case 3:
      // NYC no shares
      errno = NYC_full(arg_options, taxes_paid_ptr);
      break;

    case 5:
      // Seattle_no_shares
      break;

    case 6:
      // NYC and shares
      break;
      
    case 7:
      // California no shares
      break;

    case 10:
      // Seattle and shares
      break;

    case 14:
      // Cali and shares
      break;  

    default:
      // nothing
      break;
}

  if (errno == 0) 
{
    *salary_after_tax_ptr = arg_options->amount - *taxes_paid_ptr;
    print_salary_stats(arg_options, salary_after_tax_ptr, arg_options->location);
} else 
{
    return 1;
} // end else

  free(arg_options);
  return 0;
}



