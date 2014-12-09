#include <asm/io.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <mach/platform.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <misc/variability.h>

MODULE_LICENSE("Dual BSD/GPL");

uint8_t *base_pointer = 0;
uint64_t vemu_reg_read(int offset);
void vemu_read_state(vemu_state * target);
void vemu_reg_write(int offset, uint64_t val);

vemu_state vemu_state_pin;
vemu_state vemu_state_pout;
vemu_state vemu_state_sys;
vemu_state vemu_state_tmp;

void vemu_read_state(vemu_state * target)
{
    if (base_pointer == 0)
        return;
	iowrite32(1, (uint32_t*)(base_pointer + READ_CMD));
	memcpy_fromio(target->array32, (uint32_t *)base_pointer, VEMU_STATE_N_VARS*8);
	
}

uint64_t vemu_reg_read(int offset) {

    uint32_t lo = 0;
    uint32_t hi = 0;
    uint64_t val = 0;
    
    if (base_pointer == 0)
        return 0;

    lo = ioread32((uint32_t*)(base_pointer + offset));
    hi = ioread32((uint32_t*)(base_pointer + offset + 4));
    val = hi;
    val = val<<32;
    val += lo;
        
    return val;
} 

void vemu_reg_write(int offset, uint64_t val) {
    if (base_pointer == 0)
        return;
    iowrite32((uint32_t)val, (uint32_t*)(base_pointer + offset));
    //iowrite32((uint32_t)(val>>32), (uint32_t*)(base_pointer + offset + 4));
}

void vemu_add_delta(vemu_state * target, vemu_state * new, vemu_state * old)
{
	int i;
	for (i = 0; i < VEMU_STATE_N_VARS - 3; i++) // Don't mess with error_state, F, and V
	{
		target->array64[i] += new->array64[i] - old->array64[i];
	}
}

void vemu_process_in()
{
	vemu_read_state(&vemu_state_pin);
	vemu_add_delta(&vemu_state_sys, &vemu_state_pin, &vemu_state_pout);
    vemu_reg_write(SET_FAULTS, current->vemu.variables.fault_status);	
}
EXPORT_SYMBOL(vemu_process_in);

void vemu_process_out()
{
	vemu_read_state(&vemu_state_pout);
	vemu_add_delta(&(current->vemu), &vemu_state_pout, &vemu_state_pin);
    current->vemu.variables.fault_status = vemu_state_pout.variables.fault_status;   
    current->vemu.variables.frequency = vemu_state_pout.variables.frequency;   
    current->vemu.variables.voltage = vemu_state_pout.variables.voltage;   

}
EXPORT_SYMBOL(vemu_process_out);

asmlinkage long sys_var_reg_read(int offset, uint64_t __user *dest) {
  
    vemu_state *src_ptr;
    vemu_state *dest_ptr;
    
    dest_ptr = (vemu_state *)dest;
    
    if (offset & READ_HW) {
		src_ptr = &vemu_state_tmp;
        vemu_read_state(src_ptr);
    }
    else if (offset & READ_PROC) {
		vemu_process_out();
		src_ptr = &(current->vemu);
		vemu_process_in();
	} 
	else if (offset & READ_SYS) {
		src_ptr = &vemu_state_sys;
	}
	else {
		return -EFAULT;
	}	
		
    if (copy_to_user((void *)dest_ptr, (void *)src_ptr, sizeof(vemu_state)))
        return -EFAULT;     
        
    return 0;
}

asmlinkage long sys_var_reg_write(int offset, uint64_t __user *val) {
    
   	uint64_t myval;
   
    if ((offset > 0x1000) | (offset % 8)) {
        return -EFAULT;
    }
 
 	if (copy_from_user(&myval, val, sizeof(myval)))
                return -EFAULT;
        
    vemu_reg_write(offset, myval);
    
	return 0;
    
}

static int __init variability_init(void)
{
    base_pointer = ioremap(VARMOD_BASE, 0x1000);
    printk("Variability module init, Base Pointer=%p\n", base_pointer);
	memset(	&vemu_state_pin, 0, sizeof(vemu_state));
	memset(	&vemu_state_pout, 0, sizeof(vemu_state));	
	memset(	&vemu_state_sys, 0, sizeof(vemu_state));
	memset(	&vemu_state_tmp, 0, sizeof(vemu_state));	
    return 0;
}

static void variability_exit(void)
{
    iounmap(base_pointer);
}

module_init(variability_init);
module_exit(variability_exit);

