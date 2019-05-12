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

static uint8_t  fac_us=0;//us延时倍乘数
static uint16_t fac_ms=0;//ms延时倍乘数
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(uint8_t SYSCLK)                                                 //SYSCLK用的是TIM1,InternalClock被FreeRTOS占用
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8                     //bit2为1 外部时钟源？   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK)？？
	fac_us=SYSCLK/8;		                                                          //SYSCLK = 96MHz, HCLK = 96MHz
	fac_ms=(uint16_t)fac_us*1000;
}	


uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
uint8_t Comd[2]={0x01,0x42};	//开始命令。请求数据

void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H();                   //输出以为控制位
		}
		else DO_L();

		CLK_H();                        //时钟拉高
		delay10us(1);
		CLK_L();
		delay10us(1);
		CLK_H();
		if(DI())
			Data[1] = ref|Data[1];
	}
}

//判断是否为红灯模式
//返回值；0，红灯模式
//		  其他，其他模式
uint8_t PS2_RedLight(void)
{
	CS_L();
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H();
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//读取手柄数据
void PS2_ReadData(void)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;

	CS_L();

	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据

	for(byte=2;byte<9;byte++)          //开始接受数据
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
