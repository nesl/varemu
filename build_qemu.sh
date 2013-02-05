#/bin/sh

cp -r varemu/* qemu-linaro/
cd qemu-linaro
make CFLAGS=-DVEMU
cd ..

