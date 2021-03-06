# VarEMU: An emulation testbed for Variability-Aware Software

## Quick download, build and run

These commands download, build, and run the Linux testbed for VarEMU.

<pre>
wget https://github.com/nesl/varemu/archive/master.zip
unzip master.zip
cd varemu-master/
./build_all.sh
./start_qemu.sh
</pre>


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
./configure --disable-kvm --enable-debug --enable-debug-tcg --target-list=arm-softmmu
make CFLAGS=-DVEMU
cd ..
</pre>

### Buildroot

<pre>
cd linux
wget https://buildroot.org/downloads/buildroot-2017.02.2.tar.gz
tar xzf buildroot-2017.02.2.tar.gz
cd buildroot-2017.02.2/
make menuconfig
</pre>

* Under Load Configuration, select "buildrootconfig" in the linux directory
* Select any additional apps needed under Package Selection for the Target
* Save an Alternate Configuration file -> "buildroot-2017.02.2/.config"
* Add your apps and custom files under the "fs" directory. Buildroot will run the "prepare_fs.sh" script to copy these files before preparing the filesystem image

<pre>
make
</pre>


### Linux kernel for versatilepb

* Pre-requisite: Download and install [CodeBench for ARM](https://sourcery.mentor.com/sgpp/lite/arm/portal/subscription?@template=lite) (Linux version, tested with 2014.05-29)
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





## Running VarEMU

* From the main varemu directory, start QEMU with VarEMU options

<pre>
qemu-linaro/arm-softmmu/qemu-system-arm -M versatilepb -m 256M -kernel linux/linux-3.6.3/arch/arm/boot/zImage -initrd linux/buildroot-2017.02.2/output/images/rootfs.cpio.gz -append "root=/dev/ram rdinit=/sbin/init" -singlestep -variability power_model_data/45nm_nc.txt -net nic -net user  -nographic
</pre>


## VarEMU Power Monitor


* Start QEMU with a monitor socket by adding <code> -qmp unix:qmp-sock_01,server</code> to the command line
* Start the VarEMU Python Monitor

<pre>
python qemu-linaro/QMP/vemu-power.py qmp-sock_01
</pre>
