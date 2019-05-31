/**
  ******************************************************************************
  * File Name          : Task_encoder.c
  * Description        : 
	* 
  ******************************************************************************
  */
#include "includes.h"



/*************编码器计数*****/
int32_t encoder = 0;

uint32_t encoderDebug = 0,encoderCnt = 0, tmpEncoder = 0;
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
		tmpEncoder = __HAL_TIM_GetCounter(&htim5);
		if(__HAL_TIM_DIRECTION_STATUS(&htim5)==0)	encoder=encoder+__HAL_TIM_GetCounter(&htim5);
		else if(__HAL_TIM_GetCounter(&htim5)!=0)	encoder=encoder-(0xFFFF-__HAL_TIM_GetCounter(&htim5)+1);
		//printf("%d\n",i);
		__HAL_TIM_SetCounter(&htim5,0);
		
		vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_RATE_MS ) ); //进入阻塞态50ms
    //osDelay(1);
  }
  /* USER CODE END Task_encoder */
}
