#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include "income_calc.h"
#include "parser.h"

/*
This file contains the general formulae for calculating progressive tax contributions and printing net income. 

Tax calculator methods take a pointer to salary float value and a struct with tax info. Most countries have progressive taxation, so we the tax_t struct includes salary thresholds and tax rate arrays of equal length such that tax_rate[i] is the rate applied to income earned between salary_limits[i] and salary_limits[i+1]. 

It also contains country-specific income calculators, which internally process income tax, sales tax, national insurance payments and other country-specific contributions to add to the taxes_paid pointer. 

The main func uses the parser func (which fills in the options struct) to decide, what to do along 2 dimensions:
    salary and/or taxes
    country

*/

int calc_taxes(float *salary_ptr, float *taxes_paid, tax_t tax_rules) {
  /* Helper function to calculate amount of tax paid, given the rules and the salary. Can be applied for any contribution: NI in the UK, taxes elsewhere. 
     Given pointers to: salary, amount of tax paid and the struct of tax rules 
     return 0 if no errors.
     The value under taxes_paid will be incremented 
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
  // starting from the top bracket, increment the taxes_paid ptr by the amount of tax at the appropriate taxrate
{
    taxes_paid_at_bracket = tax_rules.TAXRATES[idx]/\
                            100*(salary - tax_rules.SALARY_LIMITS[idx]);
    *taxes_paid += taxes_paid_at_bracket;
    salary = tax_rules.SALARY_LIMITS[idx];
}
  return 0;
}


int print_salary_stats(float *salary_after_tax, short int location) {
  /* Given a pointer to salary after tax value (annual), 
     print out weekly and monthly allowance */
  char sign[3] = "";
  switch(location)
{
    case 1:
      strncpy(sign, "£", 3);
    case 2 ... 4:
      strncpy(sign, "$", 3);
}      
    printf("Salary after tax: %s%.2f\n", sign, *salary_after_tax); 
    printf("Your monthly allowance: %s%.2f\n", sign, *salary_after_tax/12);
    printf("Your weekly allowance: %s%.2f\n", sign, *salary_after_tax/52);
  return 0;
}

int Cali_full(float *salary_ptr, float *taxes_paid, int *married) {
  /* Helper function that internally calculates all tax contributions including healthcare charges. The value of all taxes paid will be added to the taxes_paid ptr  */ 
  return 0;
}

int NYC_full(float *salary_ptr, float *taxes_paid, int *married) {
  return 0;
}

int UK_full(float *salary_ptr, float *taxes_paid) {
  /* Given pointers to salary and taxes, applies respective tax rates and full costs of national insurance and assign it to respective pointers 
*/
  int errno;
  int ret = 0;
  tax_t taxes = UK;
  tax_t NI = UK_NI;
  // apply taxes
  errno = calc_taxes(salary_ptr, taxes_paid, taxes);
  if (errno == 0) 
{
    // apply national insurance
    errno = calc_taxes(salary_ptr, taxes_paid, NI);
    if (errno == 0) {
      return ret;
} 
} // if end
  ret = 1;
  return ret;
}

int main(int argc, char *argv[]) {
  /* the main initialises the necessary pointers, all functions take at least pointers to salary and taxes_paid. US tax funcs take pointer to married parameter */
  float salary, salary_after_tax;
  float *salary_after_tax_ptr = &salary_after_tax;
  float *salary_ptr = &salary;  
  float taxes_paid = 0;
  float *ptr_taxes_paid = &taxes_paid;
  int errno;
  
  
  // parser module - create and init an options struct 
  options_t * arg_options = options_init();
  parser(argc, argv, arg_options);
  int scenario = (arg_options->location) * check_options(arg_options);

  switch(scenario){
    case 1:
      errno = UK_full(salary_ptr, ptr_taxes_paid);
      break;

    case 2:
      break;
 
    case 3:
      break;

    case 6:
      break;

    default:
      printf("activated default - value of options->location = %d\n", arg_options->location);
      break;
}

  if (errno == 0) 
{
    *salary_after_tax_ptr = *salary_ptr - *ptr_taxes_paid;
    print_salary_stats(salary_after_tax_ptr, arg_options->location);
} else 
{
    return 1;
} // end else

  free(arg_options);
  return 0;
}



