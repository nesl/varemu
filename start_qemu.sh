qemu-0.15.1/arm-softmmu/qemu-system-arm -k /usr/share/qemu/keymaps/en-us -M versatilepb -m 128M -kernel linux/img/zImage -initrd linux/img/rootfs.img.gz -append "root=/dev/ram rdinit=/sbin/init" -serial stdio -singlestep -variability power_model_data/instance_$1.txt -qmp unix:./qmp-sock_$1,server
