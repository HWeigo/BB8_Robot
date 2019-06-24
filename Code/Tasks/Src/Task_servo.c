/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : 舵机控制（普通优先级）
	* 舵机信号线 -> PA0
	* 舵机PWM接在普通定时器TIM2上，对应始时钟为APB2:48MHZ。舵机驱动需要产生一个
	* 20ms的脉冲信号，采用预分频480-1，自动重载周期2000-1。
	* PWM频率 = 48 * 10^6 / 480 / 2000 = 50HZ。
	* 注意，舵机工作电压为4.8V-6V。
	* 信号线 -> PA2
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
			setMotor1Speed(100*speed/128);
			setMotor2Speed(100*speed/128);
	//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 100);
	//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
		}
	/**************************************************************************/		
		
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
					setServoSpeed(-20);					osDelay(200);
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
  /* USER CODE END Task_Servo */
}

//PID控制参数
float Zero_point=2.0; //大时往USB方向倒，小时往陀螺仪方向倒
float Balance_Kp=5.5; //4.2 
float Balance_Kd=0.42;//0.9
float Velocity_Kp= -0.32;//0.2
float Velocity_Ki= -0.32/200;//-1.5 / 200;

//4 9.0 0.75 -0.12稳定3s
//2 6.0 0.6 -0.12原地震荡一段 往一个方向走一会 倒下
//2 8.0 0.5 -0.15向一侧震荡一段距离
//2 9.0 0.5 -0.12小速度小角度内手持感觉不错
//2 7.0 0.5 -0.12感觉某一环超调 
//2 5.3 0.38 -0.1手持各状态基本都不会震荡，往前时追不上
//0 5.5 0.38 -0.17不容易倒，但会往一个方向跑
//0 5.6 0.35 -0.18 -0.18/150 往陀螺仪方向倒
//4 5.9 0.35 -0.18 两边稳定性相似
//4 5.7 0.4 -0.22 -0.22/190 好像是因为震荡的原因翻倒 
//2 5.2 0.39 -0.22 -0.22往两边倒的情况似乎差不多，会往一个方向跑且追不上，偶尔振动
//2 5.2 0.38 -0.28 -0.28略容易往线少的方向倒 视频19:41往一倾斜后回弹过大 
//2 5.2 0.39 -0.28 -0.28kd改大效果比改小好 改：d=0.4时观察到高频振动 改：p=5.1高频震荡明显
//3 5.1 0.39 -0.28 -0.28视频20:19 往线多方向倒超过 往线少方向倒回弹至中心 怀疑标记中心偏线少的方向且超调
//Kp不能大于14 v不易过大 3.4 
//4 5.1 0.42 -0.3 -0.3喜欢往没线的地方冲刺，可以持续来回抖动
//2 5.2 0.42 -0.33 -0.33很少倒，最终会往一个方向冲刺，怀疑积分项影响太大，不断累加 效果还可以

//2 5.5 0.42 -0.27 0.27/2000开分级balance 视频22:32

/***********PD直立环*************/
float balance(float Angle,float Gyro)
{  
   float Gap;
	 int balance;
	 Gap=Angle-Zero_point;                     //===求出平衡的角度中值 和机械相关
//	 if(Gap>=-3 && Gap<=3 ) Gap*=2;
//	 if((Gap>-7 && Gap<-3)||(Gap>3 && Gap<7)) Gap*=1.5;
		balance=Balance_Kp*Gap+Gyro*Balance_Kd;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}
/***********************************/

//PID控制参数

float Velocity_Ref = 1.0;
float Velocity = 0;
int32_t Encoder_Least = 0;
float Encoder = 0;
float Movement =0;
float Encoder_Integral =0;
/*********PI速度环******************/
float velocity(int32_t encoder_left,int32_t encoder_right,float Target_Velocity)
{  


	  //=============遥控前进后退部分=======================// 
//	  if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                 //如果进入避障模式,自动进入低速模式
//    else 	                         Target_Velocity=110;                 
//		if(1==Flag_Qian)    	Movement=Target_Velocity/Flag_sudu;	         //===前进标志位置1 
//		else if(1==Flag_Hou)	Movement=-Target_Velocity/Flag_sudu;         //===后退标志位置1
//	  else  Movement=0;	
//	  if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //避障标志位置1且非遥控转弯的时候，进入避障模式
//	  Movement=-Target_Velocity/Flag_sudu;
   //=============速度PI控制器=======================//	
		Movement=-Target_Velocity/Velocity_Ref;
		
		Encoder_Least =(encoder_left+encoder_right)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.75f;		                                                //===一阶低通滤波器       
		Encoder += (float)Encoder_Least*0.25f;	                                    //===一阶低通滤波器    
		Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
		Encoder_Integral = Encoder_Integral-Movement;                       //===接收遥控器数据，控制前进后退
		if(Encoder_Integral>8000)  	Encoder_Integral=8000.0;             //===积分限幅
		if(Encoder_Integral<-8000)	Encoder_Integral=-8000.0;              //===积分限幅	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //===速度控制	
//		if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //===电机关闭后清除积分
	  return Velocity;
}

/****************************************************/
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
