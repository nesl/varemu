//
//  vemu_power_tvlsi.c
//  
//
//  Created by Lucas Wanner on 11/19/12.
//  
//  
// 
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


typedef struct {
	double T;
	double at1;
	double at2;
	double at3;
	double ak1;
	double ak2;
	double ak3;
	double aalpha;
	double abeta;
	double apt1;
	double apt2;
	double apt3;
	double sA;
	double sB;
	double sC;
	double act_leak_alpha;	
} tvlsi_pm; 

static union {
	tvlsi_pm parameters;
	double pm_parray[16];
} pm_parameters; 

double vemu_pm_slp_power(void)
{
	tvlsi_pm *p = &pm_parameters.parameters;
	double t = p->T + 273;
	double power = p->sA*t*t/exp(p->sB/t) + p->sC;
	power *= 1.8;
	return power/1000000;
}

double vemu_pm_act_power(uint8_t class, uint64_t frequency)
{
	tvlsi_pm *p = &pm_parameters.parameters;
	double power = 0;
		
	if(p->T < p->at2) {
		power = p->apt1 + p->ak1*0.001 * pow(p->T - p->at1, p->aalpha);
	} else if (p->T < p->at3) {
		power = p->apt2 + p->ak2*0.001 * (p->T - p->at2);
	} else {
		power = p->apt3 + p->ak3*0.001 * pow(p->T - p->at3, p->abeta);
	}
	return power + vemu_pm_slp_power()*p->act_leak_alpha;
}

void vemu_pm_print_parameters(void) 
{
	vemu_debug("Power model parameters: T=%f, at1=%f, at2=%f, at3=%f, ak1=%f, ak2=%f, ak3=%f, aalpha=%f, abeta=%f, apt1=%f, apt2=%f, apt3=%f, sA=%f, sB=%f, sC=%f, act_leak_alpha=%f\n", pm_parameters.parameters.T, pm_parameters.parameters.at1, pm_parameters.parameters.at2, pm_parameters.parameters.at3, pm_parameters.parameters.ak1, pm_parameters.parameters.ak2, pm_parameters.parameters.ak3, pm_parameters.parameters.aalpha, pm_parameters.parameters.abeta, pm_parameters.parameters.apt1, pm_parameters.parameters.apt2, pm_parameters.parameters.apt3, pm_parameters.parameters.sA, pm_parameters.parameters.sB, pm_parameters.parameters.sC, pm_parameters.parameters.act_leak_alpha);
	vemu_debug("At t=%f, Active Power = %f W, Sleep Power = %f W\n", pm_parameters.parameters.T, vemu_pm_act_power(0,0), vemu_pm_slp_power());
}

void vemu_pm_change_parameter(uint8_t class, uint8_t parameter, double value)
{
	assert(parameter < 16);
	pm_parameters.pm_parray[parameter] = value;
}



#endif
