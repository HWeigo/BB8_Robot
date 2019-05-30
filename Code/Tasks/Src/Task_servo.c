/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : 舵机控制（普通优先级）
	* 舵机信号线 -> PA0
	* 舵机PWM接在普通定时器TIM2上，对应始时钟为APB2:48MHZ。舵机驱动需要产生一个
	* 20ms的脉冲信号，采用预分频480-1，自动重载周期2000-1。
	* PWM频率 = 48 * 10^6 / 480 / 2000 = 50HZ。
	* 注意，舵机工作电压为4.8V-6V。
	* 信号线 -> PA0
  ******************************************************************************
*/

#include "includes.h"


//舵机型号选择 
//#define SG90
#define MG995


//SG90 :150停转 <150顺时针, >150逆时针�
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995：147停止 <147顺时针 >147逆时针
#ifdef MG995
uint16_t DutyCycle_STOP = 150; //零点标定似乎与电池电量（电压）有关
#endif 

//360度模拟舵机速度控制 x>0：顺时针 x<0：逆时针
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP - x)
#define setServoStop() __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP)

uint16_t rotateSpeed = 0; //范围+-100
ServoCmd_e ServoCmd = Mode_1;
extern uint8_t key;

	/**************************************************************************/
extern int16_t servoSwerve;
extern int16_t speed;

//#define setMotor1Speed(x) \
//if(x>=0)\
//{\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 100*x/128);\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);\
//}\
//else\
//{\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, -100*x/128);\
//}
void setMotor1Speed(int16_t speed_f)
{
	if(speed_f >= 0)
	{
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 100*speed_f/128);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
	}
	else
	{	
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, -100*speed_f/128);
	}
}

//#define setMotor2Speed(x) \
//if(x<=0)\
//{\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, -x/128*100);\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);\
//}\
//else\
//{\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);\
//	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, x/128*100);\
//}
void setMotor2Speed(int16_t speed_f)
{
	if(speed_f >= 0)
	{
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 100*speed_f/128);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
	}
	else
	{	
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, -100*speed_f/128);
	}
}
	/**************************************************************************/


void Task_Servo(void const * argument)
{
	/**************************************************************************/
	/*
	*车轮PWM驱动控制，暂时放在servo.c
	*利用TIM4参数PWM，驱动频率为10kHz（推荐），采用预分频48-1，自动重载周期100-1。
	*控制范围0-100。
	* TIM4_CH1 -> PD12
	* TIM4_CH2 -> PD13
	* TIM4_CH3 -> PD14
	* TIM4_CH4 -> PD15
	*/
	
	setMotor1Speed(0);
	setMotor2Speed(0);
	
	/**************************************************************************/
	
	setServoSpeed(0);

  while(1)
  {
		
	/**************************************************************************/
		if(PS2_RedLight())
		{
			setMotor1Speed(speed);
			setMotor2Speed(speed);
	//		setMotor2Speed(50);
	//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 100);
	//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
		}
	/**************************************************************************/		
		
		if(key == 0 && servoSwerve == 0)
		{
			setServoSpeed(0);
		}
		else
		{
			setServoSpeed(0);
			switch(key)
			{
				case PSB_TRIANGLE:
				{
					setServoSpeed(20);
					osDelay(200);
					setServoSpeed(-20);
					osDelay(200);
					setServoSpeed(0);
				}break;
				default:
					break;
			}
			if(PS2_RedLight())
			{
				setServoSpeed(servoSwerve/127*25);
			}
		}
//		switch(ServoCmd)
//		{
//			case Mode_1:
//			{
//				setServoSpeed(-20);
//				osDelay(3000);
//				ServoCmd = Stop;
//			}break;
//			case Stop:
//			{
//				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP);
//			}break;
//			default:
//				break;
//		}

    osDelay(50);
  }
  /* USER CODE END Task_Servo */
}
