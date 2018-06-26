#!/bin/sh

set -e

# Load common properties and functions in the current script.
. ./common.sh

echo "*** BUILD ALSA BEGIN ***"

# Prepare the work area, e.g. 'work/alsa/alsa_objects'.
echo "Preparing alsa object area. This may take a while."
rm -rf $ALSA_OBJECTS
mkdir $ALSA_OBJECTS

# Prepare the install area, e.g. 'work/alsa/alsa_installed'.
echo "Preparing alsa install area. This may take a while."
rm -rf $ALSA_INSTALLED
mkdir $ALSA_INSTALLED

# Find the alsa source directory, e.g. 'alsa-2.23' and remember it.
ALSA_SRC=`ls -d $WORK_DIR/alsa/alsa-*`

# All alsa work is done from the working area.
cd $ALSA_OBJECTS

# 'alsa' is configured to use the root folder (--prefix=) and as result all
# libraries will be installed in '/lib'. Note that on 64-bit machines BusyBox
# will be linked with the libraries in '/lib' while the Linux loader is expected
# to be in '/lib64'. Kernel headers are taken from our already prepared kernel
# header area (see xx_build_kernel.sh). Packages 'gd' and 'selinux' are disabled
# for better build compatibility with the host system.
echo "Configuring alsa."
$ALSA_SRC/configure \
  --prefix= \
  --enable-shared=yes \
  --enable-static=no \
  --with-sysroot=$KERNEL_INSTALLED/include \
  CFLAGS="$CFLAGS"

# Compile alsa with optimization for "parallel jobs" = "number of processors".
echo "Building alsa."
make -j $NUM_JOBS

# Install alsa in the installation area, e.g. 'work/alsa/alsa_installed'.
echo "Installing alsa."
make install \
  DESTDIR=$ALSA_INSTALLED \
  -j $NUM_JOBS

cd $SRC_DIR

echo "*** BUILD ALSA END ***"
