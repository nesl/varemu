#include <stdio.h>

#include "vemu.h"


int main() {

	vemu_regs proc;
	int interval = 1000000;

	
	do {
		usleep(interval);

		vemu_read(READ_PROC, &proc);
		printf("\033[2J");
		printf("\033[%d;%dH", 0, 0);	
		printf("Cycles:         %llu\n", proc.total_cycles);
		printf("Active Energy:  %llu nJ\n", proc.total_act_energy);
		printf("Frequency:      %llu MHz\n", (uint64_t)(proc.frequency/1e6));
		printf("Voltage:        %llu mV\n", proc.voltage);
		
		
		
	} while (1);
	
	
	return 0;
}
