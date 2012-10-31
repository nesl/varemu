// @author Gauresh Rane (gdrane@cs.ucla.edu)
// Initiailizes the instruction set of the processor
// All the instructions here are supported by QEMU.
#ifndef ARM_INSTRUCTION_MAP_H
#define ARM_INSTRUCTION_MAP_H
#include "qemu-variability.h"
// Instruction Types
#define DATA_PROC_ISN 1	// Data Processing instructions
#define BRANCH_ISN 2 		// Branch instructions 
#define MULTIPLY_ISN 3		// Multiply Instructions
#define LDST_ISN 4			// Load Store Instructions
#define MISC_ISN 5			// Miscellaneous Instructions
#define EXTENDED_LDST_ISN 6
#define INSTRUCTIONS_AVAILABLE 114

// extern struct variability_instruction_set* insn_map;
/*
void init_arm_instruction_set_map(void);
void increment_cycle_counter(TranslationBlock* tb, struct variability_instruction_set*s);
struct variability_instruction_set* get_map_entry(const char* instruction);
*/
extern struct variability_instruction_set arm_instructions [];
/*
struct variability_instruction_set arm_instructions [] = {
	{"ADC_reg", 0, DATA_PROC_ISN},
	{"ADD_reg", 0, DATA_PROC_ISN},
	{"ADD_imm", 0, DATA_PROC_ISN},
	{"SUB_reg", 0, DATA_PROC_ISN},
	{"SUB_imm", 0, DATA_PROC_ISN},
	{"MOV_imm", 0, DATA_PROC_ISN},
	{"MOV_reg", 0, DATA_PROC_ISN},
	{"CMP_imm", 0, DATA_PROC_ISN},
	{"LDR_imm_lit", 0, LDST_ISN},
	{"BLX_imm", 0, BRANCH_ISN},
	{"BLX_reg", 0, BRANCH_ISN},
	{"BX", 0, BRANCH_ISN},
	{"CMP_reg", 0, DATA_PROC_ISN},
	{"NEG_reg", 0, DATA_PROC_ISN},
	{"AND_reg", 0, DATA_PROC_ISN},
	{"EOR_reg", 0, DATA_PROC_ISN},
	{"LSL_reg", 0, DATA_PROC_ISN},
	{"LSR_reg", 0, DATA_PROC_ISN},
	{"ASR_reg", 0, DATA_PROC_ISN},
	{"ROR_reg", 0, DATA_PROC_ISN},
	{"TST_reg", 0, DATA_PROC_ISN},
	{"CMN_reg", 0, DATA_PROC_ISN},
	{"ORR_reg", 0, DATA_PROC_ISN},
	{"MUL", 0, MULTIPLY_ISN},
	{"MVN_reg", 0, DATA_PROC_ISN},
	{"BIC_reg", 0, DATA_PROC_ISN},
	{"LDR_reg", 0, LDST_ISN},
	{"STR_reg", 0, LDST_ISN},
	{"STRH_reg", 0, LDST_ISN},
	{"STRB_reg", 0, LDST_ISN},
	{"LDRH_reg", 0, LDST_ISN},
	{"LDRB_reg", 0, LDST_ISN},
	{"LDRSH_reg", 0, LDST_ISN},
	{"LDRSB_reg", 0, LDST_ISN},
	{"LDR_imm_off", 0, LDST_ISN},
	{"STR_imm_off", 0, LDST_ISN},
	{"LDRB_imm_off", 0, LDST_ISN},
	{"STRB_imm_off", 0, LDST_ISN},
	{"LDRH_imm_off", 0, LDST_ISN},
	{"STRH_imm_off", 0, LDST_ISN},
	{"POP", 0, LDST_ISN},
	{"PUSH", 0, LDST_ISN},
	{"UXTH", 0, MISC_ISN},
	{"UXTB", 0, MISC_ISN},
	{"SXTH", 0, MISC_ISN},
	{"SXTB", 0, MISC_ISN},
	{"BKPT", 0, MISC_ISN},
	{"REV", 0, DATA_PROC_ISN},
	{"CPS", 0, MISC_ISN},
	{"LDRM", 0, EXTENDED_LDST_ISN},
	{"STM", 0, EXTENDED_LDST_ISN},
	{"SVC", 0, MISC_ISN},
	{"SWI", 0, MISC_ISN},
	{"CBZ", 0, BRANCH_ISN},
	{"B", 0, BRANCH_ISN},
	{"CBNZ", 0, BRANCH_ISN},
	{"LDRD_lit", 0, LDST_ISN},
	{"LDRD_imm", 0, LDST_ISN},
	{"STRD_imm", 0, LDST_ISN},
	{"TBH", 0, BRANCH_ISN},
	{"TBB", 0, BRANCH_ISN},
	{"LDREXB", 0, LDST_ISN},
	{"LDREXH", 0, LDST_ISN},
	{"SRS", 0, MISC_ISN},
	{"RFE", 0, MISC_ISN},
	{"PHKTB", 0, DATA_PROC_ISN},
	{"PHKBT", 0, DATA_PROC_ISN},
	{"SXTB16", 0, MISC_ISN},
	{"UXTB16", 0, MISC_ISN},
	{"QADD", 0, DATA_PROC_ISN},
	{"QSUB", 0, DATA_PROC_ISN},
	{"RBIT", 0, DATA_PROC_ISN},
	{"REV16", 0, DATA_PROC_ISN},
	{"REVSH", 0, DATA_PROC_ISN},
	{"SEL", 0, DATA_PROC_ISN},
	{"CLZ", 0, DATA_PROC_ISN},
	{"LDREX", 0, LDST_ISN},
	{"SMLALD", 0, MULTIPLY_ISN},
	{"SMMUL", 0, MULTIPLY_ISN},
	{"SMMLA", 0, MULTIPLY_ISN},
	{"SMMLS", 0, MULTIPLY_ISN},
	{"USAD8", 0, DATA_PROC_ISN},
	{"SDIV", 0, MULTIPLY_ISN},
	{"UDIV", 0, MULTIPLY_ISN},
	{"UMAAL", 0, MULTIPLY_ISN},
	{"UMULL", 0, MULTIPLY_ISN},
	{"UMLAL", 0, MULTIPLY_ISN},
	{"SMULL", 0, MULTIPLY_ISN},
	{"SMLAL", 0, MULTIPLY_ISN},
	{"BL", 0, BRANCH_ISN},
	{"MSR", 0, MISC_ISN},
	{"CLREX", 0, MISC_ISN},
	{"MRS", 0, MISC_ISN},
	{"UBFX", 0, DATA_PROC_ISN},
	{"SSAT", 0, DATA_PROC_ISN},
	{"USAT", 0, DATA_PROC_ISN},
	{"MOVT", 0, DATA_PROC_ISN},
	{"RSB_imm", 0, DATA_PROC_ISN},
	{"RSB_reg", 0, DATA_PROC_ISN},
	{"SBC_imm", 0, DATA_PROC_ISN},
	{"SBC_reg", 0, DATA_PROC_ISN},
	{"RSC", 0, DATA_PROC_ISN}
};

void init_arm_instruction_set_map(void)
{
 	insn_map = (struct variability_instruction_set*)arm_instructions;
}

void increment_cycle_counter(TranslationBlock* tb, struct variability_instruction_set* s)
{
	if(s == NULL || tb == NULL)
		return ;
	else if(s->cycle_count == 0)
		{
			printf("Error\n");
			return;
		}
	switch(s->instruction_type)
	{
		case DATA_PROC_ISN:
							tb->data_proc_cycle_count += s->cycle_count;
							break;
		case BRANCH_ISN:
							tb->branch_cycle_count += s->cycle_count;
							break;
		case MULTIPLY_ISN:
							tb->multiply_cycle_count += s->cycle_count;
							break;
		case MISC_ISN:
							tb->misc_cycle_count += s->cycle_count;
							break;
	}
}
*/
#endif
