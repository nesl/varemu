# VarEMU: An emulation testbed for Variability-Aware Software

## Build instructions

### VarEMU

* Get VarEMU source code.
* Download and extract [qemu-1.2](http://wiki.qemu-project.org/download/qemu-1.2.0.tar.bz2):
* Copy VarEMU extensions into QEMU
* Configure QEMU
* Build QEMU with VarEMU extensions

<pre>
git clone git://github.com/nesl/varemu.git
cd varemu
wget http://wiki.qemu-project.org/download/qemu-1.2.0.tar.bz2
tar xjf qemu-1.2.0.tar.bz2
cp -r varemu/* qemu-1.2.0/
cd qemu-1.2.0
./configure --enable-sdl --disable-kvm --enable-debug --enable-debug-tcg --target-list=arm-softmmu
make CFLAGS=-DVEMU
cd ..
</pre>

### Linux kernel for versatilepb

* Pre-requisite: Download and install [CodeBench for ARM](https://sourcery.mentor.com/sgpp/lite/arm/portal/subscription?@template=lite) (Linux version)
* Download and extract Linux kernel [version 3.6.3](http://www.kernel.org/pub/linux/kernel/v3.0/linux-3.6.3.tar.bz2)
* Apply VarEMU kernel extensions to kernel source tree
* Configure Linux kernel for versatilepb platform

<pre>
cd linux
wget http://www.kernel.org/pub/linux/kernel/v3.0/linux-3.6.3.tar.bz2
tar xjf linux-3.6.3.tar.bz2
cp -r linux-3.6.3-variability/* linux-3.6.3/
cd linux-3.6.3/
make versatile_defconfig
make menuconfig
</pre>

* Under "Kernel features", select "Use the ARM EABI"
* Build kernel

<pre>
make
</pre>
