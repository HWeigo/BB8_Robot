#ifndef __DRIVERS_UARTGYRO_H
#define __DRIVERS_UARTGYRO_H

#define GYRO_UART huart1

void InitGyroUart(void);
void gyroUartRxCpltCallback(void);

#endif
