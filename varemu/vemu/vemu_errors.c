//
//  vemu_errors.c
//  
//
//  Created by Lucas Wanner on 11/27/12.
//  

#ifdef	VEMU

#include "vemu-errors.h"
#include "vemu.h"
#include "tcg-op.h"


uint64_t vemu_errors_enabled = 0;

#define VEMU_OPCODE_MUL 0x009
#define VEMU_OPCODE_MLA 0x029
#define VEMU_OPCODE_UMULL 0x089



static void vemu_mul(CPUArchState * env, uint32_t instr_word)
{
    
    int rd = (instr_word >> 16) & 0xf;
	int rs = (instr_word >> 8) & 0xf;
	int rm = (instr_word) & 0xf;
    
    env->regs[rd] = env->regs[rm] + env->regs[rs];    
}

static void vemu_mla(CPUArchState * env, uint32_t instr_word)
{
    
    int rd = (instr_word >> 16) & 0xf;
	int rs = (instr_word >> 8) & 0xf;
	int rm = (instr_word) & 0xf;
	int rn = (instr_word >> 12) & 0xf;

    env->regs[rd] = env->regs[rm] * env->regs[rs] + env->regs[rn];
    
}

static void vemu_umull(CPUArchState * env, uint32_t instr_word)
{

}


uint32_t vemu_error_pre(CPUArchState * env, TranslationBlock* tb)
{
    return 0;
}
uint32_t vemu_error_post(CPUArchState * env, TranslationBlock* tb)
{
    return 0;
}

uint32_t vemu_error_replace(CPUArchState * env, TranslationBlock* tb)
{
	int privmode = 0;
	if(arm_feature(env, ARM_FEATURE_M)) {
			privmode = !((env->v7m.exception) && (env->v7m.control & 1));
	}	else { 
			privmode = (env->uncached_cpsr & CPSR_M) != ARM_CPU_MODE_USR;
	}	
	
    bool errors_enabled = (!privmode)  & (vemu_errors_enabled > 0);
	
	if(!errors_enabled) {
		return 0;
	}
	
	vemu_tb_info vemu_info = tb->vemu;
	uint32_t instr_word = vemu_info.instr_word;
    vemu_instr_info * instr_info = vemu_info.instr_info;
   
    int set_cond = (instr_word >> 20) & 0x1;
    if (set_cond) {
        vemu_debug("VEMU Warning: Instruction %x should set condition\n", instr_word);
    }
    
    switch(instr_info->opcode) {
        case VEMU_OPCODE_MUL:
            vemu_mul(env, instr_word);
            break;
        case VEMU_OPCODE_MLA:
            vemu_mla(env, instr_word);
        case VEMU_OPCODE_UMULL:
            vemu_umull(env, instr_word);            
        default:
            break;
    }    
    
    return 1;

}

#endif
