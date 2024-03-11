# stm32_camera项目介绍
* 实现STM32拍照功能
* 相册功能
  * 可以在相册内进行查看拍好的照片
* 白平衡设置
  * 自动
  * 太阳
  * 阴天
  * 办公室
  * 家里
* 色度设置
* 亮度设置
* 对比度设置
* 特效设置
  * 普通模式
  * 负片
  * 黑白
  * 偏红色
  * 偏绿色
  * 偏蓝色
  * 复古
* 相片保存在TF卡中

## 硬件介绍
### 器件型号
* 主控芯片：stm32f407ve
* 摄像头：ov7670
* TFT屏幕：ILI9341、9320、9325、9328、9341
* 触摸：xpt2046
### 硬件连接
#### TFT屏幕
* 数据口PB0-15
* TFT-CS  PE0
* TFT-BL PE2   背光控制
* TFT-RST PE4
* TFT-RS  PC4
* TFT-WR  PC2
* TFT-RD  PC0

#### 摄像头
* 数据口 为PA口  A8-A15
* SCL    PD3
* SDA    PD4
* CS	   PD7 片选信号(OE)
* WRST   PD5 写指针复位
* RRST   PD6 读指针复位
* RCLK   PD1 读数据时钟
* WREN   PC13 写入FIFO使能
* VSY	   PD0 同步信号检测IO

#### TFT触摸
* AD_DCLK  PC5
* AD_CS    PC6
* AD_DIN   PC7
* AD_OUT   PC3 读取触摸返回数据
* AD_PEN   PC1

## 软件介绍
* 文件系统：petit Fatfs

## 项目制作
* 毕设咨询、购买实物、毕设指导
* 打开下方连接咨询：
* https://docs.qq.com/doc/DVm51bXRXS2FGZHds