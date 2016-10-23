#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/* Module responsible for parsing the command line arguments.
   Receives argc and argv and iterates over them to fill in a previously initialised 

Given a string of chars from the command line, return a struct with flags filled in 
allowed options:
   -a <amount>
   -l <location> [uk | NYC | russia | cali(fornia) | sea(ttle)

Optional:
   -s <amount_of_stock> <stock_quote> 
   -m married (default 0)

*/ 

void show_help() {
  printf("Usage:\n income_calc [--version] [--help]\n [-a <amount> -l <location>]\n\nOptional:\n [-s <amount_of_stock> <stock_quote>]\n [-m married] (default 0)\n\n");
}

void show_version() {
  printf("Income calculator version %.1f\n", VERSION);
}

options_t * options_init() {
  /* Initialise an options_t struct with default values */
  options_t *options = malloc(sizeof(struct options_t));
  options->amount = -1;
  options->married = 0; // won't change unless -m option
  options->location = -1;
  options->stock_amount = -1;
  memcpy(options->stock_quote, "", sizeof(""));
  return options;
}


void print_options(struct options_t *options){
  // ONLY FOR DEBUGGING - REMOVE FROM END PRODUCT
  printf("\n\n\nI am a DEBUGGING function. If you see me, it's in debug mode\n");
  printf("Salary is: %f\n", options->amount);
  printf("You have %.1f shares in %s\n", options->stock_amount, options->stock_quote); 
  if (options->married == 0) {
    printf("Not married\n");
} else {
    printf("married\n"); }
  printf("The location code is: %d\n", options->location);
}

short int parse_location(const char *loc_arg) {
  /* Given a pointer to char array i.e. command line location argument, returns the location code, which is assigned to the options->location variable.

Use prime numbers as return values, so when multiplied with check_options status, the result cannot coincide with other countries.
1 - UK
2 - NYC
3 - Seattle
5 - California
*/
  char dest[10];
  strcpy(dest, loc_arg); 
  for(int idx = 0; idx < strlen(dest); idx++) 
{
    dest[idx] = tolower(dest[idx]); 
} // end for
  if ((strcmp(dest, "uk") == 0) || (strcmp(dest, "london") == 0))
{
  return 1;
} 
  else if ((strcmp(dest, "ny") == 0) ||
           (strcmp(dest, "nyc") == 0))
{
  return 3;
} 
  else if (strcmp(dest, "seattle") == 0)
{
  return 5;
}
  else if ((strcmp(dest, "california") == 0) || (strcmp(dest, "cali") == 0))
{
  return 7;
}
  else 
{
  return -1;
}
}


int parser(int argc, char *argv[], struct options_t *options) {
/* Given pointers: to arg string and to options struct, modiy the struct at pointer and return info code
   if succesfully parsed, returns 0
   else, returns 1
 */
  if (argc == 2 && ((strcmp(argv[1], "--version") == 0) || 
                    (strcmp(argv[1], "-v")) == 0)) 
{
      show_version();
      return 1;
}

  for (int idx = 1; idx < argc; idx++) {
    // debugging only
    // printf("Arg %d/%d: %s\n", idx, argc - 1, argv[idx]);
    if (strcmp(argv[idx], "-m") == 0) // set the marriage flag to 1
{
      options->married = 1; 
} // end marriage - how very sad :-)
    else if (strcmp(argv[idx], "-a") == 0) // salary amount
{
      if (argc >= idx+1) 
{
        options->amount = atof(argv[idx+1]); 
}
      else 
{
        return 1;
}
} // end salary amount
    else if (strcmp(argv[idx], "-l") == 0) // location
{
      if (argc >= idx+1) 
{
        options->location = parse_location(argv[idx+1]);
}
      else 
{
        return 1;
}
} // end location
    else if (strcmp(argv[idx], "-s") == 0) // stock options
{
      if (argc >= idx+2) {
        options->stock_amount = atof(argv[idx+1]); // amount 
        memcpy(options->stock_quote, argv[idx+2], sizeof(argv[idx+2])); // quote
}
      else 
{
        return 1;
}
} // end stock options
}  // for end 
  return 0;
} // parser end

int check_options(options_t *options) {
  /* Used after parser. Takes a pointer to options and checks how many option flags have been filled in and returns an action code:
   0 - not even the obligatory info - location and amount
   1 - only basic info
   2 - full info including stocks

   The return code and location flag will be used to call the right tax method 
*/
  int ret = 0;
  if ((options->amount != -1) && (options->location != -1))
{
  ret += 1;
}
  if (options->stock_amount != -1 && strcmp("", options->stock_quote) != 0)
{
  ret += 1;
}
  return ret;
}

