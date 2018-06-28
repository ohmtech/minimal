#!/bin/sh

set -e

./00_clean.sh
./01_get_kernel.sh
./02_build_kernel.sh
./03_get_glibc.sh
./04_build_glibc.sh

./get_alsa.sh
./build_alsa.sh

./05_prepare_sysroot.sh

cd init
./build.sh
cd ..

./06_get_busybox.sh
./07_build_busybox.sh
./08_prepare_bundles.sh
./09_generate_rootfs.sh
./10_pack_rootfs.sh
./11_generate_overlay.sh
./12_get_syslinux.sh
./12_get_systemd-boot.sh
./13_prepare_iso.sh
./14_generate_iso.sh
./15_generate_image.sh
./16_cleanup.sh
