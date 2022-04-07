
 void ReadLightInput()
 {
  //获取亮度百分数函数
  int light=analogRead(Lignt_Input);//读取A0口电压
  if(light>1000)
  {
    light=1000;
  }
  if(light<50)
  {
    light=50;
  }
  light_Percent=map(1000-light,0,950,0,100);//映射到0-100范围

  
 }
