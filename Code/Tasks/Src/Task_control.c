/**
  ******************************************************************************
  * File Name          : Task_control.c
  * Description        : ����ָ����������ķ������ݣ����PID���㷨������
	* ���������������Ƶ���������ģ�顣
	* 
	* ������ƣ�
	* ���PWM������ͨ��ʱ��TIM2�ϣ���Ӧʼʱ��ΪAPB2:48MHZ�����������Ҫ����һ��
	* 20ms�������źţ�����Ԥ��Ƶ480-1���Զ���������2000-1��
	* PWMƵ�� = 48 * 10^6 / 480 / 2000 = 50HZ��
	* ע�⣬���������ѹΪ4.8V-6V��
	* �ź��� -> PA2
	*
	*
	* ֱ��������ƣ�
	* ����TIM4����PWM������Ƶ��Ϊ10kHz���Ƽ���������Ԥ��Ƶ48-1���Զ���������100-1��
	* ���Ʒ�Χ0-100��
	* TIM4_CH1 -> PD12
	* TIM4_CH2 -> PD13
	* TIM4_CH3 -> PD14
	* TIM4_CH4 -> PD15
  ******************************************************************************
*/

#include "includes.h"

//ģʽѡ��
#define balanceCar
//#define BB8

//����ͺ�ѡ�� 
#define SG90
//#define MG995

//SG90���:150ͣת <150˳ʱ��, >150��ʱ��
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995�����147ֹͣ <147˳ʱ�� >147��ʱ��
#ifdef MG995
uint16_t DutyCycle_STOP = 147; //���궨�ƺ����ص�������ѹ���й�
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


#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))
#define MINMAX(value, min, max) (value) = ((value) < (min)) ? (min) : ((value) > (max) ? (max) : (value))

int isFirstEnter=1;
extern float gyroYAngle,gyroYspeed;
extern int32_t encoderL,encoderR;
extern int16_t speed;
float Balance_Pwm=0,Velocity_Pwm=0,Turn_Pwm, Total_Pwm=0;
void Task_Control(void const * argument)
{
	

	
	setMotor1Speed(0);
	setMotor2Speed(0);
	
	/**************************************************************************/
	
	setServoSpeed(0);

  while(1)
  {

	/**************************** BB8 MODE ************************************/
#ifdef BB8
		if(PS2_RedLight())
		{
			setMotor1Speed(50*speed/128);
			setMotor2Speed(50*speed/128);
		}
#endif
	/**************************************************************************/		

	/*********************** SELF-BALANCING MODE ******************************/
#ifdef balanceCar
		if(!PS2_RedLight())
		{
			//ֱ��Pwm
			Balance_Pwm = - balance(gyroYAngle,gyroYspeed);
			//�ٶ�pwm
			Velocity_Pwm = velocity(encoderL,encoderR,0);
			//�������pwm
			if(isFirstEnter==1)
			{
				Velocity_Pwm=0;
				isFirstEnter=0;
			}
			else
			{			
			Total_Pwm = Velocity_Pwm +  Balance_Pwm;
			MINMAX(Total_Pwm,-100,100);
			}
			setMotor1Speed(Total_Pwm); 
			setMotor2Speed(Total_Pwm);
		}
#endif
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
				/*********** Press triangle ***********/
				case PSB_TRIANGLE:
				{
					setServoSpeed(20);
					osDelay(200);
					setServoSpeed(-20);					
					osDelay(200);
					setServoSpeed(0);
				}break;
				
				/************ Press circle ************/
				case PSB_CIRCLE:
				{
					setServoSpeed(20);
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

		
    osDelay(35);
  }

}


//PID���Ʋ���
float Zero_point=2.0; 
float Balance_Kp=5.5; 
float Balance_Kd=0.42;
float Velocity_Kp= -0.32;
float Velocity_Ki= -0.32/200;

float Velocity_Ref = 1.0;
float Velocity = 0;
int32_t Encoder_Least = 0;
float Encoder = 0;
float Movement =0;
float Encoder_Integral =0;

/**********P������******************/
float balance(float Angle,float Gyro)
{  
   float Gap;
	 int balance;
	 Gap=Angle-Zero_point;                     //���ƽ��ĽǶ���ֵ �ͻ�е���
		balance=Balance_Kp*Gap+Gyro*Balance_Kd;   //����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

/*********PI�ٶȻ�******************/
float velocity(int32_t encoder_left,int32_t encoder_right,float Target_Velocity)
{  

		Movement=-Target_Velocity/Velocity_Ref;
		
		Encoder_Least =(encoder_left+encoder_right)-0;                    //��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.75f;		                                                //һ�׵�ͨ�˲���       
		Encoder += (float)Encoder_Least*0.25f;	                                    //һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral = Encoder_Integral-Movement;                       //����ң�������ݣ�����ǰ������
		if(Encoder_Integral>8000)  	Encoder_Integral=8000.0;             //�����޷�
		if(Encoder_Integral<-8000)	Encoder_Integral=-8000.0;              //�����޷�	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //�ٶȿ���	
//		if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //����رպ��������
	  return Velocity;
}

