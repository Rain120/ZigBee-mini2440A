#include "Sensor.h"
#include <ioCC2530.h>

#if 0

#define HAL_ADC_REF_115V 0x00
#define HAL_ADC_DEC_256  0x20
#define HAL_ADC_CHN_TEMP 0x0e


int8 readTemp(void)
{
  static uint16 reference_valtage;
  static uint8 bCalibrate = TRUE;
  
  uint16 value;
  int8 temp;
  
  ATEST = 0x01;    // 使能温度传感器
  TR0 |= 0x01;     // 连接温度传感器
  
  ADCIF = 0;
  ADCCON3 = (HAL_ADC_REF_115V | HAL_ADC_DEC_256 | HAL_ADC_CHN_TEMP);
  
  while(!ADCIF)
  {
    ;
  }
  ADCIF = 0;
  value = ADCL;
  value |= ((uint16)ADCH) << 8;
  
  value >>= 4;
  
  if (bCalibrate)   // 记录第一次读取的温度值，用于校正温度数据。
  {
    reference_valtage = value;
    bCalibrate = FALSE;
  }
  
  temp = 22 + ((value - reference_valtage)/4);   // 温度校正函数
  return temp;
  
  
}
//getVddvalue
unsigned int getVddvalue(void)
{
   unsigned int value;
   unsigned char tmpADCCON3 = ADCCON3;
   ADCIF = 0;
   ADCCON3 = (ADC_REF_115V|ADC_DEC_064|ADC_CHN_VDD3);
   while(!ADCIF);
   value = ADCH;
   ADCCON3 = tmpADCCON3;
   return (value);
}
#endif

#if 1

/*********************************************************
*  函数名称： Sensor_Init
*  功能    ： 初始化 cc2530 内部温度传感器
*  参数列表： 无
*  返回值  ： 无
**********************************************************/
void Sensor_Init(void)
{     
    TR0    =   0x01; //连接温度传感器到SOC__ADC；
    ATEST  = 0x01; //选择温度传感器；
}

/*********************************************************
*  函数名称： ADC_Init
*  功能    ： ADC初始化
*  参数列表： 无
*  返回值  ： 无
**********************************************************/
void ADC_Init11(void)
{
   ADCCON3 |=  (0xe << 0) | (0x3 << 4);   //512抽取率，温度传感器；
   ADCCON1 |= (3<<4);   // 手动开启模式；0011 0000
   ADCCON1 |= (1<<6);   //1时开启AD转换；  0100 0000
}

/*********************************************************
*  函数名称： GetTemperature
*  功能    ： 获取芯片内温度值
*  参数列表： 无
*  返回值  ： 温度值，float 类型
**********************************************************/
/*float GetTemperature(void)
{
    unsigned int Value; //高4位存放整数部分，低4位存放小数部分；
    ADC_Init();
    while(!(ADCCON1 & 0x80)); //等待转换完成；
    
    //8bit
    Value = ADCL>>4;
    Value |= (((unsigned int)ADCH)<<4);

    return  (Value-1367.5)/2.5 - 5;  
}*/

uint8 GetTemperature(void)
{
    Sensor_Init();
    ADC_Init11();
    unsigned int Value; //高4位存放整数部分，低4位存放小数部分；
    while(!(ADCCON1 & (1<<7))); //等待转换完成；
    
    //8bit
    Value = ADCL>>4;
    Value |= (((unsigned int)ADCH)<<4);

    return  (uint8)(Value-1367.5)/2.5 - 5;  
}

#endif