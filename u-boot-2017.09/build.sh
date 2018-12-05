make ARCH=arm CROSS_COMPILE=arm-linux-
arm-linux-objdump -S u-boot > u-boot.dmp
cd sd_fusing
sudo ./sd_fusing.sh /dev/sdc
cd ..

#bootargs=root=/dev/mtdblock8 rootfstype=ext4 ${console} ${meminfo} ${mtdparts}
