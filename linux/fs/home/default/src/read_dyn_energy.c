#include <stdio.h>
#include <unistd.h>
#include "vemu.h"

int main() {
    
    int interval = 1000000;
    
    unsigned long long int cycles, prev_cycles;
    
    cycles = vemu_get_total_cycles();
    
	while (1) {
		
        usleep(interval);
        
		prev_cycles = cycles;
		
		cycles = vemu_get_total_cycles();
		
		
		printf("total cycles=%llu, delta=%llu\n", cycles, cycles-prev_cycles);


	}

	


}
