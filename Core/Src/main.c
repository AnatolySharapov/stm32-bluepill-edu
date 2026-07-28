/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/*
	#include (Preprocessor): The lines at the very top include external files
	(headers/libraries).
	Without them, the compiler will not know what LL_GPIO_TogglePin means.
	This is the foundation of modularity in C.
*/
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUTTON_PORT             GPIOB
#define BUTTON_PIN              LL_GPIO_PIN_12
#define BUTTON_CLK_PERIPH       LL_APB2_GRP1_PERIPH_GPIOB

// Definitions for our 4 LED modes
#define MODE_LED_OFF            0U
#define MODE_LED_ON             1U
#define MODE_BLINK_SOS          2U
#define MODE_BLINK_FAST         3U

#define TOTAL_MODES             4U

// #define LED_BLINK_FAST_MS    100U
// #define LED_BLINK_SLOW_MS    600U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Global variable for debug
// Global variable for Live Expressions tracking
// Global variables are easy to track in Live Expressions
__IO uint32_t bounceCounter = 0U; // Counter for physical bounces

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
// Private Function Prototypes

uint8_t Process_Button(void);
void Update_LED_Behavior(uint8_t mode);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
	Data Types (uint32_t):
    Notice the uint32_t nCount variable.
    Embedded C requires strict memory management.
    This is an unsigned (u), integer (int), strictly 32-bit (32) number.
    Avoid using the standard, generic int for now — in microcontroller development,
    you should always use fixed-width types from the stdint.h library.
*/

/*
  The __IO Modifier (or volatile):
    The __IO macro stands before the data type in the delay function (in pure C,
    this maps to the volatile keyword).
    It prevents the compiler from optimizing away (deleting) the "empty" delay loop.
    Without it, the compiler would assume: "This loop does nothing useful, let's remove it."
    If removed, the LED would blink at a frequency of millions of Hertz,
    making it look like it is permanently turned on.
*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* NOJTAG: JTAG-DP Disabled and SW-DP Enabled */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* Enable SysTick exception (interrupt) */
  LL_SYSTICK_EnableIT();

  /* Enable GPIOB clock for the button */
  LL_APB2_GRP1_EnableClock(BUTTON_CLK_PERIPH);

  /* Configure PB12 pin as Input with Pull-up resistor */
  LL_GPIO_SetPinMode(BUTTON_PORT, BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(BUTTON_PORT, BUTTON_PIN, LL_GPIO_PULL_UP);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t currentMode = MODE_LED_OFF;

  while (1)
  {
    // 1. Ask our function if a click happened
    if (Process_Button() == 1U)
    {
      currentMode++;

      if (currentMode >= TOTAL_MODES)
      {
        currentMode = MODE_LED_OFF;
      }
    }

    // 2. Pass the mode variable as an argument to the second function
    Update_LED_Behavior(currentMode);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);

  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_12, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_12, LL_GPIO_PULL_UP);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// File-scope static variable (hidden from main.c but visible to functions below)
static uint8_t fastModeIntroPlayed = 0U;

/** 

* @brief  Reads the button with debounce and waits for release.
* @retval 1 if a true button click occurred, 0 otherwise.
*/
uint8_t Process_Button(void)
{
  if (LL_GPIO_IsInputPinSet(BUTTON_PORT, BUTTON_PIN) == 0U)
  {
    LL_mDelay(20); // Debounce delay
    if (LL_GPIO_IsInputPinSet(BUTTON_PORT, BUTTON_PIN) == 0U)
    {
      // Wait until the user releases the button
      while (LL_GPIO_IsInputPinSet(BUTTON_PORT, BUTTON_PIN) == 0U)
      {
        __NOP();
      }
      LL_mDelay(20); // Release debounce delay
      return 1U;     // Valid click confirmed!
    }
  }
  return 0U; // No valid click
}

void Update_LED_Behavior(uint8_t mode)
{
  switch (mode)
  {
    case MODE_LED_OFF:
      LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
      // Reset the intro flag for the next cycle
      fastModeIntroPlayed = 0U;
      break;

    case MODE_LED_ON:
      LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
      break;

    case MODE_BLINK_SOS:
      {
      // 1. Array storing time intervals for the SOS Morse pattern (in milliseconds)
      // 150ms = Dot, 500ms = Dash, 200ms = Pause between components
      // Total of 9 flashes (3 dots, 3 dashes, 3 dots)
      static const uint16_t sosPattern[9] = {150U, 150U, 150U, 500U, 500U, 500U, 150U, 150U, 150U};

      // 2. Loop through each of the 9 elements of the array
      for (uint8_t i = 0U; i < 9U; i++)
      {
        // Extra safety: double-check if the mode was changed while running the loop
        if (mode != MODE_BLINK_SOS)
        {
          break;
        }

        LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13); // LED ON
        LL_mDelay(sosPattern[i]);                      // Read delay value from the array element 'i'

        LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);   // LED OFF
        LL_mDelay(200);                                // Fixed pause between flashes
      }

      // Long 1.5-second pause before repeating the entire SOS signal
      LL_mDelay(1500);

      }
      break;

    case MODE_BLINK_FAST:
      // Check if we need to play the intro strobe
      if (fastModeIntroPlayed == 0U)
      {
        // Loop repeats exactly 5 times
        for (uint8_t i = 0U; i < 5U; i++)
        {
          LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13); // LED ON
          LL_mDelay(40);
          LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);   // LED OFF
          LL_mDelay(40);
        }
        fastModeIntroPlayed = 1U; // Lock the intro
      }
      // Regular continuous blinking
      LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
      LL_mDelay(150);
      break;

    default:
      break;
  }

}
/* USER CODE END 4 */

/* USER CODE END 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
