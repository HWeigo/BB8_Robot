/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId TASK_LEDHandle;
osThreadId TASK_CONTROLHandle;
osThreadId TASK_TIM_2MSHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void Task_LED(void const * argument);
void Task_Control(void const * argument);
void Task_tim_2ms(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	InitGyroUart();
	InitLED();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of TASK_LED */
  osThreadDef(TASK_LED, Task_LED, osPriorityLow, 0, 128);
  TASK_LEDHandle = osThreadCreate(osThread(TASK_LED), NULL);

<<<<<<< HEAD
  /* definition and creation of TASK_CONTROL */
  osThreadDef(TASK_CONTROL, Task_Control, osPriorityNormal, 0, 128);
  TASK_CONTROLHandle = osThreadCreate(osThread(TASK_CONTROL), NULL);

  /* definition and creation of TASK_TIM_2MS */
  osThreadDef(TASK_TIM_2MS, Task_tim_2ms, osPriorityHigh, 0, 512);
  TASK_TIM_2MSHandle = osThreadCreate(osThread(TASK_TIM_2MS), NULL);
=======
  /* definition and creation of TASK_GRYO */
//  osThreadDef(TASK_GRYO, Task_GRYO, osPriorityNormal, 0, 256);
//  TASK_GRYOHandle = osThreadCreate(osThread(TASK_GRYO), NULL);

  /* definition and creation of TASK_SERVO */
  osThreadDef(TASK_SERVO, Task_Servo, osPriorityNormal, 0, 512);
  TASK_SERVOHandle = osThreadCreate(osThread(TASK_SERVO), NULL);

  /* definition and creation of TASK_PS2 */
  osThreadDef(TASK_PS2, Task_ps2, osPriorityHigh, 0, 512);
  TASK_PS2Handle = osThreadCreate(osThread(TASK_PS2), NULL);

  /* definition and creation of TASK_ENCODER */
//  osThreadDef(TASK_ENCODER, Task_encoder, osPriorityAboveNormal, 0, 256);
//  TASK_ENCODERHandle = osThreadCreate(osThread(TASK_ENCODER), NULL);
>>>>>>> master

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* Task_LED function */
//void Task_LED(void const * argument)
//{

//  /* USER CODE BEGIN Task_LED */
//////////  /* Infinite loop */
//////////  for(;;)
//////////  {
//////////    osDelay(1);
//////////  }
//  /* USER CODE END Task_LED */
//}

///* Task_Control function */
//void Task_Control(void const * argument)
//{
//  /* USER CODE BEGIN Task_Control */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END Task_Control */
//}

///* Task_tim_2ms function */
//void Task_tim_2ms(void const * argument)
//{
//  /* USER CODE BEGIN Task_tim_2ms */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END Task_tim_2ms */
//}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
