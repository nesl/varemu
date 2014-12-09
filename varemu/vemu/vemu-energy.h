//
//  vemu-energy.h
//  
//
//  Created by Lucas Wanner on 11/19/12.
//
//

#ifndef _vemu_energy_h
#define _vemu_energy_h
#ifdef 	VEMU

double vemu_pm_slp_power(void);
double vemu_pm_act_power(uint8_t class);
void vemu_pm_print_parameters(void); 
void vemu_pm_change_parameter(uint8_t class, uint8_t parameter, double value);
double vemu_pm_get_parameter(uint8_t class, uint8_t parameter);

void vemu_energy_init(void);
void vemu_energy_change_parameter(uint8_t cc, uint8_t pp, double vv);
uint64_t vemu_get_act_energy(uint8_t class);
uint64_t vemu_get_act_energy_all_classes(void); 
uint64_t vemu_get_slp_energy(void);
void vemu_pm_print_temp_p_curve(void);

double vemu_aging_evaluation (void);


#endif // VEMU
#endif // _vemu_cycles_h
