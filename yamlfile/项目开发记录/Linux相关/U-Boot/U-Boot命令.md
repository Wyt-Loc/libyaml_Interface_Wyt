## U-BOOT命令手册

### 查看mmc分区数目

mmc list   // mmc 是一种通信协议，支持SPI和MMC

### 查看当前分区的mmc信息

mmc info

### 查看分区里的文件

fatls mmc 1:1 

### 将ZImage 下载到DDR的80800000处

fatload mmc 1:1 80800000 ZImage

### 将dtb读取到0x83000000处

fatload mmc 1:1 83000000 imx6ull-14x14-emmc-7-1024x600-c.dtb

### 启动内核

bootz 80800000 - 83000000
