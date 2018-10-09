import time
import random

cpp_output = []
r_output = []

mse = 0
se_stddev = 0

R_OUTPUT_FILE = "pvals_control_top.txt"
CPP_OUTPUT_FILE = "pvalues_control_00100_0025_10000_cpp.txt"

with open(R_OUTPUT_FILE,"r") as f, open(CPP_OUTPUT_FILE,"r") as f1:
	for line,line1 in zip(f.readlines(),f1.readlines()):
		cpp_output.append(float(line1))
		r_output.append(float(line))

print("has read R output, has %d entry"%(len(r_output)))
print("has read cpp output, has %d entry"%(len(cpp_output)))


for i in range(len(r_output)):
	mse += (cpp_output[i]-r_output[i])*(cpp_output[i]-r_output[i])
mse /= len(r_output)

for i in range(len(r_output)):
	se = (cpp_output[i]-r_output[i])*(cpp_output[i]-r_output[i])
	se_stddev += (se-mse)*(se-mse)
se_stddev /= len(r_output)

print("cpp output's mean square error from R  = %lf\nstd. dev of mse = %lf\n\n"%(mse,se_stddev))

random.seed(time.time())
print("\n\n10 random same line from R and cpp")
for i in range(10):
	r = random.randrange(len(cpp_output))
	print("\\row {0} & {1} & {2}\\\\".format(r,cpp_output[r],r_output[r]))
