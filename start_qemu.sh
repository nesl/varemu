qemu-linaro/arm-softmmu/qemu-system-arm -M versatilepb -m 256M -kernel linux/linux-3.6.3/arch/arm/boot/zImage -initrd linux/buildroot-2012.11/output/images/rootfs.cpio.gz -append "root=/dev/ram rdinit=/sbin/init" -singlestep -variability power_model_data/45nm_nc.txt -net nic -net user  -nographic


