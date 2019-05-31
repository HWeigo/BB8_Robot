/**
  ******************************************************************************
  * File Name          : Task_ps2.c
  * Description        : 读取ps2数据，将其转化为真实数据（最高优先级）
	* PS2数据读取需要用到us级别定时。其具体帧内容参见ps解码通讯手册。
	* DI/DAT  -> PB12
	* DO/CMD  -> PB13
	* CS  -> PB14
 	* CLK -> PB15
  ******************************************************************************
*/
#include "includes.h"

#define DI() 	HAL_GPIO_ReadPin(DI_GPIO_Port,DI_Pin)
#define DO_H() 		HAL_GPIO_WritePin(DO_GPIO_Port,DO_Pin,GPIO_PIN_SET)
#define DO_L()		HAL_GPIO_WritePin(DO_GPIO_Port,DO_Pin,GPIO_PIN_RESET)
#define CS_H() 		HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET)
#define CS_L() 		HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET)
#define CLK_H() 		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET)
#define CLK_L() 		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_RESET)


uint8_t flag = 0,x=0;
uint8_t key;
int16_t speed = 0;
int16_t swerve = 0;
int16_t servoSwerve = 0;
void Task_ps2(void const * argument)
{
  /* USER CODE BEGIN Task_ps2 */
  /* Infinite loop */
	HAL_Delay(1000);
  while(1)
  {
		//由于PS2手柄信号接收需要us级别延迟，故需要将所有中断关闭！！否则会导致系统宕机
			HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);	
		
		key = PS2_DataKey();
		speed = -(PS2_AnologData(PSS_LY)-127);	   
		swerve = (PS2_AnologData(PSS_LX)-127)-1;	
		servoSwerve = (PS2_AnologData(PSS_RX)-127)-1;
		
		  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn); //重启使能中断
    osDelay(30);
  }
  /* USER CODE END Task_ps2 */
}

//us级别延迟函数，通过配置TIM14实现
void delay_us(uint16_t us)
{
    uint16_t differ=0xffff-us-5;

    HAL_TIM_Base_Start(&htim14);

    __HAL_TIM_SetCounter(&htim14,differ);

    while(differ<0xffff-5)
    {
        differ=__HAL_TIM_GetCounter(&htim14);
    }

    HAL_TIM_Base_Stop(&htim14);
}

//static uint8_t  fac_us=0;//us延时倍乘数
//static uint16_t fac_ms=0;//ms延时倍乘数
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
//void delay_init(uint8_t SYSCLK)                                                 //SYSCLK用的是TIM1,InternalClock被FreeRTOS占用
//{
//	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8                     //bit2为1 外部时钟源？   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK)？？
//	fac_us=SYSCLK/8;		                                                          //SYSCLK = 96MHz, HCLK = 96MHz
//	fac_ms=(uint16_t)fac_us*1000;
//}	


uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
uint8_t Comd[2]={0x01,0x42};	//开始命令。请求数据
uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明

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
		delay_us(10);
		CLK_L();
		delay_us(10);
		CLK_H();
		if(DI())
			Data[1] = ref|Data[1];
	}
	delay_us(16); //16
}

//判断是否为红灯模式
//返回值；1，红灯模式
//		  其他，其他模式
uint8_t PS2_RedLight(void)
{
	HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);		
	CS_L();
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H();
	HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);	
	if( Data[1] == 0X73)   return 1 ;
	else return 0;

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
			delay_us(10);
			CLK_L();
			delay_us(10);
			CLK_H();
			if(DI())
			{
			Data[byte] = ref|Data[byte];
			}
		}
      delay_us(10);
	}
	CS_H();	
}

//对读出来的PS2的数据进行处理      只处理了按键部分         默认数据是红灯模式  只有一个按键按下时
//按下为0， 未按下为1
uint16_t Handkey;
uint8_t PS2_DataKey(void)
{
	uint8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
uint8_t PS2_AnologData(uint8_t button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData(void)
{
	uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

//uint32_t cnt10us = 0;
//void delay10us(uint32_t n)
//{
//	HAL_TIM_Base_Start_IT(&htim3);
//	while(cnt10us < n)
//	{
////		osDelay(1);
//	}
//	HAL_TIM_Base_Stop_IT(&htim3);
//	cnt10us = 0;
//}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  /* USER CODE BEGIN Callback 0 */
//  if (htim->Instance == TIM1) {
//    HAL_IncTick();
//  }
//  /* USER CODE END Callback 0 */
//  if (htim==(&htim3)) {
//    ++cnt10us;
//  }
//  /* USER CODE BEGIN Callback 1 */

//  /* USER CODE END Callback 1 */
//}
