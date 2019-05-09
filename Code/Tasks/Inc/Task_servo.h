/**
  ******************************************************************************
  * File Name          : Task_servo.h
  * Description        : ������ƣ���ͨ���ȼ���
  ******************************************************************************
  */
#ifndef __TASK_SERVO_H
#define __TASK_SERVO_H

#include "includes.h"

void Task_Servo(void const * argument);

typedef enum
{
	Mode_1 = 1,
	Mode_2 = 2,
	Stop = 0,
}ServoCmd_e;

#endif /*__ TASK_LED_H */
