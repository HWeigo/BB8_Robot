/**
  ******************************************************************************
  * File Name          : Task_servo.h
  * Description        : 舵机控制（普通优先级）
  ******************************************************************************
  */
#ifndef __TASK_SERVO_H
#define __TASK_SERVO_H

#include "includes.h"

void Task_Servo(void const * argument);
void setMotor1Speed(float speed_f);
void setMotor2Speed(float speed_f);
float balance(float Angle,float Gyro);

typedef enum
{
	Mode_1 = 1,
	Mode_2 = 2,
	Stop = 0,
}ServoCmd_e;

#endif /*__ TASK_LED_H */
