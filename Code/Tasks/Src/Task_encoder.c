/**
  ******************************************************************************
  * File Name          : Task_LED.c
  * Description        : LED����˸����Ӧ������PF9,PF10
	* λ��������ȼ������ڼ��ϵͳʱ���������Լ��Ƿ�崻���
  ******************************************************************************
  */
#include "includes.h"

//int32_t encoderL= 0, encoderR = 0;

//void Task_encoder(void const * argument)
//{
//  /* USER CODE BEGIN Task_encoder */
//  /* Infinite loop */
//	portTickType xLastWakeTime;
//	xLastWakeTime = xTaskGetTickCount();
//	
//  while(1)
//  {
//		if(__HAL_TIM_DIRECTION_STATUS(&htim5)==0)	encoderL=__HAL_TIM_GetCounter(&htim5);
//		else if(__HAL_TIM_GetCounter(&htim5)!=0)	encoderL=-(0xFFFF-__HAL_TIM_GetCounter(&htim5)+1);
//		if(__HAL_TIM_DIRECTION_STATUS(&htim3)==0)	encoderR=__HAL_TIM_GetCounter(&htim3);
//		else if(__HAL_TIM_GetCounter(&htim3)!=0)	encoderR=-(0xFFFF-__HAL_TIM_GetCounter(&htim3)+1);		
//		
//		
//		__HAL_TIM_SetCounter(&htim5,0);
//		__HAL_TIM_SetCounter(&htim3,0);
//		
//		vTaskDelayUntil( &xLastWakeTime, ( 45 / portTICK_RATE_MS ) ); //��������̬50ms
//    //osDelay(10);
//  }
//  /* USER CODE END Task_encoder */
//}
