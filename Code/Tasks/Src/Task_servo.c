/**
  ******************************************************************************
  * File Name          : Task_servo.c
  * Description        : ¶æ»ú¿ØÖÆ£¨ÆÕÍ¨ÓÅÏÈ¼¶£©
	* ¶æ»úÐÅºÅÏß -> PA0
	* ¶æ»úPWM½ÓÔÚÆÕÍ¨¶¨Ê±Æ÷TIM2ÉÏ£¬¶ÔÓ¦Ê¼Ê±ÖÓÎªAPB2:48MHZ¡£¶æ»úÇý¶¯ÐèÒª²úÉúÒ»¸ö
	* 20msµÄÂö³åÐÅºÅ£¬²ÉÓÃÔ¤·ÖÆµ480-1£¬×Ô¶¯ÖØÔØÖÜÆÚ2000-1¡£
	* PWMÆµÂÊ = 48 * 10^6 / 480 / 2000 = 50HZ¡£
	* ×¢Òâ£¬¶æ»ú¹¤×÷µçÑ¹Îª4.8V-6V¡£
	* ÐÅºÅÏß -> PA0
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
#define setServoSpeed(x) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP - x)
#define setServoStop() __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, DutyCycle_STOP)

uint16_t rotateSpeed = 0; //·¶Î§+-100
ServoCmd_e ServoCmd = Mode_1;
extern uint8_t key;
extern int16_t servoSwerve;
void Task_Servo(void const * argument)
{

	setServoSpeed(0);
  while(1)
  {
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
