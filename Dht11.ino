
 bool Dht_OK()
 {
  //检测DHT11是否正常
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(DHTPin, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
  {    
    return(false);//DHT错误
  }
  else
  {
    DbgSerial.println("[OK]  DHT11 Check OK!");//DHT11正常
    return(true);//DHT正常
  }
 }


 void ReadTempAndHum()
 {
  //读取温度湿度函数
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(DHTPin, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
  {    
    Dht_HardwareError();//DHT11硬件错误
  }
 }
