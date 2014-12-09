//
//  vemu_power_45nm.c
//  
//
//  Created by Liangzhen Lao on 03/28/13.
//
//  Moved frequency to PM on 12/8/14 / Lucas
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

#define TIME_SCALAR 1051200  // 1 year : 5 minutes

#define ADAPTIVE 0


typedef struct {
	double F;	// Frequency
	double Vdd;	// supply voltage
	double T;	// temperature in celsius
	double Nd;	// dynamic power multiplier
	double Bd;	// nominal value: 10.7
	double Cd;	// nominal value: 0.605
	double Nl;	// leakage power multiplier
	double Al;	// nominal value -9210
	double Bl;	// nominal value 890
	double Vnl;	// nominal value 0.2
	double Vpl;	// nominal value 0.232
	double delta_vtp;	// pmos threshold degradation
	double mode;		// 1 or 0 for upper/lower bound for aging
	double Vdd_scalar;	// new Vdd scalar to compensate aging
	//double Freq_scalar;	// alternative new frequency scalar to compensate aging
} esweek_pm;


static union {
	esweek_pm parameters;
	double pm_array[14];
} pm_parameters;

double vemu_aging_evaluation (void) {
	esweek_pm *p = &pm_parameters.parameters;
	double act_time = (double)vemu_get_act_time(0)/1e9*TIME_SCALAR;
	double clk_gated_time =  (double)vemu_get_act_time(1)/1e9*TIME_SCALAR;
	double slp_time =  (double)vemu_get_slp_time()/1e9*TIME_SCALAR;


	double omega;
	if (p->mode<0.5) {
		omega = (0.5 * act_time + clk_gated_time) / (slp_time + act_time);
	} else {
		omega = (act_time + clk_gated_time) / (slp_time + act_time);
	}
	if(omega>0.99) { omega=0.99; }
	double t = act_time + slp_time + clk_gated_time;
	
	double temp = p->T + 273;
	
	double C = 1e8 * exp(-9.8642e+03 / temp);
	double Kv = 2.9642e-44 * (p->Vdd - 0.65) * pow(C,0.5) * exp((p->Vdd-0.65)/0.1008);
	double bt = (1.1902e-09 + 0.0707* pow((1-omega)*C,0.5))  /(1.3224*1e-9+   pow((C*t),0.5)    );
	bt = 1 - bt;
	double delta_vt = 1.9174e+09 * pow(((   pow(0.1*omega,0.5)   *Kv)/(1-  pow(bt,3.6232)  )),0.276);
	double nom_delay = p->Vdd/pow((p->Vdd-(0.4+0)),1.1) + p->Vdd/pow((p->Vdd-0.5),1.1);
#ifdef ADAPTIVE
	if ( nom_delay < (p->Vdd_scalar*p->Vdd)/pow(((p->Vdd_scalar*p->Vdd)-(0.4+delta_vt)),1.1) + (p->Vdd_scalar*p->Vdd)/pow(((p->Vdd_scalar*p->Vdd)-0.5),1.1) )
	p->Vdd_scalar += 0.005;
#endif
	p->delta_vtp = delta_vt;
	return delta_vt;
}

double vemu_pm_slp_power(void)
{
	esweek_pm *p = &pm_parameters.parameters;
	double t = p->T + 273;
	double v = p->Vdd;
#ifdef ADAPTIVE
	v *= p->Vdd_scalar;
#endif 
	double power = p->Nl * v * t*t * ( exp(p->Al*p->Vnl/t) + exp(p->Al*(p->Vpl+p->delta_vtp)/t) ) * exp(p->Bl*v/t);
	return power/1000000;
}

double vemu_pm_act_power(uint8_t class)
{
	esweek_pm *p = &pm_parameters.parameters;
	double v = p->Vdd;
	double f = p->F/1e6;
#ifdef ADAPTIVE
	v *= p->Vdd_scalar;
#endif
	double power = v*v + p->Bd * pow(( v - p->Cd - p->delta_vtp ),3);
	power = power * p->Nd * f;
	return power + vemu_pm_slp_power();
}

void vemu_pm_print_parameters(void)
{
	vemu_debug("Power model parameters: F=%ld, T=%f, Vdd=%f, Nd=%f, Bd=%f, Cd=%f, Nl=%f, Al=%f, Bl=%f, Vnl=%f, Vpl=%f, delta_vtp=%f\n",
	(long int)pm_parameters.parameters.F,pm_parameters.parameters.T, pm_parameters.parameters.Vdd, pm_parameters.parameters.Nd, pm_parameters.parameters.Bd, pm_parameters.parameters.Cd, pm_parameters.parameters.Nl, pm_parameters.parameters.Al, pm_parameters.parameters.Bl, pm_parameters.parameters.Vnl, pm_parameters.parameters.Vpl, pm_parameters.parameters.delta_vtp);
	vemu_debug("At t=%f, F=%d MHz, Vdd=%f, Delta_vth = %f V, Active Power = %f W, Sleep Power = %f W\n", pm_parameters.parameters.T, (int)(pm_parameters.parameters.F/1e6), pm_parameters.parameters.Vdd, pm_parameters.parameters.delta_vtp, vemu_pm_act_power(0), vemu_pm_slp_power());
}

void vemu_pm_change_parameter(uint8_t class, uint8_t parameter, double value)
{
	assert(parameter < 14);
	//vemu_debug("p=%d, v=%f\n",parameter, value);
	pm_parameters.pm_array[parameter] = value;
}

double vemu_pm_get_parameter(uint8_t class, uint8_t parameter){
	assert(parameter < 14);
	return pm_parameters.pm_array[parameter];
}

void vemu_pm_print_temp_p_curve(void) 
{
	int ttt;
	double oldT = pm_parameters.pm_array[0];
	printf("T, Ps, Pa:\n");
	for (ttt = -20; ttt <= 100; ttt++) {
		pm_parameters.pm_array[0] = (double)ttt;
		printf("%d,%f,%f\n", ttt, vemu_pm_slp_power(), vemu_pm_act_power(0));
	}
	pm_parameters.pm_array[0] = oldT;
}


#endif
