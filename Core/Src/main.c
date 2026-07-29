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
#include <stddef.h>           // Defines NULL pointer macro
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
#define MODE_SOS                2U
#define MODE_HEARTBIT           3U
#define TOTAL_MODES             4U

// Define a structure to group all LED-related hardware parameters
typedef struct
{
  GPIO_TypeDef *port;   // Pointer to the GPIO Port (e.g., GPIOC)
  uint32_t pin;         // Specific GPIO Pin mask (e.g., LL_GPIO_PIN_13)
  uint8_t isActiveLow;  // 1 if LED turns ON when pin is LOW (like Blue Pill)
} LED_TypeDef;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Volatile tells compiler that this variable changes outside the main workflow (in ISR)
__IO uint8_t currentMode = MODE_LED_OFF;

__IO uint8_t myData = 55U;        // A standard test variable
__IO uint8_t *myDataPointer = NULL;  // A pointer variable (initialized to NULL / address 0)

// 1. Define global static arrays for patterns (Stored in flash memory due to const)
static const uint16_t sosPattern[] = {150U, 150U, 150U, 500U, 500U, 500U, 150U, 150U, 150U};
static const uint16_t heartBeatPattern[] = {100U, 200U, 100U, 800U};

// Create and initialize our onboard LED object using the new structure type
static const LED_TypeDef onboardLED =
{
    .port = GPIOC, // FIX: Added braces to satisfy GCC compiler rules
    .pin = LL_GPIO_PIN_13, // FIX: Added braces to satisfy GCC compiler rules
    .isActiveLow = 1U // Blue Pill onboard LED turns ON on LOW state
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
// Private Function Prototypes

uint8_t Process_Button(void);
void Update_LED_Behavior(uint8_t mode);

// Now function accepts a pointer to our LED configuration and a pattern
void Play_Light_Pattern(const LED_TypeDef *led, const uint16_t *patternArray, uint8_t length, uint8_t currentPatternMode);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


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

  /* 1. Enable Clocks for GPIOB and AFIO (AFIO is required for EXTI mapping on F1 series) */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);

  /* 2. Configure PB12 pin as Input with Pull-up resistor */
  LL_GPIO_SetPinMode(BUTTON_PORT, BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(BUTTON_PORT, BUTTON_PIN, LL_GPIO_PULL_UP);

  /* 3. Map EXTI Line 12 to GPIOB Port (Connects PB12 to EXTI12 controller) */
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE12);

  /* 4. Configure EXTI Line 12 to trigger on Falling Edge (Pressing pulls to GND / 0) */
  /* 4. Configure EXTI Line 12 using initialization structure */
    LL_EXTI_InitTypeDef EXTI_InitStruct;

  EXTI_InitStruct.Line_0_31   = LL_EXTI_LINE_12;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode        = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger     = LL_EXTI_TRIGGER_FALLING;

  LL_EXTI_Init(&EXTI_InitStruct); // Pass the pointer to structure

  /* 5. Enable EXTI Line 12 Interrupt in NVIC controller */
  // Line 12 on STM32F1 belongs to a shared vector EXTI15_10_IRQn (Lines 10 to 15)
  NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE END 2 */

  LL_SYSTICK_EnableIT();

  // Assign the physical memory address of 'myData' to our pointer
  myDataPointer = &myData;

  // (keep your existing button initialization code below)
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    // The main loop only cares about playing the pattern of the current mode.
    // The variable 'currentMode' will be updated completely asynchronously inside the ISR!
    Update_LED_Behavior(currentMode);

    // 1ms breath time to prevent debugger and Windows from hanging
    LL_mDelay(1);

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
    // Use structure directly if we want
    LL_GPIO_SetOutputPin(onboardLED.port, onboardLED.pin);
    break;

  case MODE_LED_ON:
    LL_GPIO_ResetOutputPin(onboardLED.port, onboardLED.pin);
    break;

  case MODE_SOS:
    // Pass the address of our structure (&onboardLED)
    Play_Light_Pattern(&onboardLED, sosPattern, 9U, MODE_SOS);
    LL_mDelay(1500);
    break;

  case MODE_HEARTBIT:
    Play_Light_Pattern(&onboardLED, heartBeatPattern, 4U, MODE_HEARTBIT);
    break;

  default:
    break;
  }
}

/** 

* @brief  Plays a light pattern on a specific LED defined by a structure pointer.
* @param  led: Pointer to the LED structural configuration.
* @param  patternArray: Pointer to the intervals array.
* @param  length: Number of elements in the array.
*/
void Play_Light_Pattern(const LED_TypeDef *led, const uint16_t *patternArray, uint8_t length, uint8_t currentPatternMode)
{
  for (uint8_t i = 0U; i < length; i++)
  {
    /* 1. CRITICAL CHECK: If the global mode changed asynchronously in the interrupt,
    instantly break the loop and exit the function right now! */

    if (currentMode != currentPatternMode)
    {
      break;
    }

    // Dynamic ON control based on structure data
    if (led->isActiveLow == 1U)
    {
      LL_GPIO_ResetOutputPin(led->port, led->pin); // LOW is ON
    } else {
      LL_GPIO_SetOutputPin(led->port, led->pin);   // HIGH is ON
    }

    LL_mDelay(*(patternArray + i));

    // Dynamic OFF control based on structure data
    if (led->isActiveLow == 1U)
    {
      LL_GPIO_SetOutputPin(led->port, led->pin);   // HIGH is OFF
    } else {
      LL_GPIO_ResetOutputPin(led->port, led->pin); // LOW is OFF
    }

    /* 2. SECOND CHECK: verify again before entering the next delay */
    if (currentMode != currentPatternMode)
    {
        break;
    }

    LL_mDelay(200);

  }
}

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
