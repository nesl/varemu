#include "uart_put.h"
#include "hw_inc.h"
#include "DriverLib.h"
#include <string.h>

int uart_is_initialized = 0;

#define ALPHANUMS "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"

/** converts an integer to a string, using a base of 10 by default.
 *
 * if you NULL out the output buffer it will return the expected
 * output string length anyway.
 */
int itoa(int number, char* out, int base) {
    int t, count;
    char *p, *q;
    char c;

    p = q = out;
    if (base < 2 || base > 36) base = 10;

    do {
        t = number;
        number /= base;
        if (out) *p = ALPHANUMS[t+35 - number*base];
        p++;
    } while (number);

    if (t < 0) {
         if (out) *p = '-';
         p++;
    }
    count = p-out;
    if (out) {
        *p-- = '\0';
        while(q < p) {
            c = *p;
            *p-- = *q;
            *q++ = c;
        }
    }
    return count;
}

// ======================== UART PUTS EMULATION (QEMU) ======================
/* initialize uart puts */
static void _uart_init( void ){
	/* Enable the UART.  */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	/* Set GPIO A0 and A1 as peripheral function.  They are used to output the
	UART signals. */
	GPIODirModeSet( GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_DIR_MODE_HW );

	/* Configure the UART for 8-N-1 operation. */
	UARTConfigSet( UART0_BASE, mainBAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE );
	uart_is_initialized = 1;
}
/* write a string char by char to uart */
int _uart_puts(char *ptr, int len) {
  int todo;
  for (todo = 0; todo < len; todo++) {
		while( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF );
		HWREG( UART0_BASE + UART_O_DR ) = *ptr++;	  
  }
  return len;
}



/* write a string followed by an ascii integer with sign */
static void uart_puts_num( const char* message, int num ){
	// send the string message to UART
	int i;
	for( i=0; i<strlen(message); i++){
		/* Send the characters. */
		while( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF );
		HWREG( UART0_BASE + UART_O_DR ) = message[i];
	}

	// send the number to UART
	char buffer[64];
	char count = 0;
	int mag = num;
	if(num < 0){
		while( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF );
		HWREG( UART0_BASE + UART_O_DR ) = '-';
		mag = -1*num;
	}
	if(mag == 0){
		while( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF );
		HWREG( UART0_BASE + UART_O_DR ) = '0';
	}else{
		// first put them into buffer (this will be reverse order)
		while( mag>0 ){
			buffer[count++] = (mag%10)+48;
			mag /= 10;
		}

		// then send them to UART, reversing the order so it's correct
		int i;
		for( i=count-1; i>=0; i--){
			while( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF );
			HWREG( UART0_BASE + UART_O_DR ) = buffer[i];
		}
	}

	// send carriage return to UART
	while( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF );
	HWREG( UART0_BASE + UART_O_DR ) = '\n';
	while( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF );
	HWREG( UART0_BASE + UART_O_DR ) = '\r';
}

int uputs(char *s)
{
	if(!uart_is_initialized) {
		_uart_init();
	}
	_uart_puts(s, strlen(s));
}

int uputi(int i)
{
	char buf[32];
	char *p;
	itoa(i, buf, 10);
	uputs(buf);
}

int uputx(int i)
{
	char buf[32];
	char *p;
	itoa(i, buf, 16);
	uputs("0x");
	uputs(buf);
}
