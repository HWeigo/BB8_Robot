/**
  ******************************************************************************
  * File Name          : Task_control.c
  * Description        : 接受指令层和驱动层的反馈数据，完成PID等算法处理，并
	* 调用驱动函数控制电机、舵机等模块。
	* 
	* 舵机控制：
	* 舵机PWM接在普通定时器TIM2上，对应始时钟为APB2:48MHZ。舵机驱动需要产生一个
	* 20ms的脉冲信号，采用预分频480-1，自动重载周期2000-1。
	* PWM频率 = 48 * 10^6 / 480 / 2000 = 50HZ。
	* 注意，舵机工作电压为4.8V-6V。
	* 信号线 -> PA2
	*
	*
	* 直流电机控制：
	* 利用TIM4参数PWM，驱动频率为10kHz（推荐），采用预分频48-1，自动重载周期100-1。
	* 控制范围0-100。
	* TIM4_CH1 -> PD12
	* TIM4_CH2 -> PD13
	* TIM4_CH3 -> PD14
	* TIM4_CH4 -> PD15
  ******************************************************************************
*/

#include "includes.h"

//模式选择
#define balanceCar
//#define BB8

//舵机型号选择 
#define SG90
//#define MG995

//SG90舵机:150停转 <150顺时针, >150逆时针�
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995舵机：147停止 <147顺时针 >147逆时针
#ifdef MG995
uint16_t DutyCycle_STOP = 147; //零点标定似乎与电池电量（电压）有关
#endif 


//360度模拟舵机速度控制 x>0：顺时针 x<0：逆时针
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, DutyCycle_STOP - x)
#define setServoStop() __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, DutyCycle_STOP)

uint16_t rotateSpeed = 0; //范围+-100
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
			//直立Pwm
			Balance_Pwm = - balance(gyroYAngle,gyroYspeed);
			//速度pwm
			Velocity_Pwm = velocity(encoderL,encoderR,0);
			//三环相加pwm
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


//PID控制参数
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

/**********P比例环******************/
float balance(float Angle,float Gyro)
{  
   float Gap;
	 int balance;
	 Gap=Angle-Zero_point;                     //求出平衡的角度中值 和机械相关
		balance=Balance_Kp*Gap+Gyro*Balance_Kd;   //计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}

/*********PI速度环******************/
float velocity(int32_t encoder_left,int32_t encoder_right,float Target_Velocity)
{  

		Movement=-Target_Velocity/Velocity_Ref;
		
		Encoder_Least =(encoder_left+encoder_right)-0;                    //获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.75f;		                                                //一阶低通滤波器       
		Encoder += (float)Encoder_Least*0.25f;	                                    //一阶低通滤波器    
		Encoder_Integral +=Encoder;                                       //积分出位移 积分时间：10ms
		Encoder_Integral = Encoder_Integral-Movement;                       //接收遥控器数据，控制前进后退
		if(Encoder_Integral>8000)  	Encoder_Integral=8000.0;             //积分限幅
		if(Encoder_Integral<-8000)	Encoder_Integral=-8000.0;              //积分限幅	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //速度控制	
//		if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //电机关闭后清除积分
	  return Velocity;
}

