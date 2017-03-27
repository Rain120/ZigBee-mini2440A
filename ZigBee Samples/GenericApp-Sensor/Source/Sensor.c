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
  
  ATEST = 0x01;    // ʹ���¶ȴ�����
  TR0 |= 0x01;     // �����¶ȴ�����
  
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
  
  if (bCalibrate)   // ��¼��һ�ζ�ȡ���¶�ֵ������У���¶����ݡ�
  {
    reference_valtage = value;
    bCalibrate = FALSE;
  }
  
  temp = 22 + ((value - reference_valtage)/4);   // �¶�У������
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
*  �������ƣ� Sensor_Init
*  ����    �� ��ʼ�� cc2530 �ڲ��¶ȴ�����
*  �����б� ��
*  ����ֵ  �� ��
**********************************************************/
void Sensor_Init(void)
{     
    TR0    =   0x01; //�����¶ȴ�������SOC__ADC��
    ATEST  = 0x01; //ѡ���¶ȴ�������
}

/*********************************************************
*  �������ƣ� ADC_Init
*  ����    �� ADC��ʼ��
*  �����б� ��
*  ����ֵ  �� ��
**********************************************************/
void ADC_Init11(void)
{
   ADCCON3 |=  (0xe << 0) | (0x3 << 4);   //512��ȡ�ʣ��¶ȴ�������
   ADCCON1 |= (3<<4);   // �ֶ�����ģʽ��0011 0000
   ADCCON1 |= (1<<6);   //1ʱ����ADת����  0100 0000
}

/*********************************************************
*  �������ƣ� GetTemperature
*  ����    �� ��ȡоƬ���¶�ֵ
*  �����б� ��
*  ����ֵ  �� �¶�ֵ��float ����
**********************************************************/
/*float GetTemperature(void)
{
    unsigned int Value; //��4λ����������֣���4λ���С�����֣�
    ADC_Init();
    while(!(ADCCON1 & 0x80)); //�ȴ�ת����ɣ�
    
    //8bit
    Value = ADCL>>4;
    Value |= (((unsigned int)ADCH)<<4);

    return  (Value-1367.5)/2.5 - 5;  
}*/

uint8 GetTemperature(void)
{
    Sensor_Init();
    ADC_Init11();
    unsigned int Value; //��4λ����������֣���4λ���С�����֣�
    while(!(ADCCON1 & (1<<7))); //�ȴ�ת����ɣ�
    
    //8bit
    Value = ADCL>>4;
    Value |= (((unsigned int)ADCH)<<4);

    return  (uint8)(Value-1367.5)/2.5 - 5;  
}

#endif