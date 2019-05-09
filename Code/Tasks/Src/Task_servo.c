/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : 舵机控制（普通优先级）
	* 舵机信号线 -> PA0
	* 舵机PWM接在普通定时器TIM2上，对应始时钟为APB2:16MHZ。舵机驱动需要产生一个
	* 20ms的脉冲信号，采用预分频160-1，自动重载周期2000-1。
	* PWM频率 = 16 * 10^6 / 160 / 2000 = 50HZ。
	* 注意，舵机工作电压为4.8V-6V。
  ******************************************************************************
*/

#include "includes.h"


//舵机型号选择 
#define SG90
//#define MG995


//SG90 :150停转 <150顺时针, >150逆时针�
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995：147停止 <147顺时针 >147逆时针
#ifdef MG995
uint16_t DutyCycle_STOP = 147;
#endif 

//360度模拟舵机速度控制 x>0：顺时针 x<0：逆时针
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP - x);

uint16_t rotateSpeed = 0; //范围+-100
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