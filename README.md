# VarEMU: An emulation testbed for Variability-Aware Software

## Build instructions

### VarEMU

* Download VarEMU source code.
* Extract QEMU Linaro snapshot
* Copy VarEMU extensions into QEMU
* Configure QEMU
* Build QEMU with VarEMU extensions

<pre>
wget https://github.com/nesl/varemu/archive/master.zip
unzip master.zip
cd varemu-master/
tar xjf qemu-linaro-13-03.28.tar.bz2
cp -r varemu/* qemu-linaro/
cd qemu-linaro
./configure --enable-sdl --disable-kvm --enable-debug --enable-debug-tcg --target-list=arm-softmmu
make CFLAGS=-DVEMU
cd ..
</pre>

### Linux kernel for versatilepb

* Pre-requisite: Download and install [CodeBench for ARM](https://sourcery.mentor.com/sgpp/lite/arm/portal/subscription?@template=lite) (Linux version, tested with 2011.09-70)
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

* Load an alternate config file, select ../linuxconfig
* Save an alternate config file: .config

<pre>
make
cd ..
</pre>

### Buildroot

<pre>
cd linux
wget http://buildroot.uclibc.org/downloads/buildroot-2012.11.tar.bz2
tar xjf buildroot-2012.11.tar.bz2 
cd buildroot-2012.11
make menuconfig
</pre>

* Under Load an Alternate Configuration File, select "buildrootconfig" in the linux directory
* Select any additional apps needed under Package Selection for the Target
* Save an Alternate Configuration file -> "buildroot-2012.11/.config" 
* Add your apps and custom configuration files under the "fs" directory. Buildroot will run the "prepare_fs.sh" script to copy these files before preparing the filesystem image

<pre>
make
</pre>




## Running VarEMU

* From the main varemu directory, start QEMU with VarEMU options

<pre>
qemu-1.2.0/arm-softmmu/qemu-system-arm -M versatilepb -m 256M -kernel linux/linux-3.6.3/arch/arm/boot/zImage -initrd linux/buildroot-2012.11/output/images/rootfs.cpio.gz -append "root=/dev/ram rdinit=/sbin/init" -singlestep -variability power_model_data/instance_01.txt -net nic -net user -nographic
</pre>


