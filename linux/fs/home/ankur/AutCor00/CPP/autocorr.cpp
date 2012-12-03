/* 
* Copyright (c) 1998 and 1999 by the EDN Embedded Microprocessor 
* Benchmark Consortium (EEMBC), Inc. 
*/ 

/* Modified */

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "errors.h"

using namespace std;

void
fxpAutoCorrelation (
    int   *InputData,     /* input data */
    int   *AutoCorrData,  /* output data */
    int   DataSize,       /* size of input data */
    int   NumberOfLags,   /* size of output data */
    int   Scale           /* partial product scale (bits) */
)
{
//    enable_errors();
    int   i;
    int   lag;
    int   LastIndex;
    int   Accumulator;

    /* Compute AutoCorrelation */
    for (lag = 0; lag < NumberOfLags; lag++) {
        Accumulator = 0;
        LastIndex = DataSize - lag;
        for (i = 0; i < LastIndex; i++) {
            Accumulator += ( InputData[i] * InputData[i+lag]) >> Scale;
        }

        /* Extract MSW of 1.31 fixed point accumulator */
        AutoCorrData[lag] = Accumulator;
    }
}


int main(int argc, const char **argv)
{
    if (argc < 3) {
        cerr << "Usage: <executable> <i/p file> <o/p file>\n";
        return 0;
    }
    

    int dataSize;
    int* input;
    int* output;

    ifstream infile;
    ofstream outfile;
    infile.open(argv[1]);
    outfile.open(argv[2]);

    infile >> dataSize;
    input = new int[dataSize];
    output = new int[dataSize];

    for (int j = 0; j < dataSize; j++) {
        infile >> input[j];
    }
    
    enable_errors();
    

    fxpAutoCorrelation(input, output, dataSize, dataSize, 16);

    disable_errors();

    
    outfile << dataSize << endl;
    for (int j = 0; j < dataSize; j++) {
        outfile << output[j] << endl;
    }

    delete input;
    delete output;
    

    return 0;
}
