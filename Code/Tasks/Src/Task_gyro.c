/**
  ******************************************************************************
  * File Name          : Task_gyro.c
  * Description        : ����imu���ݣ�����ת��Ϊ��ʵ���ݣ�������ȼ���
	* ѡ��USART1��Ϊ���ڡ�
	* RX -> PA9
	* TX -> PA10
  ******************************************************************************
*/

#include "includes.h"


extern uint8_t gyroIsReady;
extern uint8_t gyroBuffer[];
//float gyroXAngle,gyroYAngle,gyroZAngle;
//float gyroXspeed,gyroYspeed,gyroZspeed;
//float gyroXacceleration,gyroYacceleration,gyroZacceleration;
//float temperature;

uint8_t debug_flag=0;
void Task_GRYO(void const * argument)
{
  while(1)
  {
		//debug_flag = humanDetect();
		
//    if(gyroIsReady && !sumCheck())
//		{
////			printf("Processing gyro.");      //���h��dubug�����⣬δ���
//			switch(gyroBuffer[1])
//			{
//				case 0x51:
//				{
//					gyroXacceleration = ((short)(gyroBuffer[3]<<8)|gyroBuffer[2])/32768.0f*16.0f;
//					gyroYacceleration = ((short)(gyroBuffer[5]<<8)|gyroBuffer[4])/32768.0f*16.0f;
//					gyroZacceleration = ((short)(gyroBuffer[7]<<8)|gyroBuffer[6])/32768.0f*16.0f;
//					
//					temperature = ((short)(gyroBuffer[9]<<8|gyroBuffer[8]))/340.0f + 36.25f;
//				}break;
//				case 0x52:
//				{
//					gyroXspeed = ((short)(gyroBuffer[3]<<8)|gyroBuffer[2])/32768.0f*2000.0f;
//					gyroYspeed = ((short)(gyroBuffer[5]<<8)|gyroBuffer[4])/32768.0f*2000.0f;
//					gyroZspeed = ((short)(gyroBuffer[7]<<8)|gyroBuffer[6])/32768.0f*2000.0f;
//				}break;
//				case 0x53:
//				{
//					gyroXAngle = ((short)(0x00|gyroBuffer[3]<<8|gyroBuffer[2]))/32768.0f*180.0f;
//					gyroYAngle = ((short)(0x00|gyroBuffer[5]<<8|gyroBuffer[4]))/32768.0f*180.0f;
//					gyroZAngle = ((short)(0x00|gyroBuffer[7]<<8|gyroBuffer[6]))/32768.0f*180.0f;
//				}break;
//				default:
//					Error_Handler();
//			}
//					
//			gyroIsReady = 0;
//		}
		osDelay(30);//������������ʱ��Ϊ100��ԭΪ10
  }
}

//У��λ���飬Sum=0x55+0x52+wxH+wxL+wyH+wyL+wzH+wzL+TH+TL
uint8_t minus;
uint8_t sumCheck(void)
{
		minus = gyroBuffer[10];
		for(int i=0;i<10;i++)
		{
			minus -= gyroBuffer[i];
		}
		return minus;
}
