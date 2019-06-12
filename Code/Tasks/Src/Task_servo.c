/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : ¶æ»ú¿ØÖÆ£¨ÆÕÍ¨ÓÅÏÈ¼¶£©
	* ¶æ»úÐÅºÅÏß -> PA0
	* ¶æ»úPWM½ÓÔÚÆÕÍ¨¶¨Ê±Æ÷TIM2ÉÏ£¬¶ÔÓ¦Ê¼Ê±ÖÓÎªAPB2:48MHZ¡£¶æ»úÇý¶¯ÐèÒª²úÉúÒ»¸ö
	* 20msµÄÂö³åÐÅºÅ£¬²ÉÓÃÔ¤·ÖÆµ480-1£¬×Ô¶¯ÖØÔØÖÜÆÚ2000-1¡£
	* PWMÆµÂÊ = 48 * 10^6 / 480 / 2000 = 50HZ¡£
	* ×¢Òâ£¬¶æ»ú¹¤×÷µçÑ¹Îª4.8V-6V¡£
	* ÐÅºÅÏß -> PA2
  ******************************************************************************
*/

#include "includes.h"


//¶æ»úÐÍºÅÑ¡Ôñ 
//#define SG90
#define MG995


//SG90 :150Í£×ª <150Ë³Ê±Õë, >150ÄæÊ±Õë±
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995£º147Í£Ö¹ <147Ë³Ê±Õë >147ÄæÊ±Õë
#ifdef MG995
uint16_t DutyCycle_STOP = 150; //Áãµã±ê¶¨ËÆºõÓëµç³ØµçÁ¿£¨µçÑ¹£©ÓÐ¹Ø
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
//Ö±Á¢»·PID¿ØÖÆ²ÎÊý
float Zero_point=0.5;
float Balance_Kp=0.0; //6.0f
float Balance_Kd=1.0;

#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))
#define MINMAX(value, min, max) (value) = ((value) < (min)) ? (min) : ((value) > (max) ? (max) : (value))

extern float gyroYAngle,gyroYspeed;
float Balance_Pwm=0,Velocity_Pwm,Turn_Pwm;
void Task_Servo(void const * argument)
{
	/**************************************************************************/
	/*
	*³µÂÖPWMÇý¶¯¿ØÖÆ£¬ÔÝÊ±·ÅÔÚservo.c
	*ÀûÓÃTIM4²ÎÊýPWM£¬Çý¶¯ÆµÂÊÎª10kHz£¨ÍÆ¼ö£©£¬²ÉÓÃÔ¤·ÖÆµ48-1£¬×Ô¶¯ÖØÔØÖÜÆÚ100-1¡£
	*¿ØÖÆ·¶Î§0-100¡£
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
				/*µÈ´ý»Øµ÷ÐÅºÅÁ¿*/
		//xSemaphoreTake(xSemaphore_rcuart, osWaitForever);
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
			//Balance_Pwm = - MAX(MIN(balance(gyroYAngle,gyroYspeed),100),-100); 
			Balance_Pwm = - balance(gyroYAngle,gyroYspeed);
			MINMAX(Balance_Pwm,-100,100);
			
			setMotor1Speed(Balance_Pwm); //¸ºÖµÓÐÎÊÌâ
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

    osDelay(20);
  }
  /* USER CODE END Task_Servo */
}

/***********PDÖ±Á¢»·*************/
float balance(float Angle,float Gyro)
{  
   float Gap;
	 int balance;
	 Gap=Angle-Zero_point;                     //===Çó³öÆ½ºâµÄ½Ç¶ÈÖÐÖµ ºÍ»úÐµÏà¹Ø
	 balance=Balance_Kp*Gap+Gyro*Balance_Kd;   //===¼ÆËãÆ½ºâ¿ØÖÆµÄµç»úPWM  PD¿ØÖÆ   kpÊÇPÏµÊý kdÊÇDÏµÊý 
	 return balance;
}
/***********************************/


/***********PIËÙ¶È»·*************/
/*float velocity(int encoderL,int encoderR)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral,Target_Velocity;
	  //=============Ò£¿ØÇ°½øºóÍË²¿·Ö=======================// 
	  if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                 //Èç¹û½øÈë±ÜÕÏÄ£Ê½,×Ô¶¯½øÈëµÍËÙÄ£Ê½
    else 	                         Target_Velocity=110;                 
		if(1==Flag_Qian)    	Movement=Target_Velocity/Flag_sudu;	         //===Ç°½ø±êÖ¾Î»ÖÃ1 
		else if(1==Flag_Hou)	Movement=-Target_Velocity/Flag_sudu;         //===ºóÍË±êÖ¾Î»ÖÃ1
	  else  Movement=0;	
	  if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //±ÜÕÏ±êÖ¾Î»ÖÃ1ÇÒ·ÇÒ£¿Ø×ªÍäµÄÊ±ºò£¬½øÈë±ÜÕÏÄ£Ê½
	  Movement=-Target_Velocity/Flag_sudu;
   //=============ËÙ¶ÈPI¿ØÖÆÆ÷=======================//	
		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //===»ñÈ¡×îÐÂËÙ¶ÈÆ«²î==²âÁ¿ËÙ¶È£¨×óÓÒ±àÂëÆ÷Ö®ºÍ£©-Ä¿±êËÙ¶È£¨´Ë´¦ÎªÁã£© 
		Encoder *= 0.8;		                                                //===Ò»½×µÍÍ¨ÂË²¨Æ÷       
		Encoder += Encoder_Least*0.2;	                                    //===Ò»½×µÍÍ¨ÂË²¨Æ÷    
		Encoder_Integral +=Encoder;                                       //===»ý·Ö³öÎ»ÒÆ »ý·ÖÊ±¼ä£º10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===½ÓÊÕÒ£¿ØÆ÷Êý¾Ý£¬¿ØÖÆÇ°½øºóÍË
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===»ý·ÖÏÞ·ù
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===»ý·ÖÏÞ·ù	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //===ËÙ¶È¿ØÖÆ	
		if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //===µç»ú¹Ø±ÕºóÇå³ý»ý·Ö
	  return Velocity;
}*/
/***********************************/


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
