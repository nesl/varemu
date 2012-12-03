
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
* Other Copyright Notice (if any): None
*
* For conditions of distribution and use, see the accompanying README file.
*
* File:  EEMBC_dt.h   EEMBC Datatypes Abstraction File
* Standard Include file, Defines all EEMBC common types and constants
* Subcommittee: EEMBC Technical Advisory Group (TechTAG)
*   NOTE : The typedefs in this file are typed to be the same size
*          on all targets.  But note that this does not guarantee
*          that structurs defined with these types will have the same
*          size or that structure members will be at the same offset.
*          This is due to alignment requirements of the target processor.
*
*  NOTE:   You are expected to edit this file to fit your architecture!
* Algorithm # For use on all algorithms/kernels
* Author: rr, pc, arw, and the Technical Posse of EEMBC
* Origin Date: May 13, 1999
* Current Internal_Revision: 0.2
* Revision History:
* Origin Date: 05/13/99
* Note1:  arw consolidated all input into one file 8/6/99, including n_native types,
*         inclusion on win32 definition file,
*/ 

#ifndef EEMBC_DT_H
#define EEMBC_DT_H

/*----------------------------------------------------------------------------*/
/*
* Include the compilers STDDEF.H here so we get the correct defition
* of size_t
*/

#include <stddef.h>

/* If we are using this header when compiling for a windows target
** then we need the windows type defintion file....
*/

#if (defined( __WIN32__ ) || defined( WIN32 )) || defined ( _WIN32 ) && defined( _MSC_VER )

   #define  MSVC_WIN (1)  /* indicates windows compile under MS Viual C++ */

   #ifndef __wtypes_h__
   #include <wtypes.h>
   #endif

   #ifndef __WINDEF_
   #include <windef.h>
   #endif

#endif

/*------------------------------------------------------------------------------
* These defines are set depending on what kind of data types your compiler
* and target support.
*
* Each of these data types MUST BE defined in a manner which gives
* them a size of <<AT LEAST>> the number of bits indicated by its name. 
*
* The 8bit, 16bit, 24bit and 32bit integral data types are required.
* The 48bit and 64bit integral types are optional.
* The 32bit and 64bit floating point types are optional
* ---------------------------------------------------------------------------*/

/* integral data types */
#define HAVE_64    (0)    /* at least 64 bits in size  */
#define HAVE_48    (0)    /* at least 48 bits in size  */

/* Floating Point data types */
#define HAVE_F32   (1)    /* at least 32 bits in size  */
#define HAVE_F64   (1)    /* at least 64 bits in size  */

/*------------------------------------------------------------------------------
 * EEMBC Standard Data Types
 *
 * Note that these all  are prefixed with "e_"
 */

typedef unsigned char           e_u8;
typedef signed   char           e_s8;

typedef unsigned short          e_u16;
typedef signed   short          e_s16;

typedef unsigned long           e_u24;
typedef signed   long           e_s24;

typedef unsigned long           e_u32;
typedef signed   long           e_s32;

#if HAVE_48
typedef unsigned long           e_u48;
typedef signed long             e_s48;
#endif

#if HAVE_64
typedef unsigned long           e_u64;
typedef signed long             e_s64;
#endif

#if HAVE_F32
typedef float                   e_f32;
#endif

#if HAVE_F64
typedef double                  e_f64;
#endif



/*------------------------------------------------------------------------------
* Native Size Types
*/

/* Native types are used when you want to let the compiler
*  figure out the native machine size (for optimizations)
*  but should NOT be used when the sizeof() variable matters
*  in the calculation
*
*/

typedef char                    n_char;
typedef unsigned char           n_uchar;
typedef short                   n_short;
typedef unsigned short          n_ushort;
typedef int                     n_int;
typedef unsigned int            n_uint;
typedef long                    n_long;
typedef unsigned long           n_ulong;
typedef float                   n_float;
typedef double                  n_double;
typedef void					n_void; 

typedef int LoopCount;

/* LC_FMT is a string used with printf() to format a 'LoopCount' typed value
// It is used like this printf("loop count = " LC_FMT "\n", loop_count);
*/

#define LC_FMT(w)  "%"#w"lu"
#define LC_FMTZ(w) "%0"#w"lu"
#define LC_FMTH(w) "%0"#w"X"

/*------------------------------------------------------------------------------
// ST_FMT is a string used with printf() to format a 'size_t' typed value/
// It is used like this printf( "value = " ST_FMT "\n", value );
*/

#define ST_FMT(w)   "%"#w"lu"
#define ST_FMTZ(w) "%0"#w"lu"
#define ST_FMTH(w) "%0"#w"lX"



/*------------------------------------------------------------------------------
* Fixed Size Types
*/

/*------------------------------------------------------------------------------
// Fixed Size Types
*/

/* These types are always set to be a specific size
//
// If the target does not support a type of the required size, then set the
// type to the smallest size native data type that will hold the defined type.
//
// Note: These typedefs will all maintain there proper (indicatd) sizefor
//       boht the 16 and 32 bit models for the 16/32 bit compilers listed
//       in the module header.                                           
*/

