#/bin/sh

export PATH=$PATH:`pwd`/linux/buildroot-2017.02.2/output/host/usr/bin
cd linux
if [ ! -f linux-3.6.3.tar.bz2 ]; then
  wget http://www.kernel.org/pub/linux/kernel/v3.0/linux-3.6.3.tar.bz2;
  tar xjf linux-3.6.3.tar.bz2;
  patch -t linux-3.6.3/kernel/timeconst.pl linux-3.6.3-variability/timeconst-patch
fi
export ARCH=arm
export CROSS_COMPILE=arm-none-linux-gnueabi-
cp -r linux-3.6.3-variability/* linux-3.6.3/
cd linux-3.6.3/
cp ../linuxconfig .config
make
