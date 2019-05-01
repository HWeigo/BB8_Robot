#include "includes.h"

uint8_t tmp_gyro;

void InitGyroUart(void)
{
	if(HAL_UART_Receive_DMA(&GYRO_UART,&tmp_gyro,1) != HAL_OK)
	{
		Error_Handler();
	}
}

uint8_t gyro_receiving = 0;
uint8_t gyroBuffer[80] = {0};
uint8_t gyroBuffercnt = 0;
uint8_t gyroID = 0;
float gyroXAngle,gyroYAngle,gyroZAngle;
float gyroXspeed,gyroYspeed,gyroZspeed;


void gyroUartRxCpltCallback(void)
{
	if(gyro_receiving)
	{
		gyroBuffer[gyroBuffercnt] = tmp_gyro;
		gyroBuffercnt++;
		if(gyroBuffercnt > 20)gyro_receiving = 0;
		if(gyroBuffercnt == 11)
		{
			if(!sumCheck())
			{
				if(gyroBuffer[1] == 0x53)
				{
					gyroXAngle = (0x00|(gyroBuffer[3]<<8)|(gyroBuffer[2]))/32768.0f*180.0f;
					gyroYAngle = (0x00|(gyroBuffer[5]<<8)|(gyroBuffer[4]))/32768.0f*180.0f;
					gyroZAngle = (0x00|(gyroBuffer[7]<<8)|(gyroBuffer[6]))/32768.0f*180.0f;
				}
				else if(gyroBuffer[1] == 0x52)
				{
					gyroXspeed = ((short)(gyroBuffer[3]<<8)|gyroBuffer[2])/32768.0f*2000.0f;
					gyroYspeed = ((short)(gyroBuffer[5]<<8)|gyroBuffer[4])/32768.0f*2000.0f;
					gyroZspeed = ((short)(gyroBuffer[7]<<8)|gyroBuffer[6])/32768.0f*2000.0f;
				}
			}
			gyro_receiving = 0;
		}
	}
	else 
	{
		if(tmp_gyro == 0x55)
		{
			gyro_receiving = 1;
			gyroBuffercnt = 0;
			gyroBuffer[gyroBuffercnt] = tmp_gyro;
			gyroBuffercnt++;
		}
	}
	if(HAL_UART_Receive_DMA(&GYRO_UART, &tmp_gyro, 1) != HAL_OK)
		{
			Error_Handler();
	  }
}

//
uint8_t minus;
uint8_t sumCheck(void)
{
		minus = gyroBuffer[10];
		for(int i=0;i<10;i++)
		{
			minus -= gyroBuffer[i];
		}
		return minus;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle == &GYRO_UART)
	{
		gyroUartRxCpltCallback();
	}
}   


