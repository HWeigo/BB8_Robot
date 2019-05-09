/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : ¶æ»ú¿ØÖÆ£¨ÆÕÍ¨ÓÅÏÈ¼¶£©
	* ¶æ»úÐÅºÅÏß -> PA0
	* ¶æ»úPWM½ÓÔÚÆÕÍ¨¶¨Ê±Æ÷TIM2ÉÏ£¬¶ÔÓ¦Ê¼Ê±ÖÓÎªAPB2:16MHZ¡£¶æ»úÇý¶¯ÐèÒª²úÉúÒ»¸ö
	* 20msµÄÂö³åÐÅºÅ£¬²ÉÓÃÔ¤·ÖÆµ160-1£¬×Ô¶¯ÖØÔØÖÜÆÚ2000-1¡£
	* PWMÆµÂÊ = 16 * 10^6 / 160 / 2000 = 50HZ¡£
	* ×¢Òâ£¬¶æ»ú¹¤×÷µçÑ¹Îª4.8V-6V¡£
  ******************************************************************************
*/

#include "includes.h"


//¶æ»úÐÍºÅÑ¡Ôñ 
#define SG90
//#define MG995


//SG90 :150Í£×ª <150Ë³Ê±Õë, >150ÄæÊ±Õë±
#ifdef SG90
uint16_t DutyCycle_STOP = 150;
#endif 
//MG995£º147Í£Ö¹ <147Ë³Ê±Õë >147ÄæÊ±Õë
#ifdef MG995
uint16_t DutyCycle_STOP = 147;
#endif 

//360¶ÈÄ£Äâ¶æ»úËÙ¶È¿ØÖÆ x>0£ºË³Ê±Õë x<0£ºÄæÊ±Õë
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP - x);

uint16_t rotateSpeed = 0; //·¶Î§+-100
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