# Income calculator

Experimental version with basic functionality for UK salaries without shares

## Install 

_Currently only supports unix-like OS'es, sorry no Windows_

### Working executable

Build Requirements:
 - Stdlib C
 - GNU make

Download the repo and run make
```bash
git clone https://github.com/petr-tik/income_calc.git
cd income_calc
make install
```

You should have an executable called ```income_calc```

### Testing artefacts

You can also compile 2 artefacts for testing, which use macros to make executableswith different levels of verbosity.  
 - income\_calc\_test2 - integration testing. Prints a 2 decimal places float value or the error message after calculating the salary income after paying taxes
 - income\_calc\_test3 - super verbose. Prints how much money was paid at every tax bracket and other information for debugging. 

## Try in Docker

Use the Dockerfile in the repo (as of release v0.3) to build and run a Docker image

```bash
make docker_build_and_run_new
```

## Testing: 

https://www.incometaxcalculator.org.uk was used to check my salary calculator against.

Test the python download script
```bash
make test_python
```

Run integration tests
```bash
make test
```

Statically analyse code (.c and .h files) with clang
```bash
make analyze
```

## Usage: 

```bash
./income_calc -a <amount> -l <location> [-m married]
```
         -a - salary offered without currency symbol
         -l - Tax residence: 
             UK, London
             NY, NYC - cannot do New York (separated by a space in terminal)
             Cali, California
             Seattle

**To do**
         
         -s - stock options 

         <amount_of_shares> <stock_quote>


## Contributing:

Thanks a lot for checking this small project out. 
Contributions welcome using GitHub-workflow. 

### Bugs, typos, features, questions

Open issues

### Code submission, pull requests

#### Requirements to contribute
 - Stdlib C
 - GNU make
 - clang-format
 - clang (for static code analysis)

#### Step-by-step guide

Fork on GitHub
```bash
git clone https://github.com/<your_username>/income_calc.git
cd income_calc
git checkout -b new_feature_branch
```
hack, hack, hack

clang-format the code according to [.clang-format](.clang-format)

```bash
git commit -am "meaningful commit message"
git push origin new_feature_branch
```

Go to the branch on your GitHub fork and send a PR to the petr_tik/master
