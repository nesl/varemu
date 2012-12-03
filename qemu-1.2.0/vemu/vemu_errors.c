//
//  vemu_errors.c
//  
//
//  Created by Lucas Wanner on 11/27/12.
//  

#ifdef	VEMU

#include "vemu-errors.h"
#include "vemu.h"
#include "c_operations.h"

uint64_t vemu_errors_enabled = 0;

#define VEMU_OPCODE_MUL 0x009
#define VEMU_OPCODE_MLA 0x029


static void vemu_mul(CPUArchState * env, uint32_t instr_word, bool errors_enabled)
{
    
    int rd = (instr_word >> 16) & 0xf;
	int rs = (instr_word >> 8) & 0xf;
	int rm = (instr_word) & 0xf;
    
    
    
    if (!errors_enabled ) {
        env->regs[rd] = env->regs[rm] * env->regs[rs];
    } else {
        //vemu_debug("pc: %x\trd: %d\trm: %d\trs: %d\n", env->regs[15], env->regs[rd], env->regs[rm], env->regs[rs]);
		env->regs[rd] = multiply(env->regs[rm], env->regs[rs]);
    }
        
}

static void vemu_mla(CPUArchState * env, uint32_t instr_word, bool errors_enabled)
{
    
    int rd = (instr_word >> 16) & 0xf;
	int rs = (instr_word >> 8) & 0xf;
	int rm = (instr_word) & 0xf;
	int rn = (instr_word >> 12) & 0xf;

    if (!errors_enabled ) {
        env->regs[rd] = env->regs[rm] * env->regs[rs] + env->regs[rn];
    }
    
}

static void vemu_umull(CPUArchState * env, uint32_t instr_word, bool errors_enabled)
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
	
	vemu_tb_info vemu_info = tb->vemu;
	uint32_t instr_word = vemu_info.instr_word;
    vemu_instr_info * instr_info = vemu_info.instr_info;

    /*
	if (strcmp(instr_info->name, "MUL") != 0) {
		return 0;
	} 
     */

    bool errors_enabled = (!privmode)  & vemu_errors_enabled;
    
    int set_cond = (instr_word >> 20) & 0x1;
    if (set_cond) {
        vemu_debug("VEMU Warning: Instruction %x should set condition\n", instr_word);
    }
    
    switch(instr_info->opcode) {
        case VEMU_OPCODE_MUL:
            vemu_mul(env, instr_word, errors_enabled);
            break;
        case VEMU_OPCODE_MLA:
            vemu_mla(env, instr_word, errors_enabled);
        default:
            break;
    }
    

    env->regs[15] += 4;

    
    
    
    return 0;

}

#endif
