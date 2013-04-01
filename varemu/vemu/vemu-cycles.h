//
//  vemu-cycles.h
//  
//
//  Created by Lucas Wanner on 11/19/12.
//
//

#ifndef _vemu_cycles_h
#define _vemu_cycles_h
#ifdef 	VEMU

#include "vemu/vemu.h"

#define 	MAX_INSTR_CLASSES	8

void vemu_init_cycles(void);
void vemu_increment_cycles(vemu_tb_info * tb_info);
void vemu_active_start(void);
void vemu_sleep_start(void);
uint64_t vemu_get_act_time(uint8_t class);
uint64_t vemu_get_cycles(uint8_t class);
uint64_t vemu_get_act_time_all_classes(void); 
uint64_t vemu_get_cycles_all_classes(void);
uint64_t vemu_get_slp_time(void);
uint64_t vemu_get_frequency(void);
void vamu_set_frequency(uint64_t f);

#endif // VEMU
#endif // _vemu_cycles_h
