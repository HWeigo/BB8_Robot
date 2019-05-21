/**
  ******************************************************************************
  * File Name          : Task_LED.h
  * Description        : 
  ******************************************************************************
  */
#ifndef __TASK_LED_H
#define __TASK_LED_H

#include "includes.h"

void Task_LED(void const * argument);
void InitLED(void);
void delay_us(uint16_t us);

#endif /*__ TASK_LED_H */

