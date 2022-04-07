
 void Debug_init()
 {
  //调试初始化函数
  DbgSerial.begin(9600);//开启调试串口
  DbgSerial.println();//发送新行
  DbgSerial.println("*****************************************************");//发送分隔符，方便辨认
  DbgSerial.println("[Info]  System Power On!");//发送接通电源信息
  DbgSerial.println("*****************************************************");//发送分隔符，方便辨认
 }


 void Wifi_HardwareError()
 {
  //Wifi硬件错误处理函数
  CancelInterrupt();//取消按键中断
  DbgSerial.println("[Error]  Wifi Hardware Error!");//发送Wifi硬件错误信息
  while(1)
  {
    //WIFI硬件故障时红色LED快闪
    SetLed_Red(true);//亮红色LED
    delay(100);//延时100毫秒
    SetLed_Red(false);//灭红色LED
    delay(100);//延时100毫秒
  }
 }


 void Dht_HardwareError()
 {
  //DHT11硬件错误处理函数
  CancelInterrupt();//取消按键中断
  DbgSerial.println("[Error]  DHT11 Hardware Error!");//发送DHT11硬件错误信息
  while(1)
  {
    //DHT11硬件故障时红色LED以中等频率闪烁
    SetLed_Red(true);//亮红色LED
    delay(500);//延时500毫秒
    SetLed_Red(false);//灭红色LED
    delay(500);//延时500毫秒
  } 
 }


                    
 void Wifi_SoftwareError()
 {
  //WIFI软件错误处理函数
  CancelInterrupt();//取消按键中断
  DbgSerial.println("[Error]  WIFI Software Error!");//发送WIFI软件错误信息
  for(int i=0;i<4;i++)
  {
    //WIFI软件件故障时红色LED以中等频率闪烁5次
    SetLed_Red(true);//亮红色LED
    delay(500);//延时500毫秒
    SetLed_Red(false);//灭红色LED
    delay(500);//延时500毫秒
  }
  ReBoot();//重启系统
 }
