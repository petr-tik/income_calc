# Income calculator - still not ready, build and use at your own risk

### Install 

Requirements:
 - Stdlib C
 - Cmake

Download the repo and run make. _Currently only supports unix-like OS'es, sorry no Windows_
```bash
git clone https://github.com/petr-tik/income_calc.git
cd income_calc
make
```

You should have an executable called ```income_calc```

### Testing: 
Run integration tests
```bash
make test
```

### Usage: 

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

