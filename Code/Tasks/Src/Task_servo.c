/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : ������ƣ���ͨ���ȼ���
	* ����ź��� -> PA0
	* ���PWM������ͨ��ʱ��TIM2�ϣ���Ӧʼʱ��ΪAPB2:16MHZ�����������Ҫ����һ��
	* 20ms�������źţ�����Ԥ��Ƶ160-1���Զ���������2000-1��
	* PWMƵ�� = 16 * 10^6 / 160 / 2000 = 50HZ��
	* ע�⣬���������ѹΪ4.8V-6V��
  ******************************************************************************
*/

#include "includes.h"


//����ͺ�ѡ�� 
#define SG90
//#define MG995


//SG90 :150ͣת <150˳ʱ��, >150��ʱ��
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995��147ֹͣ <147˳ʱ�� >147��ʱ��
#ifdef MG995
uint16_t DutyCycle_STOP = 147;
#endif 

//360��ģ�����ٶȿ��� x>0��˳ʱ�� x<0����ʱ��
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP - x);

uint16_t rotateSpeed = 0; //��Χ+-100
uint8_t tmpflag=1;
ServoCmd_e ServoCmd = Mode_1;
void Task_Servo(void const * argument)
{
  /* USER CODE BEGIN Task_Servo */
  /* Infinite loop */
  while(1)
  {
		switch(ServoCmd)
		{
			case Mode_1:
			{
				setServoSpeed(-20);
				osDelay(3000);
				ServoCmd = Stop;
			}break;
			case Stop:
			{
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP);
			}break;
			default:
				break;
		}
//		
//		if (tmpflag)
//	  {
//		  dutyCycle ++;
//		  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, dutyCycle);
//			if(dutyCycle == 147+10) tmpflag =0;
//	  }
//	  else
//	  {
//		  dutyCycle --;
//		  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, dutyCycle);
//			if(dutyCycle == 147-10) tmpflag =1;
//	  }
//		
    osDelay(400);
  }
  /* USER CODE END Task_Servo */
}