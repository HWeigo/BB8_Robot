/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : ������ƣ���ͨ���ȼ���
	* ����ź��� -> PA0
	* ���PWM������ͨ��ʱ��TIM2�ϣ���Ӧʼʱ��ΪAPB2:48MHZ�����������Ҫ����һ��
	* 20ms�������źţ�����Ԥ��Ƶ480-1���Զ���������2000-1��
	* PWMƵ�� = 48 * 10^6 / 480 / 2000 = 50HZ��
	* ע�⣬���������ѹΪ4.8V-6V��
	* �ź��� -> PA2
  ******************************************************************************
*/

#include "includes.h"


//����ͺ�ѡ�� 
//#define SG90
#define MG995


//SG90 :150ͣת <150˳ʱ��, >150��ʱ��
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995��147ֹͣ <147˳ʱ�� >147��ʱ��
#ifdef MG995
uint16_t DutyCycle_STOP = 150; //���궨�ƺ����ص�������ѹ���й�
#endif 

//360��ģ�����ٶȿ��� x>0��˳ʱ�� x<0����ʱ��
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, DutyCycle_STOP - x)
#define setServoStop() __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, DutyCycle_STOP)

uint16_t rotateSpeed = 0; //��Χ+-100
ServoCmd_e ServoCmd = Mode_1;
extern uint8_t key;

/**************************************************************************/
extern int16_t servoSwerve;
extern int16_t speed;

//PID���Ʋ���
#define Zero_point 0.5f
#define Balance_Kp 10.0f
#define Balance_Kd 0.0f
#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

extern float gyroYAngle,gyroYspeed;
float Balance_Pwm=0,Velocity_Pwm,Turn_Pwm;

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
void setMotor1Speed(float speed_f)
{
	if(speed_f >= 0)
	{
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, speed_f);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
	}
	else
	{	
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, -speed_f);
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
void setMotor2Speed(float speed_f)
{
	if(speed_f >= 0)
	{
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, speed_f);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
	}
	else
	{	
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, -speed_f);
	}
}
	/**************************************************************************/


void Task_Servo(void const * argument)
{
	/**************************************************************************/
	/*
	*����PWM�������ƣ���ʱ����servo.c
	*����TIM4����PWM������Ƶ��Ϊ10kHz���Ƽ���������Ԥ��Ƶ48-1���Զ���������100-1��
	*���Ʒ�Χ0-100��
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
			setMotor1Speed(100*speed/128);
			setMotor2Speed(100*speed/128);
	//		setMotor2Speed(50);
	//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 100);
	//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
		}
	/**************************************************************************/

/*************ֱ��������**************/
		if(!PS2_RedLight())
		{
			Balance_Pwm = - MAX(MIN(balance(gyroYAngle,gyroYspeed),100),-100); 
			
			setMotor1Speed(Balance_Pwm); //��ֵ������
			setMotor2Speed(Balance_Pwm);

		}
		
		
		
		
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


/***********PDֱ����*************/
float balance(float Angle,float Gyro)
{  
   float Gap;
	 int balance;
	 Gap=Angle-Zero_point;                     //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 balance=Balance_Kp*Gap+Gyro*Balance_Kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}
/***********************************/
		
