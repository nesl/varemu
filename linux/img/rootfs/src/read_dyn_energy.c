#include <stdio.h>
#include <unistd.h>

#define VAR_SYSCALL 	378

#define REG_BASE 0x280
#define CYCLES_LO			0x000
#define CYCLES_HI			0x280
#define DYN_EN_LO			0x280
#define DYN_EN_HI			0x500
#define TOTAL_CYCLES		0x500
#define TOTAL_ACT_ENERGY	0x540
#define TOTAL_SLP_ENERGY	0x580
#define ACT_ERRORS			0xFC0

int main() {
	int nreg = 10;
	
    unsigned int i, j;
    unsigned long long int tempv = 0;
	unsigned long long int v[nreg];
	unsigned long long int prev[nreg];
    int r;
    
    int offset;
    
    int interval = 1000000;
    
    unsigned long long int cycles, prev_cycles, delta_cycles, dyn, prev_dyn, delta_dyn, slp, prev_slp, delta_slp;
    
	r = syscall(VAR_SYSCALL, TOTAL_ACT_ENERGY, &dyn);
	r = syscall(VAR_SYSCALL, TOTAL_SLP_ENERGY, &slp);
	r = syscall(VAR_SYSCALL, TOTAL_CYCLES, &cycles);


	while (1) {
		
        usleep(interval);
        
		prev_cycles = cycles;
		prev_dyn = dyn;
		prev_slp = slp;	  
		
		r = syscall(VAR_SYSCALL, TOTAL_ACT_ENERGY, &dyn);
		r = syscall(VAR_SYSCALL, TOTAL_SLP_ENERGY, &slp);
		r = syscall(VAR_SYSCALL, TOTAL_CYCLES, &cycles);
		
		delta_cycles = cycles - prev_cycles;
		delta_dyn = dyn - prev_dyn;
		delta_slp = slp - prev_slp;		

		printf("dyn=%llu, slp=%llu, cycles=%llu\n", delta_dyn, delta_slp, delta_cycles);

	}

	


}
