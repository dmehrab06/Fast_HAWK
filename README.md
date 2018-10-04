# Fast_HAWK

This is a faster implementation of https://github.com/atifrahman/HAWK/blob/master/log_reg_case.R and https://github.com/atifrahman/HAWK/blob/master/log_reg_control.R. This work is done as course project of CSE6411 in April 2018 semester. This code are part of the implementation of method described in this paper https://elifesciences.org/articles/32920.

## Prerequisites

boost library
https://www.boost.org/

## Installation

To build the executable you need to have make tool. You have to edit the Makefile to give the location of boost library include file ($BOOST_HOME) and boost library location ($BOOST_LIB).

```
make
```

## Test

test folder contains some dummy data (See https://github.com/atifrahman/HAWK and paper for details of data) to run the code (both log_reg_case.out and log_reg_control.out)
From base dir

```
cd test
../log_reg_case.out
../log_reg_control.out
```
Each executable dumps the p value in stdout. To store them you can redirect output or edit log_reg_case.cpp/log_reg_control.cpp.

To compare the quality of output with the original R code output you can use r_cpp_output_compare.py by adding the output containing file name (both R and cpp output file). The comparison is shown by evaluating mean square error and std. dev. of all the pvalue and corresponding R output's p values.
