#include "includes.h"

void nos_delay(int n)
{
	while(n--);
}

void init_LED(void)
{
		HAL_GPIO_WritePin(GPIOF,LED1_Pin,GPIO_PIN_SET);
		nos_delay(10000);
}


void Task_LED(void const * argument)
{

  /* USER CODE BEGIN Task_LED */
  /* Infinite loop */
  while(1)
  {
		HAL_GPIO_WritePin(GPIOF,LED1_Pin,GPIO_PIN_RESET);
//		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		HAL_GPIO_WritePin(GPIOF,LED2_Pin,GPIO_PIN_SET);
		osDelay(200);
		HAL_GPIO_WritePin(GPIOF,LED2_Pin,GPIO_PIN_RESET);
		osDelay(200);
		
    osDelay(1);
  }
  /* USER CODE END Task_LED */
}
