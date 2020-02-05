/**
  ******************************************************************************
  * File Name          : Task_servo.h
  * Description        : 舵机控制（普通优先级）
  ******************************************************************************
  */
#ifndef __TASK_SERVO_H
#define __TASK_SERVO_H

#include "includes.h"

<<<<<<< HEAD:Code/Tasks/Inc/Task_control.h
void Task_Control(void const * argument);
=======
void Task_Servo(void const * argument);
>>>>>>> master:Code/Tasks/Inc/Task_servo.h
void setMotor1Speed(float speed_f);
void setMotor2Speed(float speed_f);
float balance(float Angle,float Gyro);
float velocity(int32_t encoder_left,int32_t encoder_right,float Target_Velocity);


typedef enum
{
	Mode_1 = 1,
	Mode_2 = 2,
	Stop = 0,
}ServoCmd_e;

#endif /*__ TASK_LED_H */
