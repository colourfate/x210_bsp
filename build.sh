#!/bin/bash

BASEPATH=$(cd `dirname $0`; pwd)

#MFLAG="ARCH=arm CROSS_COMPILE=$BASEPATH/toolchain/4.9.3/bin/arm-linux-"
MFLAG="ARCH=arm CROSS_COMPILE=arm-linux-"
UBOOTDIR=u-boot-2017.09
LINUXDIR=linux-4.10
ROOTFSDIR=buildroot-2018.08

# sd卡设备文件
SDDEV=/dev/sdc
BL1POS=1       # BL1从1扇区开始
UBOOTPOS=49     # uboot从49扇区开始
# sd卡挂载点
SDDIR=/media/colourfate/fab6f5be-0e8c-461a-8d77-1300fb62d5f6

if [ $# == 0 ]; then
    echo -e "\n------------------------uboot------------------------\n"
    cd $UBOOTDIR
    make x210_defconfig $MFLAG
    make $MFLAG
    cd sd_fusing/
    make
    ./mkx210 ../u-boot.bin 210.bin
    cd $BASEPATH
    cp $UBOOTDIR/u-boot.bin $UBOOTDIR/sd_fusing/210.bin output/

    echo -e "\n------------------------linux------------------------\n"
    BOOTDIR=arch/arm/boot
    cd $LINUXDIR
    make x210_defconfig $MFLAG
    rm $BOOTDIR/uImage
    make uImage s5pv210-x210.dtb LOADADDR=0x30007FC0 $MFLAG
    cp $BOOTDIR/uImage $BOOTDIR/dts/s5pv210-x210.dtb $BASEPATH/output
    cd $BASEPATH

    # FIXME: build root的工具链需要单独配置
    echo -e "\n------------------------build root------------------------\n"
    cd $ROOTFSDIR
    make x210_defconfig
    #make BR2_TOOLCHAIN_EXTERNAL_PATH=$BASEPATH/toolchain/4.9.3/
    make
    cp output/images/rootfs.tar $BASEPATH/output
    cd $BASEPATH

elif [ $# == 1 ]; then
    if [ $1 == "help" ]; then
        echo -e "./build.sh [选项]\n"
        echo -e "如果没有指定选项，则编译全部工程\n"
        echo -e "clean         - 清除编译生成文件"
        echo -e "upload <dev>  - 烧写工程到sd卡，<dev>为sd卡设备文件"
        echo
    elif [ $1 == "clean" ]; then
        cd $UBOOTDIR
        make clean
        cd ..
        cd $LINUXDIR
        make clean
        cd ..
        cd $ROOTFSDIR
        make clean
        cd ..
    fi

elif [[ $# == 2 && $1 == "upload" ]]; then
    if [ $2 != $SDDEV ]; then
        echo SD卡设备有误
        exit 1
    fi
    echo -e "烧写uboot到$SDDEV，拷贝uImage和rootfs到$SDDIR!"
    cd output/
    echo 烧写bl1...
    dd iflag=dsync oflag=dsync if=210.bin of=$SDDEV seek=$BL1POS
    echo 烧写uboot...
    dd iflag=dsync oflag=dsync if=u-boot.bin of=$SDDEV seek=$UBOOTPOS
    echo 拷贝镜像...
    cp uImage s5pv210-x210.dtb rootfs.tar $SDDIR
    echo 完成，请在SD卡中将rootfs.tar解压
fi
