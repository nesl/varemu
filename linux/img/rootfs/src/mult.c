#include <stdio.h>
#include <unistd.h>

#define VAR_SYSCALL_READ 	378
#define VAR_SYSCALL_WRITE 	379


#define ACT_ERRORS			0xFC0

int main()
{
	
	unsigned long long int enable_errors;
	
	enable_errors = 1;
	syscall(VAR_SYSCALL_WRITE, ACT_ERRORS, &enable_errors);
	
    int a,b,c;
    long int n = 1000000;
    int y = 40000;
    int i; 
    for (i = 0; i < n; i++) {
        a = rand() % y;
        a = 2*(a-y/2);
        b = rand() % y;
        b = 2*(b-y/2);
        c = a*b;
    }
    
    //enable_errors = 0;
   	//syscall(VAR_SYSCALL_WRITE, ACT_ERRORS, &enable_errors);


    return 0;
}
