#ifndef _INCOME_CALC_H
#define _INCOME_CALC_H

// length of taxrates and salary_limits array.
#define NUM_TAXES 10
#define MAX_FLOAT 1000000
// TODO - change the definition to real MAX_FLOAT
#define MAX_PERC 100
#define STOCK_FILENAME ".stocks.csv"

typedef struct tax_t {
	float TAXRATES[NUM_TAXES];
	int SALARY_LIMITS[NUM_TAXES];
	char SIGN[3];
} tax_t;

typedef struct salary_n_taxes_t {
	float salary_after_taxes;
	float salary_taxes_paid;
	float stock_profit_after_taxes;
	float stock_taxes_paid;
} salary_n_taxes_t;

tax_t UK = {.TAXRATES = {0, 20, 40, 45, MAX_PERC},
	    .SALARY_LIMITS = {0, 11000, 43000, 150000, MAX_FLOAT},
	    .SIGN = "£"};

tax_t UK_NI = {.TAXRATES = {0, 12, 2, MAX_PERC},
	       .SALARY_LIMITS = {0, 8064, 43000, MAX_FLOAT},
	       .SIGN = ""};

tax_t California = {
    .TAXRATES = {1, 2, 4, 6, 8, 9.3, 10.3, 11.3, 12.3, MAX_PERC},
    .SALARY_LIMITS = {0, 7850, 18610, 29372, 40773, 51350, 263222, 315866,
		      526443, MAX_FLOAT},
    .SIGN = "$"};

tax_t Russia = {.TAXRATES = {13}, .SALARY_LIMITS = {MAX_FLOAT}, .SIGN = "RUB"};

/* tax_t Seattle = {.TAXRATES = {},
			 .SALARY_LIMITS = {},
			 .SIGN = "$"};

tax_t NYC = {.TAXRATES = {},
	     .SALAY_LIMITS = {},
	     .SIGN = {}
};
*/

salary_n_taxes_t *salary_n_taxes_init();

int calc_taxes(float *salary_ptr, float *taxes_paid, tax_t tax_rules);

int UK_full(options_t *arg_options, float *salary_taxes_paid_ptr);

int Cali_full(options_t *arg_options, float *salary_taxes_paid_ptr);

int NYC_full(options_t *arg_options, float *salary_taxes_paid_ptr);

int pay_taxes_from_salary(options_t *arg_options,
			  salary_n_taxes_t *salary_n_taxes_ptr);

int pay_taxes_from_stock(options_t *arg_options,
			 salary_n_taxes_t *salary_n_taxes_ptr);

#endif
