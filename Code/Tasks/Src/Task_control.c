/**
  ******************************************************************************
  * File Name          : Task_control.c
  * Description        : ½ÓÊÜÖ¸Áî²ãºÍÇý¶¯²ãµÄ·´À¡Êý¾Ý£¬Íê³ÉPIDµÈËã·¨´¦Àí£¬²¢
	* µ÷ÓÃÇý¶¯º¯Êý¿ØÖÆµç»ú¡¢¶æ»úµÈÄ£¿é¡£
	* 
	* ¶æ»ú¿ØÖÆ£º
	* ¶æ»úPWM½ÓÔÚÆÕÍ¨¶¨Ê±Æ÷TIM2ÉÏ£¬¶ÔÓ¦Ê¼Ê±ÖÓÎªAPB2:48MHZ¡£¶æ»úÇý¶¯ÐèÒª²úÉúÒ»¸ö
	* 20msµÄÂö³åÐÅºÅ£¬²ÉÓÃÔ¤·ÖÆµ480-1£¬×Ô¶¯ÖØÔØÖÜÆÚ2000-1¡£
	* PWMÆµÂÊ = 48 * 10^6 / 480 / 2000 = 50HZ¡£
	* ×¢Òâ£¬¶æ»ú¹¤×÷µçÑ¹Îª4.8V-6V¡£
	* ÐÅºÅÏß -> PA2
	*
	*
	* Ö±Á÷µç»ú¿ØÖÆ£º
	* ÀûÓÃTIM4²ÎÊýPWM£¬Çý¶¯ÆµÂÊÎª10kHz£¨ÍÆ¼ö£©£¬²ÉÓÃÔ¤·ÖÆµ48-1£¬×Ô¶¯ÖØÔØÖÜÆÚ100-1¡£
	* ¿ØÖÆ·¶Î§0-100¡£
	* TIM4_CH1 -> PD12
	* TIM4_CH2 -> PD13
	* TIM4_CH3 -> PD14
	* TIM4_CH4 -> PD15
  ******************************************************************************
*/

#include "includes.h"

//Ä£Ê½Ñ¡Ôñ
#define balanceCar
//#define BB8

//¶æ»úÐÍºÅÑ¡Ôñ 
#define SG90
//#define MG995

//SG90¶æ»ú:150Í£×ª <150Ë³Ê±Õë, >150ÄæÊ±Õë±
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995¶æ»ú£º147Í£Ö¹ <147Ë³Ê±Õë >147ÄæÊ±Õë
#ifdef MG995
uint16_t DutyCycle_STOP = 147; //Áãµã±ê¶¨ËÆºõÓëµç³ØµçÁ¿£¨µçÑ¹£©ÓÐ¹Ø
#endif 


//360¶ÈÄ£Äâ¶æ»úËÙ¶È¿ØÖÆ x>0£ºË³Ê±Õë x<0£ºÄæÊ±Õë
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, DutyCycle_STOP - x)
#define setServoStop() __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, DutyCycle_STOP)

uint16_t rotateSpeed = 0; //·¶Î§+-100
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
			//Ö±Á¢Pwm
			Balance_Pwm = - balance(gyroYAngle,gyroYspeed);
			//ËÙ¶Èpwm
			Velocity_Pwm = velocity(encoderL,encoderR,0);
			//Èý»·Ïà¼Ópwm
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


//PID¿ØÖÆ²ÎÊý
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

/**********P±ÈÀý»·******************/
float balance(float Angle,float Gyro)
{  
   float Gap;
	 int balance;
	 Gap=Angle-Zero_point;                     //Çó³öÆ½ºâµÄ½Ç¶ÈÖÐÖµ ºÍ»úÐµÏà¹Ø
		balance=Balance_Kp*Gap+Gyro*Balance_Kd;   //¼ÆËãÆ½ºâ¿ØÖÆµÄµç»úPWM  PD¿ØÖÆ   kpÊÇPÏµÊý kdÊÇDÏµÊý 
	 return balance;
}

/*********PIËÙ¶È»·******************/
float velocity(int32_t encoder_left,int32_t encoder_right,float Target_Velocity)
{  

		Movement=-Target_Velocity/Velocity_Ref;
		
		Encoder_Least =(encoder_left+encoder_right)-0;                    //»ñÈ¡×îÐÂËÙ¶ÈÆ«²î==²âÁ¿ËÙ¶È£¨×óÓÒ±àÂëÆ÷Ö®ºÍ£©-Ä¿±êËÙ¶È£¨´Ë´¦ÎªÁã£© 
		Encoder *= 0.75f;		                                                //Ò»½×µÍÍ¨ÂË²¨Æ÷       
		Encoder += (float)Encoder_Least*0.25f;	                                    //Ò»½×µÍÍ¨ÂË²¨Æ÷    
		Encoder_Integral +=Encoder;                                       //»ý·Ö³öÎ»ÒÆ »ý·ÖÊ±¼ä£º10ms
		Encoder_Integral = Encoder_Integral-Movement;                       //½ÓÊÕÒ£¿ØÆ÷Êý¾Ý£¬¿ØÖÆÇ°½øºóÍË
		if(Encoder_Integral>8000)  	Encoder_Integral=8000.0;             //»ý·ÖÏÞ·ù
		if(Encoder_Integral<-8000)	Encoder_Integral=-8000.0;              //»ý·ÖÏÞ·ù	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //ËÙ¶È¿ØÖÆ	
//		if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //µç»ú¹Ø±ÕºóÇå³ý»ý·Ö
	  return Velocity;
}

