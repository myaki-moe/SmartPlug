
 bool SmartPlug_isConfig()
 {
  //读取是否设置插座
  if(EEPROM.read(0)==1)
  {
    //如果EEPROM第0位为1
    DbgSerial.println("[Info]  SmartPlug Has Configed!");//智能插座已经被设置
    return(true);//返回已经设置
  }
  else
  {
    DbgSerial.println("[Info]  SmartPlug Has Not Configed!");//智能插座没有被设置
    SetLed_Blue(true);//亮蓝灯，等待配置
    return(false);//返回没有设置
  }
 }


 void SaveSsidAndPassWord()
 {
  //在EEPROM中存储SSID和密码
  delay(200);
  SerialClean();
  while(Serial.read()!=58){}//读取到冒号时
  for(int i=0;i<31;i++)
  {
    //WiFi的ssid最长为32个字符
    EEPROM.write(i+1, Serial.read());//写入EEPROM
  }
  DbgSerial.println("[OK]  Get And Saved SSID!");//已经取得SSID并保存
  delay(2000);//延时2秒
  while(Serial.read()!=58){}//读取到冒号时
  for(int i=0;i<63;i++)
  {
    //WiFi的密码最长为64个字符
    EEPROM.write(i+33, Serial.read());//写入EEPROM
  }
  DbgSerial.println("[OK]  Get And Saved Password!");//已经取得密码并保存
  EEPROM.write(0, 1);//写入EEPROM第0位为1,标记已配置
  DbgSerial.print("Your SSID:  ");//读出SSID
    for(int i=0;i<31;i++)
    {
      if(EEPROM.read(i+1)!=255)
      {
        DbgSerial.print(char(EEPROM.read(i+1)));
      }
    }
    
    DbgSerial.print("Your PassWord:  ");//读出密码
    for(int i=0;i<63;i++)
    {
      if(EEPROM.read(i+33)!=255)
      {
        DbgSerial.print(char(EEPROM.read(i+33)));
      }
    }
  ReBoot();//重启系统
 }


 void CleanEEPROM()
 {
  //删除EEPROM数据
    for (int i = 0 ; i < EEPROM.length() ; i++)
    {
    EEPROM.write(i, 0);//所有位写0
    }
 }
