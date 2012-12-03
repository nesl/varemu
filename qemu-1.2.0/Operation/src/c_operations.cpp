/*
 * c_operations.cpp
 * c++ implementation file (for c clients)
 *  Created on: Nov 27, 2012
 *      Author: salmaelmalaki
 */

#include "operations.h"
#include "c_operations.h"
#include <stdlib.h>
#include "stdio.h"
#include <iostream>


extern "C"{


unsigned int multiply(unsigned int src1, unsigned int src2){

    unsigned int dest;
	Bitset  A(32, src1); //size of unsigned int
    Bitset  B(32, src2);
    Bitset  Z(32, 0);

    printf("src1=%d, src2=%d\n", src1, src2);
    //printf("A=%d\t",(unsigned int)A.to_ulong());
    //printf("B=%d\n",(unsigned int)B.to_ulong());

    Multiply (A, B, Z);

    dest = (unsigned int)Z.to_ulong();

    printf("dest=%d\n",dest);

    return dest;

 } // multiply

} // extern "C"




