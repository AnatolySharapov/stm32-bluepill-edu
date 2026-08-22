/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"

/* Adds EXTI line configuration and flag clearing */
#include "stm32f1xx_ll_exti.h"   

#include "stm32f1xx_ll_cortex.h"

/* Adds delay functions like LL_mDelay */
#include "stm32f1xx_ll_utils.h" 
 
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"

/* Adds USART_InitTypeDef and UART functions */
#include "stm32f1xx_ll_usart.h"  
#include "stm32f1xx_ll_gpio.h"

/* Adds Low-Layer TIM peripheral driver definitions */
#include "stm32f1xx_ll_tim.h" 

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>              /* Defines NULL pointer macro */
#include <string.h>              /* Required for strcmp function operations */
#include <ctype.h>               /* Required for toupper() function for case conversion */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/

/* USER CODE BEGIN EM */
#define DEBUG_LOG_ENABLE 1

#if defined(DEBUG_LOG_ENABLE) && (DEBUG_LOG_ENABLE == 1)
  /* Automated logging macro with function name and line number prefix */
  #define DEBUG_PRINT(fmt, ...) do { \
    char tx_buf[128]; \
    snprintf(tx_buf, sizeof(tx_buf), "[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    UART_Send_String(tx_buf); \
  } while(0)
#else
  /* Stripped out completely during the compilation process for release targets */
  #define DEBUG_PRINT(fmt, ...)
#endif

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

// FIX: Declare the function prototype so stm32f1xx_it.c knows it exists
void UART_Send_String(const char *str);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

void Delay(__IO uint32_t);

/* Definitions for our 4 LED modes */
#define MODE_LED_OFF 0U
#define MODE_LED_ON  1U
#define MODE_LED_SOS 2U
#define MODE_LED_HTB 3U /* Heartbeat mode alias */

/* Size of the USART receive buffer */
#define RX_BUF_SIZE 32

/* Expanded definition for PWM operational mode */
#define MODE_LED_PWM 4U 

/* Safely redefine the total modes count to include PWM without compiler warnings */
#ifdef TOTAL_MODES
#undef TOTAL_MODES
#endif
/* Updated total modes count to include PWM support */
#define TOTAL_MODES  5U 

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
