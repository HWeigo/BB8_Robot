/**
  ******************************************************************************
  * File Name          : Task_ps2.h
  * Description        : 
  ******************************************************************************
  */
#ifndef __TASK_PS2_H
#define __TASK_PS2_H

#include "includes.h"

//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2         9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //”““°∏ÀX÷· ˝æ›
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8


void Task_ps2(void const * argument);
void delay10us(uint32_t n);
void PS2_Cmd(uint8_t CMD);
uint8_t PS2_RedLight(void);
void PS2_ReadData(void);
uint8_t PS2_DataKey(void);
uint8_t PS2_AnologData(uint8_t button);
void PS2_ClearData(void);
void delay_us(uint16_t us);

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif /*__ TASK_LED_H */

