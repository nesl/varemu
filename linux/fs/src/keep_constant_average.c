#include <stdio.h>
#include <unistd.h>

#define ABS(a)	   (((a) < 0) ? -(a) : (a))


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
	double average_power = 0;
	double target_power = 300;
	double tolerance = 10;
    int r;
    
    int offset;
    
    int interval = 1000000;
    
    unsigned long long int cycles, prev_cycles, delta_cycles, dyn, prev_dyn, delta_dyn, slp, prev_slp, delta_slp;
    
	r = syscall(VAR_SYSCALL, TOTAL_ACT_ENERGY, &dyn);
	r = syscall(VAR_SYSCALL, TOTAL_SLP_ENERGY, &slp);
	//r = syscall(VAR_SYSCALL, TOTAL_CYCLES, &cycles);


	while (1) {
		
        usleep(interval);
        
		prev_cycles = cycles;
		prev_dyn = dyn;
		prev_slp = slp;	  
		
		r = syscall(VAR_SYSCALL, TOTAL_ACT_ENERGY, &dyn);
		r = syscall(VAR_SYSCALL, TOTAL_SLP_ENERGY, &slp);
		//r = syscall(VAR_SYSCALL, TOTAL_CYCLES, &cycles);
		
		delta_dyn = dyn - prev_dyn;
		delta_slp = slp - prev_slp;	
		
		average_power = (delta_slp + delta_dyn)*1000000/(double)interval;

		printf("dyn=%llu, slp=%llu, avg=%f, target=%f, interval=%d\n", delta_dyn, delta_slp, average_power, target_power, interval);
		
		if (ABS(target_power - average_power) > tolerance) {
			if (average_power > target_power) {
				interval = interval + 10000;
			} else if (average_power < target_power) {
				interval = interval - 10000;
				if (interval < 10000) {
						
				}
			}
		}


	}

	


}
