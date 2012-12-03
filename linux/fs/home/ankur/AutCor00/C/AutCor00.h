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
* File:         AutCor00.h
* Subcommittee: EEMBC Telecom Subcommittee 
* Algorithm:    Fixed Point AutoCorrelation
* Author:       RP
* Origin Date: 
* Current Internal_Revision: 0 
* Revision History: 
* 
*   5/26/99    rlr
*       Initial cleanup
* 
*   1/20/00    arw & sl
*       Correction of minor Sev 3 defects, cleanup
* 
* - END OF HEADER - 
*/ 

#ifndef __ALGO_H
#define __ALGO_H

/*******************************************************************************
    Includes                                                                    
*******************************************************************************/
#include "eembc_dt.h"
//#include "uu_encode.h"
/*******************************************************************************
    Defines                                                                     
*******************************************************************************/
#ifndef USE_UUENCODING
#define MAX_DATA_SIZE 1024
#endif

/* Compile time Data set select for uuencode: 
 * DATA_1 or DATA_2 or DATA_3
 * DATA_3 is default
 */ 
#define DATA_3

/* OUTPUT_SCALE is used to accomodate data size limit of 16 bits */
#define OUTPUT_SCALE 16

/*******************************************************************************
    Global Variables                                                            
*******************************************************************************/
#ifndef ALGO_GLOBALS            /* Don't define these twice! */
#define ALGO_GLOBALS
#endif /* ALGO_GLOBALS */

/*******************************************************************************
    Function Prototypes                                                         
*******************************************************************************/

void fxpAutoCorrelation (
    e_s16   *InputData,     /* input data */
    e_s16   *AutoCorrData,  /* output data */
    e_s16   DataSize,       /* size of input data */
    e_s16   NumberOfLags,   /* size of output data */
    e_s16   Scale           /* partial product scale (bits) */
    );

#endif /* __ALGO_H */
