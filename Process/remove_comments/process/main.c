
#include "main.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>              
#include "stm32f1xx_ll_usart.h"  

#define BUTTON_PORT             GPIOB
#define BUTTON_PIN              LL_GPIO_PIN_12
#define BUTTON_CLK_PERIPH       LL_APB2_GRP1_PERIPH_GPIOB

typedef struct {
  GPIO_TypeDef *port;   
  uint32_t pin;         
  uint8_t isActiveLow;  
} LED_TypeDef;

__IO uint8_t currentMode = MODE_LED_OFF; 
__IO uint8_t myData = 55U;               
__IO uint8_t *myDataPointer = NULL;      

static const uint16_t sosPattern[] = {150U, 150U, 150U, 500U, 500U, 500U, 150U, 150U, 150U};
static const uint16_t heartBeatPattern[] = {100U, 200U, 100U, 800U};

static const LED_TypeDef onboardLED =
{
  .port = GPIOC, 
  .pin = LL_GPIO_PIN_13, 
  .isActiveLow = 1U 
};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

uint32_t LL_GetTick(void);
void UART_Send_String(const char *str);
uint8_t Process_Button(void);
void Update_LED_Behavior(uint8_t mode);

void Play_Light_Pattern(const LED_TypeDef *led, const uint16_t *patternArray, uint8_t length, uint8_t currentPatternMode);

volatile uint32_t ms_ticks = 0; 

uint32_t LL_GetTick(void)
{
  return ms_ticks;
}

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();

  LL_SYSTICK_EnableIT();

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);

  LL_USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.BaudRate            = 115200U;                  
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;    
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;      
  USART_InitStruct.Parity              = LL_USART_PARITY_NONE;     
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX; 
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;  
  LL_USART_Init(USART1, &USART_InitStruct);

  LL_USART_Enable(USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);

  LL_GPIO_SetPinMode(BUTTON_PORT, BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(BUTTON_PORT, BUTTON_PIN, LL_GPIO_PULL_UP);

  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE12);

  LL_EXTI_InitTypeDef EXTI_InitStruct;

  EXTI_InitStruct.Line_0_31   = LL_EXTI_LINE_12;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode        = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger     = LL_EXTI_TRIGGER_FALLING;

  LL_EXTI_Init(&EXTI_InitStruct); 

  NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  UART_Send_String("STM32CubeIDE 01_blink_ll has started\r\n");

  while (1)
  {

    Update_LED_Behavior(currentMode);

    LL_mDelay(1);

  }

}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
}

static void MX_USART1_UART_Init(void)
{

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
   
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);

}

static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);

  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_12, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_12, LL_GPIO_PULL_UP);

}

uint8_t Process_Button(void)
{
  if (LL_GPIO_IsInputPinSet(BUTTON_PORT, BUTTON_PIN) == 0U)
  {
    LL_mDelay(20); 
    if (LL_GPIO_IsInputPinSet(BUTTON_PORT, BUTTON_PIN) == 0U)
    {
      
      while (LL_GPIO_IsInputPinSet(BUTTON_PORT, BUTTON_PIN) == 0U)
      {
        __NOP();
      }
      LL_mDelay(20); 
      return 1U;     
    }
  }
  return 0U; 
}

void Update_LED_Behavior(uint8_t mode)
{

  switch (mode)
  {
    case MODE_LED_OFF:  
      
      LL_GPIO_SetOutputPin(onboardLED.port, onboardLED.pin);
      break;

    case MODE_LED_ON:  
      
      LL_GPIO_ResetOutputPin(onboardLED.port, onboardLED.pin);
      break;

    case MODE_LED_SOS:  
      Play_Light_Pattern(&onboardLED, sosPattern, sizeof(sosPattern)/sizeof(sosPattern[0]), currentMode);
      break;

    case MODE_LED_HEARTBIT:
      Play_Light_Pattern(&onboardLED, heartBeatPattern, sizeof(heartBeatPattern)/sizeof(heartBeatPattern[0]), currentMode);
      break;

    default:
      
      Play_Light_Pattern(&onboardLED, NULL, 0, MODE_LED_OFF);
      break;
  }
}

void Play_Light_Pattern(const LED_TypeDef *led, const uint16_t *patternArray, uint8_t length, uint8_t currentPatternMode)
{

  static uint32_t last_led_update = 0;
  static uint8_t pattern_step = 0U;

  static uint8_t last_mode = 0U;

  if (currentPatternMode != last_mode)
  {

      char tx_buffer[64];
      
      snprintf(tx_buffer, sizeof(tx_buffer), "@Play_Light_Pattern : length  = %d\r\n", length);
      
      UART_Send_String(tx_buffer);

      snprintf(tx_buffer, sizeof(tx_buffer), "@Play_Light_Pattern : currentPatternMode  = %d\r\n", currentPatternMode);
      
      UART_Send_String(tx_buffer);

      pattern_step = 0U;              
      last_led_update = LL_GetTick(); 
      last_mode = currentPatternMode; 

      LL_GPIO_SetOutputPin(led->port, led->pin);
  }

  if (currentPatternMode == MODE_LED_OFF) {
      return;
  }

  if (patternArray == NULL || length == 0U) {
      return;
  }

  if (pattern_step >= (length * 2)) {
      pattern_step = 0U; 
  }

  uint16_t current_interval = patternArray[pattern_step / 2];

  if ((LL_GetTick() - last_led_update) >= current_interval)
  {
    last_led_update = LL_GetTick(); 

    if (pattern_step % 2 == 0) {
      LL_GPIO_ResetOutputPin(led->port, led->pin); 
    }
    
    else {
      LL_GPIO_SetOutputPin(led->port, led->pin); 
    }
    pattern_step++; 
  }
}

void UART_Send_String(const char *str)
{
  
  while (*str != '\0')
  {
    
    while (LL_USART_IsActiveFlag_TXE(USART1) == 0U)
    {
      __NOP(); 
    }

    LL_USART_TransmitData8(USART1, *str);

    str++;
  }
}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
   
}

#ifdef  USE_FULL_ASSERT
 
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif  
