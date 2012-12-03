#ifndef errors_h
#define errors_h

#define VAR_SYSCALL_READ 	378
#define VAR_SYSCALL_WRITE 	379
#define ACT_ERRORS		0xFC0

void enable_errors() {
	unsigned long long int err_status;
	err_status = 1;
	syscall(VAR_SYSCALL_WRITE, ACT_ERRORS, &err_status);
}

void disable_errors() {
	unsigned long long int err_status;
	err_status = 0;
	syscall(VAR_SYSCALL_WRITE, ACT_ERRORS, &err_status);
}

#endif
