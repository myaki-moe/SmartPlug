  
 unsigned int LastTime=0;//记录上一次按键引脚按下时间
 int number=0;//记录按键电平变化次数
 
 void Button_init()
 {
  //按键初始化函数
  pinMode(Button,INPUT);//将按键接口设置为输入
  attachInterrupt(0, Button_Push, CHANGE);//开启按键中断,中断号为0，也就是D2引脚
  DbgSerial.println("[OK]  Button Init!");//按键初始化完成
 }


 void CancelInterrupt()
 {
  //取消中断函数
  detachInterrupt(0);//取消按键中断
 }

 
 void Button_Push()
 {
  //按键中断处理函数
  if(number==0)
  {
    LastTime=millis();
    number=1;
  }
  else
  {
    number=0;
    unsigned int Time=millis()-LastTime;//用现在的时间减去上次触发中断时间
    if(Time<5000)
    {
      //如果小于5秒
      if(digitalRead(5)==0)
      {
        SetRelay(true);//打开继电器
        SetLed_Blue(true);//打开蓝色LED
        relay=true;
      }
      else
      {
        SetRelay(false);//关闭继电器
        SetLed_Blue(false);//关闭蓝色LED
        relay=true;
      }
      
    }
    else
    {
      //如果大于5秒
      CleanEEPROM();//删除EEPROM数据
      ReBoot();//重启系统
    }
   }
  }
 
