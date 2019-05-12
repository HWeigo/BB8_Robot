/**
  ******************************************************************************
  * File Name          : Task_ps2.h
  * Description        : 
  ******************************************************************************
  */
#ifndef __TASK_PS2_H
#define __TASK_PS2_H

#include "includes.h"

void Task_ps2(void const * argument);
void delay10us(uint32_t n);
void PS2_Cmd(uint8_t CMD);
uint8_t PS2_RedLight(void);
void PS2_ReadData(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif /*__ TASK_LED_H */

