//
//  vemu-errors.h
//  
//
//  Created by Lucas Wanner on 11/27/12.
//
//

#ifndef _vemu_errors_h
#define _vemu_errors_h

#ifdef 	VEMU

#include "qemu-common.h"
#include "exec-all.h"

uint32_t vemu_error_pre(CPUArchState * env, TranslationBlock* tb);
uint32_t vemu_error_post(CPUArchState * env, TranslationBlock* tb);
uint32_t vemu_error_replace(CPUArchState * env, TranslationBlock* tb);


#endif // VEMU
#endif // _vemu_errors_h
