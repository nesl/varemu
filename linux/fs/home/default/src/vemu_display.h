#ifndef vemu_display_h
#define vemu_display_h

#include "vemu.h" 

typedef struct {
	double act_time[MAX_INSTR_CLASSES];
	double act_energy[MAX_INSTR_CLASSES];
	double cycles[MAX_INSTR_CLASSES];    
	double total_act_time;
	double total_act_energy;
	double total_cycles;  
	double slp_time;
	double slp_energy;  
	double error_status;
} vemu_regs_display;

typedef struct {
	char act_time[MAX_INSTR_CLASSES];
	char act_energy[MAX_INSTR_CLASSES];
	char cycles[MAX_INSTR_CLASSES];    
	char total_act_time;
	char total_act_energy;
	char total_cycles;  
	char slp_time;
	char slp_energy;  
	char error_status;
} vemu_regs_prefix;

char si_prefix[] = { 'n', 'u', 'm', ' ', 'K', 'M', 'G'  };

void vemu_prepare_display(vemu_regs_display * display, vemu_regs_prefix *prefix, vemu_regs * regs)
{
	char *prefix_ptr = (char *)prefix;
	
	int decimal_power[VEMU_STATE_N_VARS] =
		{ -3, -3, -3, -3, -3, -3, -3, -3,
		  -3, -3, -3, -3, -3, -3, -3, -3,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  -3,
		  -3,
		  0,
		  -3,
		  -3,
		  0,
		  0,
		  0
		};
	int i;
	double *display_array = (double *)display;
	uint64_t *regs_array = (uint64_t *)regs;
	for (i = 0; i < VEMU_STATE_N_VARS - 1; i++) 
	{
		display_array[i] = regs_array[i];		
		while (display_array[i] > 999) {
			display_array[i] /= 1000;
			decimal_power[i]++;
		}
		prefix_ptr[i] = si_prefix[decimal_power[i] + 3];
	}		
}



#endif
