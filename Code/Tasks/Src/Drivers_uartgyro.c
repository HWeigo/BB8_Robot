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
uint8_t gyroIsReady = 0;

void gyroUartRxCpltCallback(void)
{
	if(HAL_UART_Receive_DMA(&GYRO_UART, &tmp_gyro, 1) != HAL_OK)
	{
		Error_Handler();
	}
	if(gyro_receiving)
	{
		gyroBuffer[gyroBuffercnt] = tmp_gyro;
		gyroBuffercnt++;
		if(gyroBuffercnt > 13)
		{
			gyro_receiving = 0;
			gyroBuffercnt = 0;
		}
		if(gyroBuffercnt == 11)
		{
			gyroIsReady = 1;
			gyro_receiving = 0;
			gyroBuffercnt = 0;
		}
	}
	else 
	{
		if(gyroBuffercnt == 0 && tmp_gyro == 0x55)
		{
			gyro_receiving = 1;
			gyroBuffer[gyroBuffercnt] = tmp_gyro;
			++gyroBuffercnt;
		}
	}

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle == &GYRO_UART)
	{
		gyroUartRxCpltCallback();
	}
}   


