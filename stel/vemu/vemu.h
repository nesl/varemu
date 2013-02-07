/*
 * variability.h
 *
 * VarEMU driver for Stellaris
 * Lucas Wanner, 02/06/13
 *
 */

#ifndef _VARIABILITY_H_
#define _VARIABILITY_H_

#define VARMOD_BASE         0x40022000       /* Variability Module */

#define ACT_TIME			0x000
#define ACT_EN				(ACT_TIME + 8*MAX_INSTR_CLASSES)
#define CYCLES				(ACT_EN + 8*MAX_INSTR_CLASSES)
#define TOTAL_ACT_TIME		(CYCLES + 8*MAX_INSTR_CLASSES)
#define TOTAL_ACT_EN		(TOTAL_ACT_TIME + 8)
#define TOTAL_CYCLES		(TOTAL_ACT_EN + 8)
#define SLP_TIME			(TOTAL_CYCLES + 8)
#define SLP_ENERGY			(SLP_TIME + 8)
#define ERRORS_EN			(SLP_ENERGY + 8)

#define READ_CMD       		(0xD00)
#define EXIT_CMD           	(0xF00)

#define	READ_HW				(0x1000000)
#define	READ_SYS			(0x0100000)
#define	READ_PROC			(0x0010000)


#define	MAX_INSTR_CLASSES	8
#define VEMU_STATE_N_VARS	(MAX_INSTR_CLASSES*3+6)

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef struct {
	uint64_t act_time[MAX_INSTR_CLASSES];
	uint64_t act_energy[MAX_INSTR_CLASSES];
	uint64_t cycles[MAX_INSTR_CLASSES];    
	uint64_t total_act_time;
	uint64_t total_act_energy;
	uint64_t total_cycles;  
	uint64_t slp_time;
	uint64_t slp_energy;  
	uint64_t error_status;
} vemu_regs;

typedef union {
	vemu_regs variables;
	uint32_t array32[VEMU_STATE_N_VARS*2];
	uint64_t array64[VEMU_STATE_N_VARS];
} vemu_state;

uint64_t vemu_read_reg(int offset);
void vemu_read_state(vemu_regs * target);
void vemu_write_reg(int offset, uint64_t val);
void vemu_delta(vemu_regs * target, vemu_regs * new, vemu_regs * old);


#endif /* _VARIABILITY_H_ */
