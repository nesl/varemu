
#g++ -Wall -g -o ../bin/autocorr ../CPP/autocorr.cpp
#g++ -Wall -g -o ../bin/compare ../CPP/compare.cpp

# Repeat all 3 steps below for all instances of faulty multipliers and 
# inputs in ../DataSets. So if there are M faulty multiplier instances # and I inputs, total M*I executions are required.

# Step 1: Run the application
../bin/autocorr ../DataSets/input1.txt ../AppOut/out1.txt

# ../bin/autocorr should also output a text file containing list of 
# all the multiply operations (operands and output) that were executed
# on the faulty multiplier.

# Step 2: Compare the output of the application with the golden output
../bin/compare ../GoldenOut/gout1.txt ../AppOut/out1.txt > ../Results/res1.txt

# Step 3: Count  the number of incorrect multiplies
# Need a script to compute the number of incorrect multiply operations
