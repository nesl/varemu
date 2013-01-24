#include <asm/io.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <mach/platform.h>
#include <asm/uaccess.h>
#include <misc/variability.h>

MODULE_LICENSE("Dual BSD/GPL");

uint8_t *base_pointer = 0;
uint64_t vemu_reg_read(int offset);
void vemu_reg_write(int offset, uint64_t val);

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
    iowrite32((uint32_t)(val>>32), (uint32_t*)(base_pointer + offset + 4));
}

void vemu_save_error_status(vemu_status * proc)
{
    proc->vemu_error_status = vemu_reg_read(VARMOD_ERRORS_EN);   
}
EXPORT_SYMBOL(vemu_save_error_status);

void vemu_restore_error_status(vemu_status * proc)
{
    vemu_reg_write(VARMOD_ERRORS_EN, proc->vemu_error_status);
}
EXPORT_SYMBOL(vemu_restore_error_status);


asmlinkage long sys_var_reg_read(int offset, uint64_t __user *dest) {

    uint64_t val = 0;
    
    if ((offset > 0x1000) | (offset % 8)) {
        return -EFAULT;
    }
    
    val = vemu_reg_read(offset);    
	
    if (copy_to_user(dest, &val, sizeof(val)))
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
    return 0;
}

static void variability_exit(void)
{
    iounmap(base_pointer);
}

module_init(variability_init);
module_exit(variability_exit);

