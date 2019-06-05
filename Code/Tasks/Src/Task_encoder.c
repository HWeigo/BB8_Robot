/**
  ******************************************************************************
  * File Name          : Task_encoder.c
  * Description        : 
	* 
  ******************************************************************************
  */
#include "includes.h"



/*************编码器计数*****/
int32_t encoderL= 0, encoderR = 0;

uint32_t encoderDebug = 0,encoderCnt = 0;
uint32_t tmpEncoderL = 0;
void Task_encoder(void const * argument)
{

	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
  while(1)
  {
		++encoderCnt;
		if(encoderCnt == 20) 
		{
			encoderCnt=0;
			++encoderDebug;
		}
		//tmpEncoderL = __HAL_TIM_GetCounter(&htim5);
		
		if(__HAL_TIM_DIRECTION_STATUS(&htim5)==0)	encoderL=encoderL+__HAL_TIM_GetCounter(&htim5);
		else if(__HAL_TIM_GetCounter(&htim5)!=0)	encoderL=encoderL-(0xFFFF-__HAL_TIM_GetCounter(&htim5)+1);
		if(__HAL_TIM_DIRECTION_STATUS(&htim3)==0)	encoderR=encoderR+__HAL_TIM_GetCounter(&htim3);
		else if(__HAL_TIM_GetCounter(&htim3)!=0)	encoderR=encoderR-(0xFFFF-__HAL_TIM_GetCounter(&htim3)+1);		
		
		
		__HAL_TIM_SetCounter(&htim5,0);
		__HAL_TIM_SetCounter(&htim3,0);
		
		vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_RATE_MS ) ); //进入阻塞态50ms
    //osDelay(1);
  }
  /* USER CODE END Task_encoder */
}
