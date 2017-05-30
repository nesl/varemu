#/bin/sh

tar xjf qemu-linaro-13-03.28.tar.bz2
cp -r varemu/* qemu-linaro/
cd qemu-linaro
./configure --disable-kvm --enable-debug --enable-debug-tcg --target-list=arm-softmmu --audio-drv-list=alsa
make CFLAGS=-DVEMU
cd ..
