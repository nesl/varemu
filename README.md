# VarEMU: An emulation testbed for Variability-Aware Software

## Build instructions

### VarEMU

* Download VarEMU source code.
* Download and extract [qemu-1.2](http://wiki.qemu-project.org/download/qemu-1.2.0.tar.bz2):
* Copy VarEMU extensions into QEMU
* Configure QEMU
* Build QEMU with VarEMU extensions

<pre>
wget https://github.com/nesl/varemu/archive/master.zip
unzip master.zip
cd varemu-master/
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
export ARCH=arm
export CROSS_COMPILE=arm-none-linux-gnueabi-
cp -r linux-3.6.3-variability/* linux-3.6.3/
cd linux-3.6.3/
make versatile_defconfig
make menuconfig
</pre>

* Under "Kernel features", select "Use the ARM EABI"
* Build kernel

<pre>
make
cd ..
</pre>

### Buildroot

<pre>
wget http://buildroot.uclibc.org/downloads/buildroot-2012.11.tar.bz2
cd buildroot-2012.11
</pre>

* Under Target Architecture, Select ARM Little Endian
* Under Target Architecture Variant, Select arm926t
* Under Toolchain -> Toolchain type, Select External Toolchain
* Under Toolchain -> Toolchain origin, Select pre-installed toolchain
* Under Toolchain -> Toolchain path, type in path for CodeBench toolchain (e.g. /opt/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/)
* Under Filesystem images, select "cpio the root filesystem". Under compression method, select gzip
* Under System Configuration -> Port to run a getty, use "tty0"

### Busybox (Alternative to Buildroot) 

* Download [Busybox](http://www.busybox.net/downloads/busybox-1.20.2.tar.bz2)

<pre>
wget http://www.busybox.net/downloads/busybox-1.20.2.tar.bz2
tar xjf busybox-1.20.2.tar.bz2
cd busybox-1.20.2/
make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- defconfig
make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- menuconfig
</pre>

* Under "Busybox Settings –> Build Options", select "Build as a static binary"
* Build busybox

<pre>
make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- install
cd ..
</pre>

### Root filesystem with demo apps

* Build demo apps
* Make fs image

<pre>
cd fs/src/
make
cd ../..
make image
cd ..
</pre>

## Running VarEMU

* From the main varemu directory, start QEMU with VarEMU options

<pre>
qemu-1.2.0/arm-softmmu/qemu-system-arm -k /usr/share/qemu/keymaps/en-us -M versatilepb -m 128M -kernel linux/img/zImage -initrd linux/img/rootfs.img.gz -append "root=/dev/ram rdinit=/sbin/init console=ttyAMA0" -singlestep -variability power_model_data/instance_01.txt -nographic 
</pre>


