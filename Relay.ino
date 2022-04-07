
 void Relay_init()
 {
  //继电器初始化函数
  pinMode(Relay,OUTPUT);//设置继电器引脚为输出模式
  SetRelay(false);//关闭继电器
  DbgSerial.println("[OK]  Relay Init!");//继电器初始化完成
 }


 void SetRelay(bool state)
 {
  //继电器控制函数
  digitalWrite(Relay,state);//控制继电器引脚电平
 }
