# 基于Ardunio和ESP8266智能插座设计方案

## 项目概述

这个项目是我在2016年刚学习Arduino时参加比赛写的一个简单项目，现在上传上来供大家吐槽

我也不知道这个平台的接口还能不能用（唉）

反正这个项目以后是不会更新了（咕咕咕咕咕咕咕咕咕咕咕咕咕咕咕）




---

---



# 以下为当时写的介绍

## 目  录
1. 概述

2. 系统结构分析

3. 硬件设计

4. 软件设计

5. 总结

6. 参考资料

---

## 1.概述
### 1.1项目概述

​	本项目开发的是一款基于Ardunio和ESP8266的智能插座。用户使用手机安卓客户端连接ONENET平台查看温湿度和光照数据并发送指令控制继电器。插座以Arduino  PRO  MINI开发板为核心，通过单总线读取DHT11温湿度传感器，通过A/D转换读取光敏电阻，并通过ESP8266无线模块连接OneNet物联网平台，上传温湿度和光照数据，接收指令控制插座开关。

---

## 2.系统结构分析
### 2.1系统结构组成

​	本系统由五部分构成，分别是：安卓客户端、核心模块、通信模块、检测模块、控制模块。

​	其中，安卓客户端由E4A语言编写，核心模块为ARDUINO PRO MINI；通信模块为ESP8266-01，通过串口与核心模块通信；检测模块为DHT11温湿度传感器、光敏电阻和按键，通过IO口连接核心模块；控制模块为电磁继电器和两个发光二极管，通过IO口连接核心模块。

---

### 2.2 系统的基本功能

1. 通过安卓客户端连接连接通信模块创建的WIFI热点并上传SSID和密码。

2. 核心模块通过通信模块以HTTP协议连接ONENET并上传温湿度和光照数据。

3. 核心模块通过通信模块以HTTP协议连接ONENET并获取继电器开关值，控制继电器IO。

4. 通过安卓客户端以HTTP协议连接ONENET并查看温湿度和光照数据。

5. 通过安卓客户端以HTTP协议连接ONENET并上传继电器控制数据。

---

### 2.3系统的工作流程

系统的工作流程分为两部分，单片机部分和手机部分。

#### 单片机部分：

1. 核心模块读取EEPROM，查看是否已配置。

2. 若未配置，则通过串口发送AT指令向通信模块开启AP热点，并在8080端口创建服务器，等待手机连接。

3. 手机连接热点后，将手机APP发送的SSID和密码保存在EEPROM中，系统重启。

4. 若已配置，则通过串口发送AT指令向通信模块连接WIFI和ONENET服务器，循环读取继电器的控制数据。

5. 每隔60秒发送温湿度和光照数据到ONENET服务器。

#### 手机部分:
1. 若未配置，则手机连接通信模块的AP热点，APP连接服务器，发送SSID和密码。

2. 若已配置，则通过HTTP连接ONENET并显示温湿度和光照数据，用户操作按键时，上传继电器控制数据。

---

## 3.硬件设计
### 3.1核心模块

​		核心模块为ARDUINO PRO MINI开发板，Arduino ProMini的处理器核心是ATmega328，同时具有14路数字输入/输出口（其中6路可作为PWM输出），6路模拟输入，一个晶体谐振，一个复位按钮。有两个版本：一种是工作在3.3V和8MHz时钟，另一种是工作在5V和16MHz时钟，本项目使用第二个版本。

​		核心模块与其他模块连线如下：

	D0   <->  ESP8266-RX
	D1   <->  ESP8266-TX
	D2   <->  Button
	D3   <->  ESP82066-CH_PD
	D4   <->  DHT11-DATA
	D5   <->  Relay
	D6   <->  NC
	D7   <->  NC
	D8   <->  NC
	D9   <->  NC
	D10  <->  DebugSerial-RX
	D11  <->  DebugSerial-TX
	D12  <->  Led-Blue
	D13  <->  Led-Red
	A0   <->  Light-Input

---

### 3.2通信模块

通信模块为ESP8266-01

