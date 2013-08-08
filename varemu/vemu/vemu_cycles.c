//
//  vemu_cycles.c
//  
//
//  Created by Lucas Wanner on 11/16/12.
//
//

#include <stdio.h>
#include "qemu/timer.h"
#include "vemu.h"
#include "qemu-common.h"
#include "vemu-cycles.h"


#ifdef 	VEMU

#define period_ns(F)	1e12/F

//uint64_t vemu_frequency = 500e9;
uint64_t vemu_frequency = 8e9;


double cycle_count[MAX_INSTR_CLASSES];
double active_time[MAX_INSTR_CLASSES];
int64_t total_sleep_time;	// Can be negative if actual runtime is faster than virtual runtime

uint64_t last_active_time_vm;
uint64_t last_active_time_cycles;
uint64_t last_sleep_time;

bool entered_sleep_mode;

uint64_t vemu_get_frequency(void)
{
	return vemu_frequency;
}

void vamu_set_frequency(uint64_t f)
{
	vemu_frequency = f;
}


uint64_t vemu_get_cycles(uint8_t class) 
{
	assert(class < MAX_INSTR_CLASSES);
	return cycle_count[class];
}

uint64_t vemu_get_cycles_all_classes(void)
{
	int i;
	uint64_t sum = 0;
	for (i = 0; i < MAX_INSTR_CLASSES; i++) {
		sum += cycle_count[i];
	}
	return sum;
}

uint64_t vemu_get_act_time(uint8_t class)
{
	assert(class < MAX_INSTR_CLASSES);
	return active_time[class];
}

uint64_t vemu_get_act_time_all_classes(void)
{
	int i;
	double sum = 0;
	for (i = 0; i < MAX_INSTR_CLASSES; i++) {
		sum += active_time[i];
	}
	return (uint64_t)sum;
}

uint64_t vemu_get_slp_time(void) 
{
	int64_t retval = 0;
	if (entered_sleep_mode) {
		uint64_t time = qemu_get_clock_ns(vm_clock);
		int64_t interval = time - last_sleep_time;
		retval = total_sleep_time + interval;		
	} else {
		retval = total_sleep_time;
	}
	if (retval < 0) return 0;
	return retval;
}


void vemu_increment_cycles(vemu_tb_info * tb_info)
{
	vemu_instr_info * instr_info = &(tb_info->instr_info);
	
    int class = instr_info->class;
    double cycles = instr_info->cycles;
	assert(class < MAX_INSTR_CLASSES);
	
	//vemu_debug("Instruction %x (opcode %x): %s (%f cycles)\n", tb_info->instr_word, instr_info->opcode,  instr_info->name, instr_info->cycles);
		
	if(cycles == 0) {
		vemu_debug("Warning: 0 cycles for opcode %x\n", instr_info->opcode);
	}
	cycle_count[class] += cycles;
	active_time[class] += cycles * period_ns(vemu_frequency);
}


void vemu_active_start(void) 
{
	if (!entered_sleep_mode) {
		// If we're not coming back from sleep, there's nothing to do.
		return;
	}
	
	/* Note: If interval is negative, HW would have not completed active 
	  instructions (and entered sleep mode) at this time. If frequency is small
	  enough (or host is fast enough), total_sleep_time may be negative.
	 */
	uint64_t time = qemu_get_clock_ns(vm_clock);
	int64_t interval = time - last_sleep_time;
	total_sleep_time += interval;
	last_active_time_vm = time;
	last_active_time_cycles = vemu_get_act_time_all_classes();		
	entered_sleep_mode = false;
	//uint64_t total_active_time = vemu_get_act_time_all_classes();
	//vemu_debug("VM Time: %llu, ACT: %llu, SLP: %llu, Delta: %lld\n", time, total_active_time, total_sleep_time, time - total_active_time - total_sleep_time);	
}

void vemu_sleep_start(void) 
{
	uint64_t time = qemu_get_clock_ns(vm_clock);
	uint64_t interval = time - last_active_time_vm;
	// Correct time to enter sleep mode: VM runs best effort, so VM Execution Time != HW Execution Time
	uint64_t act_period = vemu_get_act_time_all_classes() - last_active_time_cycles;
	int64_t delta = act_period - interval;
	last_sleep_time = time + delta;
	entered_sleep_mode = true;	
}





void vemu_init_cycles(void) 
{
	int i;
	vemu_debug("vemu_init_cycles\n");
	for (i = 0; i < MAX_INSTR_CLASSES; i++) {
		cycle_count[i] = 0;
	}
	last_active_time_vm = last_sleep_time = qemu_get_clock_ns(vm_clock);
	last_active_time_cycles = 0;
	total_sleep_time = 0;
	entered_sleep_mode = false;
}



#endif
