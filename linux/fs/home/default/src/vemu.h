#ifndef errors_h
#define errors_h

#define VAR_SYSCALL_READ 	378
#define VAR_SYSCALL_WRITE 	379
#define ACT_ERRORS		0xFC0
#define VEMU_KILL		0xFD0
void vemu_enable_errors(int idx) {
	unsigned long long int err_status;
	err_status = idx;
	syscall(VAR_SYSCALL_WRITE, ACT_ERRORS, &err_status);
}

void vemu_disable_errors() {
	unsigned long long int err_status;
	err_status = 0;
	syscall(VAR_SYSCALL_WRITE, ACT_ERRORS, &err_status);
}

void vemu_kill() {
	unsigned long long int err_status;
	err_status = 1;
	syscall(VAR_SYSCALL_WRITE, VEMU_KILL, &err_status);	
}

#endif