​		ESP8266 是一个完整且自成体系的 WiFi 网络解决方案，能够独立运行，也可以作为 slave 搭载于其他 Host 运行。

​		ESP8266 在搭载应用并作为设备中唯一的应用处理器时，能够直接从外接闪存中启动。内置的高速缓冲存储器有利于提高系统性能，并减少内存需求。

​		另外一种情况是，无线上网接入承担 WiFi 适配器的任务时，可以将其添加到任何基于微控制器的设计中，连接简单易行，只需通过 SPI/SDIO 接口或中央处理器 AHB 桥接口即可。

​		ESP8266 强大的片上处理和存储能力，使其可通过 GPIO 口集成传感器及其他应用的特定设备，实现了最低前期的开发和运行中最少地占用系统资源。

​		ESP8266 高度片内集成，包括天线开关balun、电源管理转换器，因此仅需极少的外部电路，且包括前端模块在内的整个解决方案在设计时将所占PCB空间降到最低。

​		装有ESP8266的系统表现出来的领先特征有：节能VoIP在睡眠/唤醒模式之间的快速切换、配合低功率操作的自适应无线电偏置、前端信号的处理功能、故障排除和无线电系统共存特性为消除蜂窝/蓝牙/DDR/LVDS/LCD干扰。

---

### 3.3传感器模块

​		传感器模块由三部分组成：DHT11、光敏电阻和按键。

​		DHT11 是广州奥松有限公司生产的一款湿温度一体化的数字传感器。该传感器包括一个电阻式测湿元件和一个 NTC 测温元件，并与一个高性能 8 位单片机相连接。通过单片机等微处理器简单的电路连接就能够实时的采集本地湿度和温度。DHT11 与单片机之间能采用简单的单总线进行通信，仅仅需要一个I/O 口。传感器内部湿度和温度数据 40Bit 的数据一次性传给单片机，数据采用校验和方式进行校验，有效的保证数据传输的准确性。DHT11 功耗很低，5V 电源电压下，工作平均最大电流 0.5mA。

---

### 3.4 控制模块

​		控制模块由两部分组成：继电器和LED

​		继电器是具有隔离功能的自动开关元件，广泛应用于遥控、遥测、通讯、自动控制、机电一体化及电 力电子设备中，是最重要的控制元件之一。 

​    	继电器一般都有能反映一定输入变量（如电流、电压、功 率、阻抗、频率、温度、压力、速度、光等）的感应机构（输入部分）；有能对被控电路实现“通”、“ 断”控制的执行机构（输出部分）；在继电器的输入部分和输出部分之间，还有对输入量进行耦合隔离， 功能处理和对输出部分进行驱动的中间机构（驱动部分）。 

​		作为控制元件，概括起来，继电器有如下 几种作用： 

	1. 扩大控制范围。例如，多触点继电器控制信号达到某一定值时，可以按触点组的不同 形式，同时换接、开断、接通多路电路。 
	
	2. 放大。例如，灵敏型继电器、中间继电器等，用一个很微小的控制量，可以控制很大功率的电路。 
	
	3. 综合信号。例如，当多个控制信号按规定的形式输入 多绕组继电器时，经过比较综合，达到预定的控制效果。 
	
	4. 自动、遥控、监测。例如，自动装置上 的继电器与其他电器一起，可 以组成程序控制线路，从而实现自动化运行。

   ​		发光二极管简称为 LED。由含 镓（Ga）、 砷（As）、 磷（P）、 氮（N）等的 化合物制成。当电子与空穴复合时能辐射出 可见光，因而可以用来制成发光二极管。在电路及仪器中作为指示灯，或者组成文字或数字 显示。砷化镓二极管发红光，磷化镓二极管发绿光，碳化硅二极管发黄光，氮化镓二极管发蓝光。因化学 性质又分有机发光二极管OLED和无机发光二极管LED。

---

### 3.5 外围电路

​		由于ESP8266需要3.3v电源，而直流电源为5v，故需要AMS1117-3.3稳压芯片。

​		单片机的引脚输出电流过小，因此需要S8050三极管放大。

