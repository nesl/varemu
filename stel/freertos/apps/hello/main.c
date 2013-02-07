#include "FreeRTOS.h"
#include "hw_inc.h"
#include "DriverLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <uart_put.h>
#include "vemu.h"



void prvSetupHardware( void )
{
    /* If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
    a workaround to allow the PLL to operate reliably. */
    if( DEVICE_IS_REVA2 )
    {
        SysCtlLDOSet( SYSCTL_LDO_2_75V );
    }

	/* Set the clocking to run from the PLL at 50 MHz */
	SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ );

}


int main( void )
{
	
	vemu_regs curr, prev, delta;
	vemu_regs *cp, *pp, *tp, *dp;
	
	
	prvSetupHardware();
	
	cp = &curr;
	pp = &prev;
	dp = &delta;
	
	unsigned long long cycles = 0;
	
	while(1) {
		tp = pp;
		pp = cp;
		cp = tp;	
		
	
		vemu_read_state(cp);		
		cycles = vemu_read_reg(TOTAL_CYCLES);		
		
		vemu_delta(dp, cp, pp);	
		
		
		uputs("Cycles: "); 
		uputi(cp->total_cycles);
		uputs("     ");
		uputs("Delta: "); 
		uputi(dp->total_cycles);
		uputs("     ");		
		uputs("Single read: ");
		uputi(cycles);
		uputs("\n");
		
		
	}

	return 0;

}
