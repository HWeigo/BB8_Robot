#include "includes.h"

//LED1:PF9 LED2:PF10
//低电平点亮
#define LED1ON() 		HAL_GPIO_WritePin(GPIOF,LED1_Pin,GPIO_PIN_RESET)
#define LED20ON() 		HAL_GPIO_WritePin(GPIOF,LED2_Pin,GPIO_PIN_RESET)
#define LED1OFF() 		HAL_GPIO_WritePin(GPIOF,LED1_Pin,GPIO_PIN_SET)
#define LED2OFF() 		HAL_GPIO_WritePin(GPIOF,LED2_Pin,GPIO_PIN_SET)
#define LED1TOGGLE() 		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin)
#define LED2TOGGLE() 		HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin)

//初始化闪灯
uint8_t debug_flag=0;
void InitLED(void)
{
	LED1OFF();
	LED20ON();
	
	debug_flag=1;
	const uint8_t initFlash = 5;          //初始化闪灯次数
	for(uint8_t i=0;i<initFlash;++i)
	{
		LED1TOGGLE();
		LED2TOGGLE();
		HAL_Delay(100);
	}

	debug_flag =2;
}

//LED任务运行在最低优先级
/* Task_LED function */

void Task_LED(void const * argument)
{

  /* USER CODE BEGIN Task_LED */
  /* Infinite loop */
  while(1)
  {
		LED1ON();
		LED2TOGGLE();
		osDelay(400);		

  }
  /* USER CODE END Task_LED */
}