/* Portable unsigned types
//                                                   
*/
typedef          e_u8   Byte;     /* Always  8 bits (unsigned) */
typedef          e_u16  Word;     /* Always 16 bits (unsigned) */
typedef          e_u32  Dword;    /* Always 32 bits (unsigned) */

/* Portable signed types
//                                                             */
typedef          e_s8   Char;     /* Always  8 bits (signed)   */
typedef          e_s16  Short;    /* Always 16 bits (signed)   */
typedef          e_s16  Bool;     /* Always 16 Bits (signed)   */
typedef          e_s32  Long;     /* Always 32 bits (signed)   */

/* Other types
//                                                             */
typedef          e_s16  Status;   /* Always 16 bits (signed)   */


/* The status type is used by functions which return 'Success' or 'Failure' */
/* where 'Success' is always zero and 'Failure' is defined as default value */
/* of '1' but is also considered to be any non zero value.  This is defied */
/* as a short because we usually don't need 32bit value here. */
/* BUT! Status >does< need to be a signed! */


/*------------------------------------------------------------------------------
* Defines and Macros
*/

#undef NULL                     /* Correctly Define NULL */

#ifdef __cplusplus
  #define NULL (0)
#else
  #define NULL ((void*)(0))
#endif

/* Ignore common defines from previous headers (this file does it right) */
#undef  FALSE
#undef  TRUE
#undef  NO
#undef  YES
#undef  BAD
#undef  GOOD
#undef  STOP
#undef  GO
#undef  CLEAR
#undef  SET
#undef  OFF
#undef  ON
#undef  OK
#undef  NOTOK
#undef  SUCCESS
#undef  FAILURE
#undef  ERROR
#undef  NOERR
#undef  ENABLE
#undef  DISABLE

#define FALSE   (0)                    /* See FN#2 */
#define TRUE    (!FALSE)
#define NO      (0)
#define YES     (1)
#define BAD     (0)
#define GOOD    (1)
#define STOP    (0)
#define GO      (1)
#define CLEAR   (0)
#define SET     (1)
#define OFF     (0)
#define ON      (1)

#define OK      (0)
#define NOTOK   (1)
#define SUCCESS (0)
#define FAILURE (1)
#define VALID   (1)
#define INVALID (0)

#define ENABLE  (1)
#define DISABLE (0)

/*- Constants =
----------------------------------------------------------------*/

#ifdef __cplusplus

   const Bool   False     =  0;         /* See FN#1 */
   const Bool   True      =  (!False);
   const Bool   No        =  0;
   const Bool   Yes       =  1;
   const Bool   Bad       =  0;
   const Bool   Good      =  1;
   const Bool   Stop      =  0;
   const Bool   Go        =  1;
   const Bool   Clear     =  0;
   const Bool   Set       =  1;
   const Bool   Off       =  0;
   const Bool   On        =  1;

   const Status Ok        =  0;
   const Status Success   =  0;
   const Status Valid     =  0;
   const Status Notok     =  1;
   const Status Failure   =  1;
   const Status Invalid   =  1;

   const short  Enable    =  1;
   const short  Disable   =  0;

#else

   #define False     ((Bool)(0))
   #define True      ((Bool)(1)) 

   #define No        ((Bool)(0))
   #define Yes       ((Bool)(1))
   #define Bad       ((Bool)(0))
   #define Good      ((Bool)(1))
   #define Stop      ((Bool)(0))
   #define Go        ((Bool)(1))
   #define Clear     ((Bool)(0))
   #define Set       ((Bool)(1))
   #define Off       ((Bool)(0))
   #define On        ((Bool)(1))

   #define Ok        ((Status)(0))
   #define Success   ((Status)(0))
   #define Valid     ((Status)(0))
   #define Notok     ((Status)(1))
   #define Failure   ((Status)(1))
   #define Invalid   ((Status)(1))

   #define Enable    ((Status)(1))
   #define Disable   ((Status)(0))

#endif /* if C++ */

/*------------------------------------------------------------------------------
 * Miscelaneous MACROS
 */

/* Round a value upward to the nearest multiple of 4 */
#define ROUNDUP4(x) (((x) + 3) & -4)
#define ROUNDUP(x,y) ((((n_long)x) + ((n_long)(y-1))) & ((n_long)(0-y)))

/*---------------------------------------------------------------------------*/

#endif /* EEMBC_DT_H */


/* Function prototype declaration */
n_int dt_chk(void);



/*
* FOOT NOTES :

FN#1  -------------------------------------------------------------------------


Note that global types are not declared using Enumerations.  This is because
the size of an enumeration is compiler dependant.  While this can often be
controlled (usually between an int or char) using compiler switches, it is
highly non-portable so we don't do it here.

FN#2  -------------------------------------------------------------------------

All tokens that are in UPPER CASE should are assumed to be #defined.
Constants and typedefes, structs, and class names, all begin with a capital
letter.

* ENDOF  : EEMBC_DT.h
* 
*/



