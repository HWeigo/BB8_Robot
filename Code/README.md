 **注意,每次使用STM32CubeMX配置后，需要：**  
```
  1,将/Application/User/freertos.c和/Application/User/main.c中的头文件声明删去，并改为#include "includes.h";
  2,将/Application/User/freertos.c中任务句柄删去（已转移到/Tasks下的任务当中）;
  3,检查是否有新引用内容，若有，添加到includes.h当中。
  ```
