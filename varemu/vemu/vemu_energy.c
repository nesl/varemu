//
//  vemu_energy.c
//  
//
//  Created by Lucas Wanner on 11/20/12.
//
//

#include <stdio.h>
#include <math.h>
#include "vemu.h"
#include "qemu-common.h"
#include "vemu-cycles.h"
#include "vemu-energy.h"
#include "qemu/timer.h"


#ifdef 	VEMU

uint64_t act_time_ckpt[MAX_INSTR_CLASSES];
uint64_t slp_time_ckpt;
double act_energy[MAX_INSTR_CLASSES];
double slp_energy;

uint64_t vemu_get_act_energy(uint8_t class)
{
	assert(class < MAX_INSTR_CLASSES);
	uint64_t curr_time = vemu_get_act_time(class);
	uint64_t interval = curr_time - act_time_ckpt[class]; // pS
	double energy =  interval * vemu_pm_act_power(class);
	//vemu_debug("ACT Interval: %llu, Power: %f, Energy: %f\n", interval, vemu_pm_act_power(class, vemu_frequency), energy);
	act_time_ckpt[class] = curr_time;
	act_energy[class] += energy;
	return (uint64_t)(act_energy[class]); 
}

uint64_t vemu_get_act_energy_all_classes(void)
{
	int i;
	uint64_t sum = 0;
	for (i = 0; i < MAX_INSTR_CLASSES; i++) {
		sum += vemu_get_act_energy(i);
	}
	return sum;
}

uint64_t vemu_get_slp_energy(void) 
{
	uint64_t curr_slp_time = vemu_get_slp_time();
	int64_t interval = curr_slp_time - slp_time_ckpt;  // pS
	double energy = interval * vemu_pm_slp_power();	
	//vemu_debug("SLP Interval: %lld, Power: %f, Energy: %f\n", interval, vemu_pm_slp_power(), energy);	
	if (interval < 0) {
		// If interval is negative, host is not keeping up with expected HW frequency
		// Don't accumulate sleep energy now, wait for host to catch up
		return (uint64_t)slp_energy; 
	}
	slp_time_ckpt = curr_slp_time;
	slp_energy += energy;
	return (uint64_t)slp_energy; 
}

void vemu_energy_init(void)
{
	int i;
	for (i = 0; i < MAX_INSTR_CLASSES; i++) {
		act_time_ckpt[i] = 0;
		act_energy[i] = 0;
	}
	slp_time_ckpt = 0;
	slp_energy = 0;
}	

void vemu_energy_change_parameter(uint8_t cc, uint8_t pp, double vv) {
	
	printf("Change model parameter %d of class %d to %f \n",  pp, cc, vv);
	vemu_get_act_energy_all_classes();
	vemu_get_slp_energy();
	vemu_pm_change_parameter(cc, pp, vv);
	vemu_pm_print_parameters();			
}




#endif
