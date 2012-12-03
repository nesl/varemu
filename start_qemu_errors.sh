qemu-1.2.0/arm-softmmu/qemu-system-arm -k /usr/share/qemu/keymaps/en-us -M versatilepb -m 128M -kernel linux/img/zImage -initrd linux/img/rootfs.img.gz -append "root=/dev/ram rdinit=/sbin/init console=ttyAMA0" -singlestep -variability power_model_data/instance_01.txt -nographic 


