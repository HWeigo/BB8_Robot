/**
  ******************************************************************************
  * File Name          : Task_encoder.c
  * Description        : 
	* 
  ******************************************************************************
  */
#include "includes.h"

uint32_t encoderDebug = 0,encoderCnt = 0;
void Task_encoder(void const * argument)
{

	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
  while(1)
  {
		++encoderCnt;
		if(encoderCnt == 100) 
		{
			encoderCnt=0;
			++encoderDebug;
		}
		
		vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_RATE_MS ) ); //½øÈë×èÈûÌ¬10ms
    //osDelay(1);
  }
  /* USER CODE END Task_encoder */
}
