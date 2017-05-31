FROM 32bit/ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get -y dist-upgrade
RUN apt-get install -y wget \
  build-essential \
  zip \
  unzip \
  python \
  pkg-config \
  zlib1g-dev \
  libasound2-dev \
  libglib2.0-dev \
  libfdt-dev \
  libpixman-1-dev \
  cpio \
  rsync \
  bc
RUN mkdir /varemu && \
  cd /varemu && \
  wget https://github.com/nesl/varemu/archive/master.zip && \
  unzip master.zip && \
  cd varemu-master && \
  sh build_all.sh

CMD cd /varemu/varemu-master && sh start_qemu.sh
