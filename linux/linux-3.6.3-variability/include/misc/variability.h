/*
 * variability.h
 *
 * VarEMU driver
 * Lucas Wanner, 01/23/13
 *
 */

#ifndef _VARIABILITY_H_
#define _VARIABILITY_H_

#define VARMOD_BASE         0x101F5000       /* Variability Module */

#define SET_FAULTS				(0x000)
#define SET_FREQUENCY			(SET_FAULTS+8)	
#define SET_VOLTAGE				(SET_FREQUENCY+8)
#define READ_CMD       			(0xD00)
#define EXIT_CMD           		(0xF00)


#define	READ_HW				(0x1000000)
#define	READ_SYS			(0x0100000)
#define	READ_PROC			(0x0010000)


#define	MAX_INSTR_CLASSES	4
#define VEMU_STATE_N_VARS	(MAX_INSTR_CLASSES*3+8)

typedef struct {
	uint64_t act_time[MAX_INSTR_CLASSES];
	uint64_t act_energy[MAX_INSTR_CLASSES];
	uint64_t cycles[MAX_INSTR_CLASSES];    
	uint64_t total_act_time;
	uint64_t total_act_energy;
	uint64_t total_cycles;  
	uint64_t slp_time;
	uint64_t slp_energy;  
	uint64_t fault_status;
	uint64_t frequency;
	uint64_t voltage;
} vemu_regs;

typedef union {
	vemu_regs variables;
	uint32_t array32[VEMU_STATE_N_VARS*2];
	uint64_t array64[VEMU_STATE_N_VARS];
} vemu_state;


extern void vemu_process_in(void);
extern void vemu_process_out(void);

#endif /* _VARIABILITY_H_ */
