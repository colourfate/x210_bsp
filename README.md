## 概述
九鼎s5pv210开发板移植的bsp，平台信息如下：

| 开发板 | 九鼎x210（s5pv210） |
| --- | --- |
| bootloader | u-boot-2017.09 |
| kernel | linux-4.10.0 |
| rootfs | buildroot（busybox-1.29） |
| toolchain | arm-cortexa9-linux-gnueabihf-4.9.3 |

目前能够使用SD卡开机启动，挂载rootfs并进入命令行。

## 使用方法
**1. 配置toolchain**
[下载工具链](https://pan.baidu.com/s/1NAO1ryuMCmkyIT0lXMHEPw#list/path=%2FDVD%2FH3%2FNanoPi-NEO%2Ftoolchain&parentPath=%2FDVD%2FH3o) 
将工具链加入到环境变量并配置为`arm-linux-`，运行`arm-linux-gcc -v`可查看版本号为`4.9.3`。
==提供的工具链基于64位Linux，32位不能运行！==如不使用此处提供的工具链，也需要将本地的工具链配置为`arm-linux-`。

**2. SD卡分区**
uboot占用SD卡前10MB空间，linux内核挂载SD卡的第一分区，文件系统格式为ext4。
使用`fdisk`将SD卡分区，分区号为1，起始扇区是20480（10MB），结束扇区默认。然后将1号分区格式化为ext4文件系统。
具体分区步骤参考[我的博客](https://blog.csdn.net/Egean/article/details/84249607) 。

**3. 修改build.sh文件**
```bash
# sd卡设备文件
SDDEV=/dev/sdx
# sd卡挂载点
SDDIR=<SD卡目录>
```
打开源码根目录中的`build.sh`文件，找到以上两个变量，指定SD卡的设备文件位置和SD卡挂载点位置。

**4. 编译**
```bash
./build.sh
```
编译全部文件，其中编译`buildroot`时会从网络下载busybox等支持包，因此需==保持网络畅通==。编译完成全部文件输出到`output/`目录。

**5. 下载到SD卡**
```bash
sudo ./build.sh upload /dev/sdx
```
插入SD卡，确认SD卡设备文件并替换`sdx`，然后执行。完成后进入SD卡中将`rootfs.tar`解压。

**6. 启动**
将SD卡插入x210开发板的==SD2==插槽并启动。