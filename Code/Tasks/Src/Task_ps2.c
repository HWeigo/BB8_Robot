#include "includes.h"

#define DI() 	HAL_GPIO_ReadPin(DI_GPIO_Port,DI_Pin)
#define DO_H() 		HAL_GPIO_WritePin(DO_GPIO_Port,DO_Pin,GPIO_PIN_SET)
#define DO_L()		HAL_GPIO_WritePin(DO_GPIO_Port,DO_Pin,GPIO_PIN_RESET)
#define CS_H() 		HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET)
#define CS_L() 		HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET)
#define CLK_H() 		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET)
#define CLK_L() 		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_RESET)


uint8_t flag = 0,x=0;
void Task_ps2(void const * argument)
{
  /* USER CODE BEGIN Task_ps2 */
  /* Infinite loop */
  while(1)
  {
//		x++;
//		delay10us(100000);
		
		PS2_ReadData();
		
    osDelay(5);
  }
  /* USER CODE END Task_ps2 */
}

uint32_t cnt10us = 0;
void delay10us(uint32_t n)
{
	HAL_TIM_Base_Start_IT(&htim3);
	while(cnt10us < n)
	{
//		osDelay(1);
	}
	HAL_TIM_Base_Stop_IT(&htim3);
	cnt10us = 0;
}

/****************************/

static uint8_t  fac_us=0;//us��ʱ������
static uint16_t fac_ms=0;//ms��ʱ������
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(uint8_t SYSCLK)                                                 //SYSCLK�õ���TIM1,InternalClock��FreeRTOSռ��
{
	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8                     //bit2Ϊ1 �ⲿʱ��Դ��   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK)����
	fac_us=SYSCLK/8;		                                                          //SYSCLK = 96MHz, HCLK = 96MHz
	fac_ms=(uint16_t)fac_us*1000;
}	


uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
uint8_t Comd[2]={0x01,0x42};	//��ʼ�����������

void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H();                   //�����Ϊ����λ
		}
		else DO_L();

		CLK_H();                        //ʱ������
		delay10us(1);
		CLK_L();
		delay10us(1);
		CLK_H();
		if(DI())
			Data[1] = ref|Data[1];
	}
}

//�ж��Ƿ�Ϊ���ģʽ
//����ֵ��0�����ģʽ
//		  ����������ģʽ
uint8_t PS2_RedLight(void)
{
	CS_L();
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	CS_H();
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;

	CS_L();

	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������

	for(byte=2;byte<9;byte++)          //��ʼ��������
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H();
			CLK_L();
			delay10us(1);
			CLK_H();
		      if(DI())
		      Data[byte] = ref|Data[byte];
		}
        delay10us(1);
	}
	CS_H();	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE END Callback 0 */
  if (htim==(&htim3)) {
    ++cnt10us;
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}
