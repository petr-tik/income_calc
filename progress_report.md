## 22/11

It compiles again! a git hook would be great to check if it can compile in the future. Changed the function definitions and signatures, changed bracket indentation in income_calc.c

Added more comments at the beginning. Added a commented-out tax_shares method (TODO), now need to implement file reading and checking for most recent salary quote/stock price and calculate the post-tax profit from stock selling.


## 17/11

Expanded the download_stock script - now has 2 use cases - 
1. download all stock data for default stocks
2. Append a new stock info to the current file

Refactored procedural code into another function, 
which creates a URL to query yahoo finance.

Added docstrings and general infromation about code

## 14/11

Thinking about communicating between python and C processes with a socket. Might be an overkill, but lighter than messing around and embedding a python interpreter into the C runtime. Also, should refactor the main. The struct is initialised with "" as stock quote and 0 for number of shares, the tax for that is the same everywhere. Rework the ideas from 17/8. 

## 12/11

Researching methods of calling python script from a C program. On install time, a csv of different preset companies stock values will be stored. At run-time, the stock quote from terminal is checked against the file - if already saved, use the price and calculate taxes. If not, I want to call the same python script with the stock quote as argument. In the python script, behaviour will change if it's provided an argument. The methods to run python from C at run-time:
    1. Embed the whole python interpreter
    2. Call a one-liner python script

Scenarios for stock market:
    1. Stock downloaded at install time and saved in .stock.csv
    2. No internet at install time OR the stock quote not in the list
    2.1. Download the stock value and append it to the file
    3. No internet now
    3.1 Ask for estimate of stock price

Now calculate the taxes from selling all shares. 

## 07/11

Been crazy busy, but coming back to it. [Craig](https://github.com/cratuki) suggested using 
a python script for initial stock quote download. 
It's easier to write in a higher level language using libs like requests, 
than open sockets in C. 
For now, will leave it in python, but ideally rewrite in C later, 
so I can learn about sockets etc. 

Wrote the first version of the download script, with a hard-wired list 
of companies (TODO: add a configure method to add a company on install). 


## 05/10

Haven't had time to contribute anyythng, which is a shame. Picking up where I left off. In the future, optimise struct to an array - more efficient for L1 caching, not that it's necessary, but still minor optimisation. 

## 29/8

Idea about debug mode - make a variable that at compile time is set to 0 or 1, which determines if the executable prints all the error debug messages, or does nothing

## 26/8

Reading about efficient encoding and thinking how to change the return codes for different scenarios - 
country X salary only
country X salary + shares

Also, realised I am missing abort methods and messages for badly parsed/incomplete input. Should split main into another function.

## 21-22/8

Split income_calc into separate repo, using git subtree split and git pull into a new repo. Very clean - thanks git. 

Working out the specifics of Makefile. I don't have tests (yet), so no test recipe, but when I do, I should be able to run through the test suite with a simple ```bash make test```


## 17/8

There are 2 decisions to make about calculating income:
      salary only vs salary and stocks
      country of tax residence

Both are represented by int flags and can be combined in one switch statement.

Countries are listed from 0 to 3 (TODO: change to from 1 to 4) and salary vs salary and stock is decided by check_options method, which returns 0 in case of failure, 1 if only salary present, 2 if both salary and stocks. If we multiply the flag ints, we can get different branches for the switch statement

countries:
1 - UK
2 - NYC
3 - Seattle
4 - California

options:
0 - no info
1 - just salary
2 - salary and stock

multiplying makes sense, because 0 as result will trigger an exit and show_help method. We want to avoid overload 

with 4 countries, we have 9 scenarios
+1 - not enough info
+2 - UK - salary and salary and stocks
+2 - NYC - salary and salary and stocks
+2 - Seattle - salary and salary and stocks
+2 - California - salary and salary and stocks

the minimum number of bits we need is number_of_countries*2 + 1. 

one idea is to use prime numbers for country flag, so they don't intersect. so it becomes
1 - UK
2 - NYC
3 - Seattle
5 - California


## 16/8 

Reworked UK\_full to have fewer if statements. If applying tax rules returns not 0, no point working further, return 1. Decision tree for UK\_full is

                            return 0
                          /
              no problem /
                        /
               apply NI           
             /          \
 no problem /      error \
           /              \
apply taxes                 return 1
           \ 
     error  \   
             \    
                return 1


## 13/8

Idea to keep tax_rules structs outside the options struct. Use the location int flag to activate a branch in a ```switch``` statement, which calls the right helper function with all the business logic for that location's tax contributions, including national insurance and other charges. 

Keeps the structs nice and light, makes it easy to extend to new tax residencies by adding:
      relevant tax_rules structs
      helper func for full calculation
      a ```switch``` statement branch

## 11/8

Added the helper function for the parser - it copies the location array of chars and lowercases all upper case chars. 
-l UK, -l Uk, -l uk   - will be treated the same
-l NYc, -l NY, -l ny  - will be treated the same

This will simplify switch statement.

Thinking about struct design. Right now, I am thinking not to put the tax_rules struct inside the options, because it will make it a bloated struct. Instead, keep those structs separate and have helper funcs for each tax rule calculating the full tax contribution. 

Instead of using a bloated struct like below and initialising the substruct ```tax_t```. 
```c
typedef struct options_t {
  float amount; // mandatory
  float stock_amount; // optional 
  int married; // optional 
  char stock_quote[6]; // optional 
  short int location; // mandatory
  tax_t country;
} options_t;
```

Look inside ```options_t->location``` and call the relevant full tax calculator function for that country

Upsides:
* structs remain nice and compact
* no need for substruct nesting


Downsides: 
* Adding new countries will be a lot of manual work defining another if else branch into main, not parser. 
  * This isn't great, cos the idea is once parser is done - no more worrying about the cli args


==============================================

Changelog

Version 0.2 released so far. 

Run and compile the salary_calc.c and it should take your arguments and product basic salary stats for UK taxes and National Insurance

## Design Lessons learnt

* Need a good modular design, where the parser reads the arg string and returns a struct with relevant country, tax, income and data. After this, you shouldn't have to return to the arg string again.
* Design a good data structure that's not too clunky, but has all the necessary info.
* Still working out how to split 
* Future contributors should find it easy to add tax residencies into one file, which will add them to the command line options, but also use relevant tax_struct and rules.


## C internals

* If you are handling a pointer to a struct with an array inside it. You cannot make an assignment to an array stored with a '->' operator, instead you need to use the memcpy function, which requires arguments for: destination, source value, and number of bits of info. 
* Write and keep a good makefile - it's awesome. 
* Strings cannot be compared with a '==' operator. Use strcmp instead - it returns the number of bits different between 2 strings. If it returns 0, you have identical strings. 
* To pass argv to another function, define the function with char \*argv[] and pass it directly.
* It can be useful for a function to return a return code, which will decide what to do next.


## Roadmap
* Rework the data structure for the parser modifies
* Make it work for UK salaries (no personal allowance if salary >150k) with parser
* Write a makefile for OSX
* Test on linux and OSX
* Add stock quote look up module
* Split the folder into a separate repo
