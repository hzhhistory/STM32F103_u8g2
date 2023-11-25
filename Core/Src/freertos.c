/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"oled.h"
#include"u8g2.h"
#include"WALL_E.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
u8g2_t u8g2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
short x=10,y=10;
typedef struct
{
    char * str;
}SET_LIST;

SET_LIST list[]={
        {"CAR_SETING"},
        {"TEMP"},
        {"BMP280"},
        {"BEEP"},
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for oled */
osThreadId_t oledHandle;
const osThreadAttr_t oled_attributes = {
  .name = "oled",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LED */
osThreadId_t LEDHandle;
const osThreadAttr_t LED_attributes = {
  .name = "LED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void testDrawPixelToFillScreen()
{
    int t = 1000;
    u8g2_ClearBuffer(&u8g2);
    for (int j = 0; j < 64; j++)
    {
        for (int i = 0; i < 128; i++)
        {
            u8g2_DrawPixel(&u8g2,i, j);
        }
    }
    do {
    u8g2_SendBuffer(&u8g2);
    HAL_Delay(1000);
    }while (0);
}


void my_u8g2_init(void)
{
    u8g2_init(&u8g2);

    u8g2_FirstPage(&u8g2);
    u8g2_ClearBuffer(&u8g2);
    do
    {
        for(int i=20;i>12;i--)
        {
            u8g2_DrawCircle(&u8g2,64,32,i,U8G2_DRAW_ALL);
            HAL_Delay(200);
            u8g2_SendBuffer(&u8g2);
        }

    }while(u8g2_NextPage(&u8g2));

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawXBMP(&u8g2, 0, 0, wall_e_bmp01_x, wall_e_bmp01_y, wall_e_bmp01);
    u8g2_SendBuffer(&u8g2);
    HAL_Delay(500);
}

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of oled */
  oledHandle = osThreadNew(StartDefaultTask, NULL, &oled_attributes);

  /* creation of LED */
  LEDHandle = osThreadNew(StartTask02, NULL, &LED_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the oled thread.
  * @param  argument: Not used
  * @retval None
  */

/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{

  /* USER CODE BEGIN StartDefaultTask */
    my_u8g2_init();
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2,u8g2_font_ncenB10_tr);
    u8g2_DrawStr(&u8g2,0,30,"HELLO WORLD!!");
    u8g2_SendBuffer(&u8g2);
    HAL_Delay(500);
  /* Infinite loop */
  for(;;)
  {

//      testDrawPixelToFillScreen();
      uint8_t listlen=sizeof (list)/sizeof (SET_LIST);
    u8g2_SetFont(&u8g2,u8g2_font_ncenB08_tr);

   u8g2_ClearBuffer(&u8g2);
   for(int i=0;i<listlen;i++)
   {
       u8g2_DrawStr(&u8g2,x,y*i+16,list[i].str);
   }
      u8g2_DrawStr(&u8g2,110,16,"*");
      u8g2_SendBuffer(&u8g2);


    osDelay(1);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

   vTaskDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
