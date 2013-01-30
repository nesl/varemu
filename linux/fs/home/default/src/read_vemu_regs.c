#include <stdio.h>

#include "vemu.h"
#include "vemu_display.h"

typedef struct {
	vemu_regs hw, sys, proc;
	vemu_regs_display hw_d, sys_d, proc_d;
	vemu_regs_prefix hw_p, sys_p, proc_p;
} all_info;





int main() {
	all_info curr, prev, delta;
	all_info *cp, *pp, *tp, *dp;
	int interval = 1000000;
	cp = &curr;
	pp = &prev;
	dp = &delta;
	
	do {
		usleep(interval);
		tp = pp;
		pp = cp;
		cp = tp;
		vemu_read(READ_HW, &(cp->hw));
		vemu_read(READ_SYS, &(cp->sys));
		vemu_read(READ_PROC, &(cp->proc));
		vemu_delta(&dp->hw, &cp->hw, &pp->hw);
		vemu_delta(&dp->sys, &cp->sys, &pp->sys);
		vemu_delta(&dp->proc, &cp->proc, &pp->proc);
		vemu_prepare_display(&(cp->hw_d), &(cp->hw_p), &(cp->hw));
		vemu_prepare_display(&(cp->sys_d), &(cp->sys_p), &(cp->sys));
		vemu_prepare_display(&(cp->proc_d), &(cp->proc_p), &(cp->proc));
		vemu_prepare_display(&(dp->hw_d), &(dp->hw_p), &(dp->hw));
		vemu_prepare_display(&(dp->sys_d), &(dp->sys_p), &(dp->sys));
		vemu_prepare_display(&(dp->proc_d), &(dp->proc_p), &(dp->proc));	
		
		
		printf("\033[2J");
		printf("\033[%d;%dH", 0, 0);
				
		printf("--------------------------------------------------------------------------------\n");
		printf("Cycles: \n");
			
		printf("hw:    %#7.2f %cC   sys:   %#7.2f %cC   proc:  %#7.2f %cC\n", 
				cp->hw_d.total_cycles, cp->hw_p.total_cycles, 
				cp->sys_d.total_cycles, cp->sys_p.total_cycles, 
				cp->proc_d.total_cycles, cp->proc_p.total_cycles);
		printf("delta: %#7.2f %cC   delta: %#7.2f %cC   delta: %#7.2f %cC\n", 
				dp->hw_d.total_cycles, dp->hw_p.total_cycles, 
				dp->sys_d.total_cycles, dp->sys_p.total_cycles, 
				dp->proc_d.total_cycles, dp->proc_p.total_cycles);	
				
		printf("--------------------------------------------------------------------------------\n");
		printf("Time: \n");
			
		printf("hw:    %#7.2f %cs   sys:   %#7.2f %cs   proc:  %#7.2f %cs   sleep: %#7.2f %cs\n", 
				cp->hw_d.total_act_time, cp->hw_p.total_act_time, 
				cp->sys_d.total_act_time, cp->sys_p.total_act_time, 
				cp->proc_d.total_act_time, cp->proc_p.total_act_time,
				cp->hw_d.slp_time, cp->hw_p.slp_time);				
				
		printf("delta: %#7.2f %cs   delta: %#7.2f %cs   delta: %#7.2f %cs   delta: %#7.2f %cs\n", 
				dp->hw_d.total_act_time, dp->hw_p.total_act_time, 
				dp->sys_d.total_act_time, dp->sys_p.total_act_time, 
				dp->proc_d.total_act_time, dp->proc_p.total_act_time,
				dp->hw_d.slp_time, dp->hw_p.slp_time);				
								
				
		printf("--------------------------------------------------------------------------------\n");
		printf("Energy: \n");
					
		printf("hw:    %#7.2f %cJ   sys:   %#7.2f %cJ   proc:  %#7.2f %cJ   sleep: %#7.2f %cJ\n", 
				cp->hw_d.total_act_energy, cp->hw_p.total_act_energy, 
				cp->sys_d.total_act_energy, cp->sys_p.total_act_energy, 
				cp->proc_d.total_act_energy, cp->proc_p.total_act_energy,
				cp->hw_d.slp_energy, cp->hw_p.slp_energy);
				
		printf("delta: %#7.2f %cJ   delta: %#7.2f %cJ   delta: %#7.2f %cJ   delta: %#7.2f %cJ\n", 
				dp->hw_d.total_act_energy, dp->hw_p.total_act_energy, 
				dp->sys_d.total_act_energy, dp->sys_p.total_act_energy, 
				dp->proc_d.total_act_energy, dp->proc_p.total_act_energy,	
				dp->hw_d.slp_energy, dp->hw_p.slp_energy);
		printf("--------------------------------------------------------------------------------\n");


		
	} while (1);
	
	
	return 0;
}