---

## 4.软件设计

### 4.1 单片机程序设计

### SmartPlug.ino

主程序，包括头文件引用和宏定义

### Button.ino

 `void Button_init()` 

初始化按键IO口和注册中断

 `void CancelInterrupt()` 

取消按键中断

 `void Button_Push()` 

按键中断处理函数

### Debug.ino

 `void Debug_init()` 

初始化软件调试串口

 `void Wifi_HardwareError()` 

Wifi硬件错误处理函数

 `void Dht_HardwareError()` 

DHT11硬件错误处理函数

 `void Wifi_SoftwareError()` 

Wifi软件错误处理函数

### DHT11.ino

 `bool Dht_OK()` 

检测DHT11是否正常

 `void ReadTempAndHum()` 

读取温度湿度函数

### EEPROM.ino

 `bool SmartPlug_isConfig()` 

读取是否设置插座

 `void SaveSsidAndPassWord()` 

在EEPROM中存储SSID和密码

 `void CleanEEPROM()` 

删除EEPROM数据

### LED.ino

 `void Led_init()` 

LED初始化函数

 `void SetLed_Blue(bool state)` 

蓝色LED控制函数

 `void SetLed_Red(bool state)` 

红色LED控制函数

### LightInput.ino

 `void ReadLightInput()` 

获取亮度百分数函数

### Relay.ino

 `void Relay_init()` 

继电器初始化函数

 `void SetRelay(bool state)` 

继电器控制函数

### WIFI.ino

 `void Wifi_init()` 

WiFi初始化函数

 `bool Wifi_OK()` 

测试WIFI是否正常

 `void Wifi_SetConfigMode()` 

设置WIFI为配置模式

 `bool Client_isConnected()` 

WIFI客户端是否连接

 `void Wifi_SetWorkMode()` 

设置WIFI为工作模式

 `void SendData()` 

发送温度湿度亮度数据

 `void GetRelayData()` 

获取继电器数据

 `void SendRelayData()` 

发送继电器数据

 `void SerialClean()` 

清空串口缓冲区，因为1.0版本后的`Serial.flush()`函数不再有此功能

---

### 4.2 安卓程序设计

### icon.png

安卓程序图标

### blx.keystore

安卓程序签名文件

### SmartPlug.e4a

安卓主程序，使用E4A编写（咱也没办法啊，当时就是不会JAVA采用垃圾E4A的）

### 发布版_智能插座设置.apk

已经编译好的安卓程序

---

## 5.总 结

### 5.1系统实现的功能

1. 远程插座控制

2. 温湿度上传

---

### 5.2 系统存在的不足

1. 因为不可避免的网络延迟，导致实时性不强。

2. 智能程度不高

3. 安卓程序兼容性不强

4. 内存占用过高

5. 稳定性不高

---

## 6.参考资料

http://www.asciima.com/

http://www.geek-workshop.com/thread-4345-1-1.html

http://open.iot.10086.cn/bbs/forum.php?mod=viewthread&tid=363&extra=page%3D1%26filter%3Dreply%26orderby%3Dreplies

https://open.iot.10086.cn/bbs/thread-977-1-1.html

http://blog.csdn.net/t5bcss/article/details/8273212

http://blog.csdn.net/zhangxiaoxiang/article/details/765082

http://www.cnblogs.com/Mysterious/p/4816583.html

http://open.iot.10086.cn/bbs/forum.php?mod=viewthread&tid=410&extra=page%3D2%26filter%3Dreply%26orderby%3Dreplies

https://open.iot.10086.cn/doc/art261.html#68

https://www.cnblogs.com/ranyonsue/p/5984001.html

http://blog.csdn.net/robert_tina/article/details/78919356

http://blog.csdn.net/qq_29219435/article/details/78480872

http://www.cnblogs.com/Mysterious/p/4816482.html

http://playground.arduino.cc/Code/AvailableMemory

http://blog.csdn.net/y511374875/article/details/73511728

http://blog.csdn.net/Chuxin126/article/details/78304177
