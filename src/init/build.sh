#!/bin/sh

set -e

# Load common properties and functions in the current script.
. ../common.sh

echo "*** BUILD INIT BEGIN ***"

gcc --sysroot ../work/sysroot main.cpp

cp a.out ../minimal_rootfs/a.out

# We go back to the main MLL source folder.
cd $SRC_DIR

echo "*** BUILD INIT END ***"
