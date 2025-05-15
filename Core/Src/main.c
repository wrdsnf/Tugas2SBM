// Header
#include "main.h"

// Variabel privat
uint8_t mode = 0;
uint32_t last_interrupt_time_pb0 = 0;
uint32_t last_interrupt_time_pb1 = 0;
const uint32_t debounce_delay = 200;

uint32_t last_blink_time = 0;
const uint32_t blink_interval = 300;
uint8_t blink_state = 0;

// Deklarasi fungsi privat
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

// Main program
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  while (1)
  {
    uint32_t now = HAL_GetTick();
    if (now - last_blink_time >= blink_interval)
    {
      last_blink_time = now;
      blink_state = !blink_state;

      switch (mode)
      {
        case 0:
        // Mode 0 (Default): PC13 menyala, PA5 dan PA6 mati
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // PC13 ON
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET); // PA5, PA6 OFF
          break;

        case 1:
        // Mode 1: PC13 menyala, PA5 blink, PA6 mati
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // PC13 ON
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, blink_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
          break;

        case 2:
        // Mode 2: PC13 menyala, PA5 mati, PA6 blink
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // PC13 ON
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, blink_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
          break;

        case 3:
        // Mode 3: PC13 mati, PA5 dan PA6 blink bersamaan
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // PC13 OFF
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, blink_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, blink_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
          break;
      }
    }
  }
}

// Callback untuk interrupt dari tombol
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t now = HAL_GetTick();

  if(GPIO_Pin == GPIO_PIN_0 && (now - last_interrupt_time_pb0 > debounce_delay))
  {
	// Tombol PA0 ditekan: ganti mode (0-3)
	mode = (mode + 1) % 4;
    last_interrupt_time_pb0 = now;
  }
  else if(GPIO_Pin == GPIO_PIN_1 && (now - last_interrupt_time_pb1 > debounce_delay))
  {
	 // Tombol PA1 ditekan: reset ke mode 0
    mode = 0;
    last_interrupt_time_pb1 = now;
  }
}

// Konfigurasi sistem clock
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

// Inisialisasi GPIO
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Aktifkan clock untuk GPIOC dan GPIOA
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Konfigurasi pin output untuk LED: PC13, PA5, PA6
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Konfigurasi pin input dengan interrupt: PA0 dan PA1
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Inisialisasi EXTI (interrupt)
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}
