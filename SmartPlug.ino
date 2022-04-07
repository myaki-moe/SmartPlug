/* 
 * Arduino+ESP8266+DHT11+Led+光敏电阻 实现智能云插座
 * 功能:远程开关插座&小夜灯 温度&湿度&光照读取上传云 
 * 
 * 接线:
 * D0   <->  ESP8266-RX
 * D1   <->  ESP8266-TX
 * D2   <->  Button
 * D3   <->  ESP82066-CH_PD
 * D4   <->  DHT11-DATA
 * D5   <->  Relay
 * D6   <->  NC
 * D7   <->  NC
 * D8   <->  NC
 * D9   <->  NC
 * D10  <->  DebugSerial-RX
 * D11  <->  DebugSerial-TX
 * D12  <->  Led-Blue
 * D13  <->  Led-Red
 * A0   <->  Light-Input
 */
#define Button 2
#define CH_PD 3
#define DHTPin 4
#define Relay 5
#define DbgSerial_RX 10
#define DbgSerial_TX 11
#define Led_Blue 12
#define Led_Red 13
#define Lignt_Input A0
//对所有外部接口宏定义

//#include <avr/wdt.h> 
//包含看门狗头文件

#include <EEPROM.h>
//包含EEPROM头文件

#include <SimpleDHT.h>
//包含DHT头文件
SimpleDHT11 dht11; //创建DHT11对象

#include <SoftwareSerial.h>
//包含软串口头文件
SoftwareSerial DbgSerial(DbgSerial_RX, DbgSerial_TX); //创建软件调试串口对象

void(* ReBoot)(void)=0;//定义一个指向地址为0的函数指针，用于软复位

//************************************************************************************************************************

void setup() {
  Debug_init();//初始化调试
  Wifi_init();//初始化WIFI
  Button_init();//初始化按键
  Led_init();//初始化LED
  Relay_init();//初始化继电器
  DbgSerial.println("[Info]  System Init OK!");//所有软硬件初始化完成
  DbgSerial.println("*****************************************************");//发送分隔符，方便辨认
  
//************************************************************************************************************************
  if(Dht_OK()==false)
  {
    //如果DHT11不正常
    Dht_HardwareError();//报告DHT11硬件错误
  }
  if(Wifi_OK()==false)
  {
    //如果Wifi不正常
    Wifi_HardwareError();//报告Wifi硬件错误
  }

  DbgSerial.println("[Info]  Hardware Check OK!");//所有硬件检查完成
  DbgSerial.println("*****************************************************");//发送分隔符，方便辨认
  
//************************************************************************************************************************  
 
  if(SmartPlug_isConfig()==false)
  {
    //如果没有设置插座
    Wifi_SetConfigMode();//设置WIFI为配置模式
    while(Client_isConnected()==false){}//等待客户端连接
    SaveSsidAndPassWord();//保存SSID和密码在EEPROM中
  }
  Wifi_SetWorkMode();//设置WIFI为工作模式
  //wdt_enable(WDTO_8S);//开启硬件看门狗
  DbgSerial.println("[OK]  WDT Open!");//开启看门狗
//************************************************************************************************************************  

  DbgSerial.println("[Info]  System Config OK!");//所有软硬件设置完成
  DbgSerial.println("*****************************************************");//发送分隔符，方便辨认
}


byte temperature = 0;//温度
byte humidity = 0;//湿度
int light_Percent;//光照
int num=0;//循环次数
bool relay =false;//是否发送继电器数据
void loop() {
  if(relay==true)
  {
    SendRelayData();
    relay=false;
  }
  if(num==60)
  {
    //如果循环次数达到60次
    ReadTempAndHum();//读取温度和湿度
    ReadLightInput();//读取光照
    SendData();//发送数据
    num=0;//重置循环
    DbgSerial.println("[Info]  Send Data！");//发送数据
    delay(500);//延时500毫秒
    SerialClean();//清空串口
  }
  DbgSerial.print("[Info]  ");
  GetRelayData();//获取继电器数据
  while(Serial.available()!=63){}
  SerialClean();
  while(Serial.available()!=63){}
  SerialClean();
  while(Serial.available()!=63){}
  SerialClean();
  while(Serial.available()!=63){}
  SerialClean();
  //因为arduino串口缓冲区只有64字节，所以先删除前256个无用数据
  delay(100);//延时，等待串口数据
  for(int i=0;i<15;i++)
  {
    Serial.read();//清空15个串口数据
  }
  while(Serial.read()!=58){}//读取到冒号时
  int Data=Serial.read();
  int AfterData=Serial.read();
  if(AfterData==125)
  {
    //如果接收数据的下个字为" }"
    if(Data==49)
    {
    //如果数据为"0"
    SetRelay(true);
    SetLed_Blue(true);
    DbgSerial.println("Relay_on");//开启继电器
    }
  if(Data==48)
    {
    //如果数据为"1"
    SetRelay(false);
    SetLed_Blue(false);
    DbgSerial.println("Relay_off");//关闭继电器
    }
  }
  else
  {
    DbgSerial.println("Data Error!");//数据错误
  }
  SerialClean();//清理串口
  delay(500);//延时500毫秒，防止请求太快
  //wdt_reset();//喂狗
  DbgSerial.println("[OK]  Feed Dog!");//开启看门狗
  num++;//循环数加1
}
