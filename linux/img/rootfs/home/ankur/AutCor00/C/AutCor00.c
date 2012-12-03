/* 
* Copyright (c) 1998 and 1999 by the EDN Embedded Microprocessor 
* Benchmark Consortium (EEMBC), Inc. 
* 
* All Rights Reserved. This is licensed program product and 
* is owned by EEMBC. The Licensee understands and agrees that the 
* Benchmarks licensed by EEMBC hereunder (including methods or concepts 
* utilized therein) contain certain information that is confidential 
* and proprietary which the Licensee expressly agrees to retain in the 
* strictest confidence and to use only in conjunction with the Benchmarks 
* pursuant to the terms of this Agreement. The Licensee further agrees 
* to keep the source code and all related documentation confidential and 
* not to disclose such source code and/or related documentation to any 
* third party. The Licensee and any READER of this code is subject to 
* either the EEMBC Member License Agreement and/or the EEMBC Licensee 
* Agreement. 
* TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, EEMBC DISCLAIMS ALL 
* WARRANTIES, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, 
* IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
* PURPOSE, WITH REGARD TO THE BENCHMARKS AND THE ACCOMPANYING 
* DOCUMENTATION. LICENSEE ACKNOWLEDGES AND AGREES THAT THERE ARE NO 
* WARRANTIES, GUARANTIES, CONDITIONS, COVENANTS, OR REPRESENTATIONS BY 
* EEMBC AS TO MARKETABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR OTHER 
* ATTRIBUTES, WHETHER EXPRESS OR IMPLIED (IN LAW OR IN FACT), ORAL OR 
* WRITTEN. 
* 
* Licensee hereby agrees by accessing this source code that all benchmark 
* scores related to this code must be certified by ECL prior to publication 
* in any media, form, distribution, or other means of conveyance of 
* information subject to the terms of the EEMBC Member License Agreement 
* and/or EEMBC Licensee Agreement. 
* 
* Other Copyright Notice (if any): 
* 
* For conditions of distribution and use, see the accompanying README file. 
* 
* File:         AutCor00.c
* Subcommittee: EEMBC Telecom Subcommittee 
* Algorithm:    Fixed Point AutoCorrelation
* Author:       RP
* Origin Date: 
* Current Internal_Revision: 1.6 
* Revision History: 
* 
*   5/26/99    rlr
*       Initial cleanup
*
*   1/20/00    arw & sl
*       Correction of minor Sev 3 defects, cleanup
* 
* 
* - END OF HEADER - 
*/ 

/*******************************************************************************
    Includes                                                                    
*******************************************************************************/
#include "AutCor00.h"
#include <stdio.h>
#include <stdlib.h>
//#include "errors.h"

/*******************************************************************************
    Functions                                                                   
*******************************************************************************/
/*------------------------------------------------------------------------------
// FUNC    : fxpAutoCorrelation
//
// DESC    : 
//
// Compute the autocorrelation of the InputData (size = DataSize),
// store the results in AutoCorrData (size = NumberOfLags).
// For this application NumberOfLags is small (<64) so a direct
// sum-of-products implementation is used to compute the output.
// Partial products are scaled by Scale bits.
//
//         
// RETURNS : 
//      true/false
// ---------------------------------------------------------------------------*/

void
fxpAutoCorrelation (
    e_s16   *InputData,     /* input data */
    e_s16   *AutoCorrData,  /* output data */
    e_s16   DataSize,       /* size of input data */
    e_s16   NumberOfLags,   /* size of output data */
    e_s16   Scale           /* partial product scale (bits) */
)
{
//    enable_errors();
    n_int   i;
    n_int   lag;
    n_int   LastIndex;
    e_s32    Accumulator;

    /* Compute AutoCorrelation */
    for (lag = 0; lag < NumberOfLags; lag++) {
        Accumulator = 0;
        LastIndex = DataSize - lag;
        for (i = 0; i < LastIndex; i++) {
            Accumulator += ((e_s32) InputData[i] * (e_s32) InputData[i+lag]) >> Scale;
        }
        /* Extract MSW of 1.31 fixed point accumulator */
        AutoCorrData[lag] =  (Accumulator) ;
    }
}

int main (int argc, char** argv)
{
    if (argc < 3) {
        printf("Usage: <execuatable> <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }

    FILE *ifh;
    FILE *ofh;
    ifh = fopen(argv[1],"r");
    ofh = fopen(argv[2],"w");
    if (ifh == NULL) {
        printf("Can't open file %s",argv[1]);
        exit(1);
    }
    if (ofh == NULL) {
        printf("Can't open file %s",argv[2]);
        exit(1);
    }

    e_s16 dataSize;
    e_s16 *input = (e_s16 *) malloc(dataSize * sizeof(e_s16));
    e_s16 *output = (e_s16 *) malloc(dataSize * sizeof(e_s16));
    char *str = (char*)malloc(10*sizeof(char));
    int i;
    fgets(str, 10, ifh);
    dataSize = atoi(str);

    fprintf(ofh, "%d\n", dataSize);

    for (i = 0; i < dataSize; i++) {
        fgets(str, 10, ifh);
        input[i] = atoi(str);
    }
    fxpAutoCorrelation(input, output, dataSize, dataSize, 16);
    for (i = 0; i < dataSize; i++) {
        fprintf(ofh, "%d\n", output[i]);
    }

    free(str);
    return 0;
}
