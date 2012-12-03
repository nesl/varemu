# VarEMU: An emulation testbed for Variability-Aware Software

## VarEMU Build instructions

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
</pre>
