
 void Wifi_init()
 {
  //WiFi初始化函数
  pinMode(CH_PD,OUTPUT);//设置ESP8266的CH_PD引脚
  Serial.begin(9600);//开启WIFI串口
  digitalWrite(CH_PD,LOW);//CH_PD置低电平，ESP8266不工作
  delay(50);//延时100毫秒
  digitalWrite(CH_PD,HIGH);//CH_PD置高电平，ESP8266工作
  delay(100);//延时200毫秒,等待ESP8266输出完初始化信息
  SerialClean();//清空缓冲区
  DbgSerial.println("[OK]  Wifi Init!");//WIFI初始化完成
 }


 bool Wifi_OK()
 {
  //测试WIFI是否正常
  Serial.println("ATE0");//关闭回显
  delay(50);//延时50毫秒,等待ESP8266反应
  SerialClean();//清空串口缓冲区
  Serial.println("AT");//发送AT，正常应回复OK
  delay(50);//延时50毫秒,等待ESP8266反应
  if(Serial.available()==6)
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Check OK!");//Wifi正常
    return(true);//返回真
  }
  else
  {
    SerialClean();//清空缓冲区
    return(false);//返回假
  }
 }


 void Wifi_SetConfigMode()
 {
  //设置WIFI为配置模式
  DbgSerial.println("[Info]  Start Confinging!");//开始设置插座
  Serial.println("AT+CWMODE_CUR=2");//设置为AP模式
  delay(200);//延时100毫秒,等待ESP8266反应
  if((Serial.available()==6))
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Set AP Mode OK!");//Wifi设置AP正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.println("AT+CIPMUX=1");//设置多路连接模式
  delay(200);//延时200毫秒,等待ESP8266反应
  if(Serial.available()==6)
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Set Muti-Connect OK!");//Wifi设置多路连接正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.println("AT+CWSAP_CUR=\"SmartPlug\",\"\",5,0,1,0");//设置SSID和密码
  delay(2000);//延时2000毫秒,等待ESP8266反应
  if(Serial.available()==6)
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Set SSID And Password OK!");//Wifi设置热点正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.println("AT+CIPSERVER=1,8080");//在8080端口建立服务器
  delay(200);//延时2000毫秒,等待ESP8266反应
  if(Serial.available()==6)
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Create Server OK!");//Wifi建立服务器正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  DbgSerial.println("[Info]  Server Waiting For Connect!");//Wifi等待连接
 }


 bool Client_isConnected()
 {
  //WIFI客户端是否连接
  if(Serial.available()==7)
  {
    //如果串口缓冲区为7
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  Client Is Connected!");//客户端已经连接
    delay(100);//延时100毫秒
    return(true);
  }
  
  return(false);
 }


 void Wifi_SetWorkMode()
 {
  //设置WIFI为工作模式
  Serial.println("AT+CWMODE_CUR=1");//设置为Station模式
  delay(100);//延时100毫秒,等待ESP8266反应
  if((Serial.available()==6))
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Set Station Mode OK!");//Wifi设置Station正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.println("AT+CIPMUX=0");//设置单路连接模式
  delay(100);//延时100毫秒,等待ESP8266反应
  if(Serial.available()==6)
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Set Singal-Connect OK!");//Wifi设置单路连接正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.print("AT+CWJAP_CUR=\"");//设置连接AP
  for(int i=0;i<31;i++)
    {
      if(EEPROM.read(i+1)!=255)
      {
        Serial.print(char(EEPROM.read(i+1)));
      }
    }
  Serial.print("\",\"");
  for(int i=0;i<63;i++)
    {
      if(EEPROM.read(i+33)!=255)
      {
        Serial.print(char(EEPROM.read(i+33)));
      }
    }
  Serial.println("\"");
  delay(15000);
  if(Serial.available()==35)
  {
    //如果串口缓冲区为35
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Connect OK!");//Wifi连接AP正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.println("AT+CIPMODE=1");//设置透传模式
  delay(100);//延时100毫秒,等待ESP8266反应
  if(Serial.available()==6)
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  WIFI Set Unvarnished Transmission Mode OK!");//Wifi设置透传模式正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.println("AT+CIPSTART=\"TCP\",\"183.230.40.33\",80");//连接服务器
  delay(500);//延时500毫秒,等待ESP8266反应
  if(Serial.available()==15)
  {
    //如果串口缓冲区为15
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  Connect To Server OK!");//连接服务器正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
  Serial.println("AT+CIPSEND");//开启透传
  delay(100);//延时100毫秒,等待ESP8266反应
  if(Serial.available()==9)
  {
    //如果串口缓冲区为6
    SerialClean();//清空缓冲区
    DbgSerial.println("[OK]  Set Unvarnished Transmission OK!");//Wifi设置透传模式正常
  }
  else
  {
    SerialClean();//清空缓冲区
    Wifi_SoftwareError();//WIFI软件错误
  }
 }


 void SendData()
 {
  //发送温度湿度亮度数据
  Serial.println("POST /devices/25769898/datapoints?type=3 HTTP/1.1");//HTTP请求头，以方式3发送
  Serial.println("api-key: wAUFepED3=uuggijWIrqkd0CVWE=");//ONENET提供的apikey
  Serial.println("Host:api.heclouds.com");//主机：api.heclouds.com"
  Serial.println("Content-Length:31");//传输长度
  Serial.println();//body和header之间空行
  Serial.print("{\"Temp\":");
  Serial.print(int(temperature));//发送温度
  Serial.print(",\"");
  Serial.print("Hum\":");
  Serial.print(int(humidity));//发送湿度
  Serial.print(",\"");
  Serial.print("Light\":");
  Serial.print(light_Percent);//发送亮度
  Serial.println("}");
 }


 void GetRelayData()
 {
  //获取继电器数据
  Serial.println("GET /devices/25769898/datapoints?datastream_id=Relay HTTP/1.1");//GET数据
  Serial.println("api-key: wAUFepED3=uuggijWIrqkd0CVWE=");//api
  Serial.println("Host:api.heclouds.com");//主机
  Serial.println();//空行
  
 }

void SendRelayData()
{
  //发送继电器数据
  Serial.println("POST /devices/25769898/datapoints?type=3 HTTP/1.1");//HTTP请求头，以方式3发送
  Serial.println("api-key: wAUFepED3=uuggijWIrqkd0CVWE=");//ONENET提供的apikey
  Serial.println("Host:api.heclouds.com");//主机：api.heclouds.com"
  Serial.println("Content-Length:11");//传输长度
  Serial.println();//body和header之间空行
  Serial.print("{\"Relay\":");
  Serial.print(digitalRead(2));//发送温度
  Serial.println("}");
  
}

 void SerialClean()
 {
   //清空串口缓冲区，因为1.0版本后的Serial.flush()函数不再有此功能
   while(Serial.available()>0)
   {
     //如果仍有缓冲区数据
     Serial.read();//清空一个字节数据
   }
 }
