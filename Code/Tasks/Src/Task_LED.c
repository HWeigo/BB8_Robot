#include "includes.h"

//LED1:PF9 LED2:PF10
//�͵�ƽ����
#define LED1ON() 		HAL_GPIO_WritePin(GPIOF,LED1_Pin,GPIO_PIN_RESET)
#define LED20ON() 		HAL_GPIO_WritePin(GPIOF,LED2_Pin,GPIO_PIN_RESET)
#define LED1OFF() 		HAL_GPIO_WritePin(GPIOF,LED1_Pin,GPIO_PIN_SET)
#define LED2OFF() 		HAL_GPIO_WritePin(GPIOF,LED2_Pin,GPIO_PIN_SET)
#define LED1TOGGLE() 		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin)
#define LED2TOGGLE() 		HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin)

//��ʼ������
void InitLED(void)
{
	LED1OFF();
	LED20ON();
	
	const uint8_t initFlash = 5;          //��ʼ�����ƴ���
	for(uint8_t i=0;i<initFlash;++i)
	{
		LED1TOGGLE();
		LED2TOGGLE();
		HAL_Delay(100);
	}
}

//LED����������������ȼ�
/* Task_LED function */

void Task_LED(void const * argument)
{

  /* USER CODE BEGIN Task_LED */
  /* Infinite loop */
  while(1)
  {
		LED1ON();
		LED2TOGGLE();

		HAL_IWDG_Refresh(&hiwdg); //ι��
		
		osDelay(300);		

  }
  /* USER CODE END Task_LED */
}

uint8_t humanDetect(void)
{
	//return HAL_GPIO_ReadPin(HCSensor_GPIO_Port,HCSensor_Pin) == GPIO_PIN_SET ? 1:0;
	if(HAL_GPIO_ReadPin(HCSensor_GPIO_Port,HCSensor_Pin) == GPIO_PIN_SET)
	{
		return 1;
	}
	else{
		return 0;
	}
}

