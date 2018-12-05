# /bin/bash
sdcard="/media/colourfate/fab6f5be-0e8c-461a-8d77-1300fb62d5f6/"

if [ -e $sdcard ]
then
	echo "Find sdcard"
	#make zImage ARCH=arm CROSS_COMPILE=arm-linux-
	#mkimage -A arm -O linux -T kernel -C none -a 0x30007FC0 -e 0x30008000 -n 'Linux-4.10' -d arch/arm/boot/zImage arch/arm/boot/uImage
	rm arch/arm/boot/uImage
	make uImage s5pv210-x210.dtb LOADADDR=0x30007FC0 ARCH=arm CROSS_COMPILE=arm-linux-
	sudo cp arch/arm/boot/uImage arch/arm/boot/dts/s5pv210-x210.dtb $sdcard
else
	echo "Please insert sdcard"
fi
