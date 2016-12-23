./income_calc_test2 -a 25000 -l UK
./income_calc_test2 -a 35000 -l London -m
./income_calc_test2 -m -l london -a 40000
./income_calc_test2 -l uk -m -a 50000
./income_calc_test2 -l Uk -a 160000 -m 
# wrong examples
./income_calc_test2 -l sd -a sdf
./income_calc_test2 -l nyc -m
./income_calc_test2 -a 25000
./income_calc_test2 -a 150000 -m
