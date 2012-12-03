all:  
	echo "Invoking: GCC C++ Compiler"
	g++ -Iboost_1_47_0 -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"src/operations.d" -MT"src/operations.d" -o "src/operations.o" "src/operations.cpp"	
	g++ -Iboost_1_47_0 -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"src/c_operations.d" -MT"src/c_operations.d" -o "src/c_operations.o" "src/c_operations.cpp"
	echo "Finished building: src/c_operations.cpp"
	echo "Building target: libOperation.a"
	echo "Invoking: GCC Archiver"
	ar -r  "libOperation.a"  ./src/c_operations.o ./src/operations.o   
	echo "ar: creating libOperation.a"
	echo "Finished building target: libOperation.a"
	#cp libOperation.a ../
