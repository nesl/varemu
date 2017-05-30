#/bin/sh

cp -r varemu/* qemu-linaro/
cd qemu-linaro
./configure --disable-kvm --enable-debug --enable-debug-tcg --target-list=arm-softmmu --audio-drv-list=alsa
make CFLAGS=-DVEMU
cd ..
