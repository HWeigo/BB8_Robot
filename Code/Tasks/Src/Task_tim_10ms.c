/**
  ******************************************************************************
  * File Name          : Task_tim_10ms.c
  * Description        : ����10ms��׼��ʱ��ÿ��һ�ζ�ȡһ��ps2���ݣ�����ת��Ϊ 
	  ��ʵ���ݡ�ͬʱ��ÿ���50ms��ȡ��ˢ�±������Ķ����� ��������ȼ���
	* PS2���ݶ�ȡ��Ҫ�õ�us����ʱ�������֡���ݲμ�ps����ͨѶ�ֲᡣ
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

uint8_t cnt10ms = 0;
int32_t encoderL= 0, encoderR = 0;
void Task_tim_2ms(void const * argument)
{

	HAL_Delay(1500);
  while(1)
  {
		//����PS2�ֱ��źŽ�����Ҫus�����ӳ٣�����Ҫ�������жϹرգ�������ᵼ��ϵͳ崻�
			HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);	
		
		key = PS2_DataKey();
		speed = -(PS2_AnologData(PSS_LY)-127);	   
		swerve = (PS2_AnologData(PSS_LX)-127)-1;	
		servoSwerve = (PS2_AnologData(PSS_RX)-127)-1;
			HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn); //����ʹ���ж�
		
		if(cnt10ms == 4)
		{
			if(__HAL_TIM_DIRECTION_STATUS(&htim5)==0)	encoderL=__HAL_TIM_GetCounter(&htim5);
			else if(__HAL_TIM_GetCounter(&htim5)!=0)	encoderL=-(0xFFFF-__HAL_TIM_GetCounter(&htim5)+1);
	

			if(__HAL_TIM_DIRECTION_STATUS(&htim3)==0)	encoderR=__HAL_TIM_GetCounter(&htim3);
			else if(__HAL_TIM_GetCounter(&htim3)!=0)	encoderR=-(0xFFFF-__HAL_TIM_GetCounter(&htim3)+1);		
			
			if((encoderL>-3 && encoderL<3) || encoderL>2000 || encoderL<-2000) encoderL = 0;
			if((encoderR>-3 && encoderR<3) || encoderR>2000 || encoderR<-2000) encoderR = 0;
		
			
			__HAL_TIM_SetCounter(&htim5,0);
			__HAL_TIM_SetCounter(&htim3,0);
				
				cnt10ms = 0;
		}

		++cnt10ms;
    osDelay(7);
  }
  /* USER CODE END Task_ps2 */
}

//us�����ӳٺ�����ͨ������TIM14ʵ��
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

uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
uint8_t Comd[2]={0x01,0x42};	//��ʼ�����������
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
	};	//����ֵ�밴����

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
		delay_us(10);
		CLK_L();
		delay_us(10);
		CLK_H();
		if(DI())
			Data[1] = ref|Data[1];
	}
	delay_us(16); //16
}

//�ж��Ƿ�Ϊ���ģʽ
//����ֵ��1�����ģʽ
//		  ����������ģʽ
uint8_t PS2_RedLight(void)
{
	HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);		
	CS_L();
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	CS_H();
	HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);	
	if( Data[1] == 0X73)   return 1 ;
	else return 0;

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

//�Զ�������PS2�����ݽ��д���      ֻ�����˰�������         Ĭ�������Ǻ��ģʽ  ֻ��һ����������ʱ
//����Ϊ0�� δ����Ϊ1
uint16_t Handkey;
uint8_t PS2_DataKey(void)
{
	uint8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
uint8_t PS2_AnologData(uint8_t button)
{
	return Data[button];
}

//������ݻ�����
void PS2_ClearData(void)
{
	uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

