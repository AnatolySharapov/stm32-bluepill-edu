/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */

/* Global tracking variables inherited from main.c for interrupt handling */
extern volatile uint8_t currentMode;
extern uint32_t last_press_time;

/* USART buffers and flags inherited from main.c */
extern volatile char rx_buffer[];
extern volatile uint8_t rx_index;
extern volatile uint8_t command_ready;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

extern volatile uint32_t ms_ticks;
uint32_t LL_GetTick(void); // Prototype of the tick getter function

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  ms_ticks++; // Increment the global millisecond counter every 1 ms
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/** 

* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* Check if the RXNE interrupt flag is active and interrupt generation is enabled */
  if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
  {
    /* Reading the data register automatically clears the RXNE flag */
    char received_char = (char)LL_USART_ReceiveData8(USART1);

    /* Echo feature: send the received character back to the transmitter host */
    while (!LL_USART_IsActiveFlag_TXE(USART1));
    LL_USART_TransmitData8(USART1, received_char);
    
    /* CRITICAL SAFETY HACK: 
       If the previous command has NOT been processed in main yet, ignore 
       all new incoming characters. This protects rx_buffer from data corruption. */
    if (command_ready == 1)
    {
      return; 
    }

    /* Check if the received character is a valid command terminator */
    /* Check for line termination characters */
    if (received_char == '\n' || received_char == '\r')
    {
      if (rx_index > 0) /* Ensure the buffer actually contains command letters */
      {
        rx_buffer[rx_index] = '\0'; /* Terminate the string safely */
        command_ready = 1;          /* Notify the main loop that data is ready */
                                    /* Set the atomic flag indicating data is ready */
      }
    }
    else
    {
      /* Ignore any unexpected leading control characters (like extra \r or \n) */
      if (received_char >= 32 && received_char <= 126)
      {
        /* Secure check: ensure we leave exactly 1 byte for the null terminator '\0' */
        if (rx_index < (RX_BUF_SIZE - 1))
        {
          rx_buffer[rx_index++] = received_char;
        }
        else
        {
          /* Hard constraint: buffer is full, force terminate and trigger error flag */
          rx_buffer[rx_index] = '\0';
          command_ready = 1;
        }
      }
    }
  }
  /* USER CODE END USART1_IRQn 0 */
}


/** 

* @brief  This function handles EXTI Line 10 to 15 interrupts (Triggers on PB12 press).
*/
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* Check if the interrupt was actually triggered by EXTI Line 12 (PB12) */
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_12) != RESET)
  {
    /* Clear the interrupt pending flag immediately to avoid infinite ISR re-entry */
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_12);

    /* Get the current system uptime in milliseconds from SysTick counter */
    uint32_t current_time = LL_GetTick();

    /* Software debounce: check if 500 ms have passed since the last valid press */
    if ((current_time - last_press_time) > 500)
    {
      /* If 500 ms passed, it is a valid button press, not a contact bounce */
      
      /* Advance to the next sequential LED operational mode cyclically */
      currentMode = (currentMode + 1) % TOTAL_MODES;

      /* Transmit the updated status log via unified automated macro */
      switch (currentMode)
      {
        case MODE_LED_OFF: DEBUG_PRINT("Mode Changed: LED OFF\r\n"); break;
        case MODE_LED_ON:  DEBUG_PRINT("Mode Changed: LED ON\r\n"); break;
        case MODE_LED_SOS: DEBUG_PRINT("Mode Changed: SOS\r\n"); break;
        case MODE_LED_HTB: DEBUG_PRINT("Mode Changed: HEARTBEAT\r\n"); break;
        default: break;
      }

      /* Save the timestamp of the current valid press event */
      last_press_time = current_time;
    }
  }
  /* USER CODE END EXTI15_10_IRQn 0 */
}


/* USER CODE END 1 */
