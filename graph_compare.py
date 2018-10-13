import matplotlib.pyplot as plt 
import csv
import math

#change name of the files accordingly
cpp_file = 'pvalues_control_00100_0025_10000_cpp.txt'
r_file = 'pvals_control_top.txt'

yc = []
yr = []

with open(cpp_file,'r') as csvfile:
	plots = csv.reader(csvfile)
	for row in plots:
		yc.append(-1.0*math.log(float(row[0])))

with open(r_file,'r') as csvfile2:
	plots = csv.reader(csvfile2)
	for row in plots:
		yr.append(-1.0*math.log(float(row[0])))

plt.plot(yr,yc,'ro')
plt.xlabel('-log(pvalue_control_R)')
plt.ylabel('-log(pvalue_control_cpp)')
plt.savefig('pvalue_control.png')
