// @author Gauresh D Rane
#ifndef STELLARIS_INSTRUCTION_MAP_H
#define STELLARIS_INSTRUCTION_MAP_H
#include "target-arm/arm_instruction_map.h"

void init_stellaris_instruction_set_map(void);

void init_stellaris_instruction_set_map(void)
{

	if(insn_map == NULL)
		return;
	get_map_entry("MOV_imm")->cycle_count = 1;
	get_map_entry("MOV_reg")->cycle_count = 1;
	get_map_entry("MOVT")->cycle_count = 1;
	get_map_entry("MOVW")->cycle_count = 1;
	get_map_entry("ADD_reg")->cycle_count = 1;
	get_map_entry("ADD_imm")->cycle_count = 3;
	get_map_entry("SUB_reg")->cycle_count = 1;
	get_map_entry("SUB_imm")->cycle_count = 1;
	get_map_entry("SBC_reg")->cycle_count = 1;
	get_map_entry("RSB_reg")->cycle_count = 3;
	get_map_entry("MUL")->cycle_count = 1;
	get_map_entry("SMULL")->cycle_count = 4; // 3-5 
	get_map_entry("UMULL")->cycle_count = 4; // 2-5
	get_map_entry("SMLAL")->cycle_count = 6; // 4-7
	get_map_entry("UMLAL")->cycle_count = 6; // 4-7
	get_map_entry("SDIV")->cycle_count = 9; // 2-12
	get_map_entry("UDIV")->cycle_count = 9; // 2-12
	get_map_entry("SSAT")->cycle_count = 1;
	get_map_entry("USAT")->cycle_count = 1;
	get_map_entry("CMP_reg")->cycle_count = 1;
	get_map_entry("CMN_reg")->cycle_count = 1;
	get_map_entry("AND_reg")->cycle_count = 1;
	get_map_entry("EOR_reg")->cycle_count = 1;
	get_map_entry("ORR_reg")->cycle_count = 1;
	get_map_entry("ORN_reg")->cycle_count = 1;
	get_map_entry("ORNS_reg")->cycle_count = 1;
	get_map_entry("BIC_reg")->cycle_count = 1;
	get_map_entry("BIC_imm")->cycle_count = 1;
	get_map_entry("MVN_reg")->cycle_count = 1;
	get_map_entry("TST_reg")->cycle_count = 1;
	get_map_entry("LSL_reg")->cycle_count = 1;
	get_map_entry("LSR_reg")->cycle_count = 1;
	get_map_entry("ASR_reg")->cycle_count = 1;
	get_map_entry("ROR_reg")->cycle_count = 1;
	get_map_entry("CLZ")->cycle_count = 1;
	get_map_entry("LDR_reg")->cycle_count = 3;
	get_map_entry("LDRH_reg")->cycle_count = 2;
	get_map_entry("LDRB_reg")->cycle_count = 2;
	get_map_entry("LDRSH_reg")->cycle_count = 2;
	get_map_entry("LDRSB_reg")->cycle_count = 2;
	get_map_entry("LDR_imm_off")->cycle_count = 2;
	get_map_entry("LDRH_imm_off")->cycle_count = 2;
	get_map_entry("LDRB_imm_off")->cycle_count = 2;
	get_map_entry("LDRD_imm")->cycle_count = 3;
	get_map_entry("LDRD_lit")->cycle_count = 3;
	get_map_entry("STR_reg")->cycle_count = 2;
	get_map_entry("STRH_reg")->cycle_count = 2;
	get_map_entry("STRB_reg")->cycle_count = 2;
	get_map_entry("STR_imm_off")->cycle_count = 2;
	get_map_entry("STRH_imm_off")->cycle_count = 2;
	get_map_entry("STRB_imm_off")->cycle_count = 2;
	get_map_entry("STRD_imm")->cycle_count = 3;
	get_map_entry("STM")->cycle_count = 1;
	get_map_entry("PUSH")->cycle_count = 1;
	get_map_entry("POP")->cycle_count = 1;
	get_map_entry("LDREX")->cycle_count = 2;
	get_map_entry("LDREXH")->cycle_count = 2;
	get_map_entry("LDREXB")->cycle_count = 2;
	get_map_entry("STREX")->cycle_count = 2;
	get_map_entry("STREXH")->cycle_count = 2;
	get_map_entry("STREXB")->cycle_count = 2;
	get_map_entry("CLREX")->cycle_count = 1;
	get_map_entry("B")->cycle_count = 3;
	get_map_entry("BLX_reg")->cycle_count = 3;
	get_map_entry("BL")->cycle_count = 3;
	get_map_entry("BX")->cycle_count = 3;
	get_map_entry("BFI")->cycle_count = 1;
	get_map_entry("BFC")->cycle_count = 1;
	get_map_entry("CBZ")->cycle_count = 2;
	get_map_entry("CBNZ")->cycle_count = 2;
	get_map_entry("TBB")->cycle_count = 4;
	get_map_entry("TBH")->cycle_count = 4;
	get_map_entry("SVC")->cycle_count = 0;
	get_map_entry("MRS")->cycle_count = 2;
	get_map_entry("MSR")->cycle_count = 2;
	get_map_entry("BKPT")->cycle_count = 0;
	get_map_entry("SXTH")->cycle_count = 1;
	get_map_entry("SXTB")->cycle_count = 1;
	get_map_entry("UXTH")->cycle_count = 1;
	get_map_entry("UXTB")->cycle_count = 1;
	get_map_entry("UBFX")->cycle_count = 1;
	get_map_entry("SBFX")->cycle_count = 1;
	get_map_entry("REV")->cycle_count = 1;
	get_map_entry("REV16")->cycle_count = 1;
	get_map_entry("REVSH")->cycle_count = 1;
	get_map_entry("RBIT")->cycle_count = 1;
}
#endif
