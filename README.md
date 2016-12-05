# Income calculator - still not ready, build and use at your own risk

## Install 

Requirements:
 - Stdlib C
 - GNU make

Download the repo and run make. _Currently only supports unix-like OS'es, sorry no Windows_
```bash
git clone https://github.com/petr-tik/income_calc.git
cd income_calc
make
```

You should have an executable called ```income_calc```

## Testing: 
Test the python download script
```bash
make test_python
```

Run integration tests
```bash
make test
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

#### Requirements
 - Stdlib C
 - GNU make
 - clang-format

#### Step-by-step guide

Fork on GitHub
```bash
git clone https://github.com/<your_username>/income_calc.git
cd income_calc
git checkout -b new_feature_branch
```
hack, hack, hack

```bash
git commit -am "meaningful commit message"
git push origin new_feature_branch
```

Go to the branch on your GitHub fork and send a PR to the master branch 
