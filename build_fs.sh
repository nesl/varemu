#/bin/sh

cd linux
if [ ! -f buildroot-2017.02.2.tar.gz ]; then
  wget https://buildroot.org/downloads/buildroot-2017.02.2.tar.gz;
  tar xzf buildroot-2017.02.2.tar.gz;
  cp buildrootconfig buildroot-2017.02.2/.config;
fi
cd buildroot-2017.02.2/;
make
cd ../..
