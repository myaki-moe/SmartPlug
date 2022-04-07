
 void Led_init()
 {
  //LED初始化函数
  pinMode(Led_Blue,OUTPUT);//设置LED引脚为输出模式
  pinMode(Led_Red,OUTPUT);//设置LED引脚为输出模式
  SetLed_Blue(false);//熄灭蓝色LED
  SetLed_Red(false);//熄灭红色LED
  DbgSerial.println("[OK]  LED Init!");//LED初始化完成
 }


 void SetLed_Blue(bool state)
 {
  //蓝色LED控制函数
  digitalWrite(Led_Blue,!state);//控制LED引脚电平
 }


 void SetLed_Red(bool state)
 {
  //红色LED控制函数
  digitalWrite(Led_Red,state);//控制LED引脚电平
 }
 
