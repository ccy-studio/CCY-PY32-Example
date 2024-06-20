/**
 ******************************************************************************
 * @file    main.c
 * @author  MCU Application Team
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
 * All rights reserved.</center></h2>
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UartHandle;
TIM_HandleTypeDef TimHandle;
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void App_GpioInit();
static void APP_TimConfig(void);
static void App_GpioExti();

/**
 * @brief  Application Entry Function.
 * @retval int
 */
int main(void) {
    /* Reset of all peripherals, Initializes the Systick. */
    HAL_Init();

    /* System clock configuration */
    APP_SystemClockConfig();

    /* USART initialization */
    UartHandle.Instance = USART1;
    UartHandle.Init.BaudRate = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&UartHandle);

    App_GpioInit();
    App_GpioExti();
    APP_TimConfig();

    if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK) {
        APP_ErrorHandler();
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)"Hello World!\n", 14);
    /* infinite loop */
    while (1) {
        HAL_Delay(500);
        // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    printf("GPIO进入按键中断\n");
    if (GPIO_Pin == GPIO_PIN_0) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    }
}

static void App_GpioInit() {
    GPIO_InitTypeDef gpio;
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_1;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOA, &gpio);
}

static void App_GpioExti() {
    GPIO_InitTypeDef gpio;

    gpio.Pin = GPIO_PIN_0;
    gpio.Mode = GPIO_MODE_IT_FALLING;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOA, &gpio);

    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
}

/**
 * @brief  TIM Configure
 * @param  None
 * @retval None
 */
static void APP_TimConfig(void) {
    // 24000000/100/24=10,000Hz.
    /* TIM1 */
    TimHandle.Instance = TIM14;

    /* Period = 2000 - 1  */
    TimHandle.Init.Period = 100 - 1;

    /* Prescaler = 1200 - 1 */
    TimHandle.Init.Prescaler = 24 - 1;

    /* ClockDivision = 0  */
    TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    /* Counter direction = Up */
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

    /* Repetition = 0 */
    TimHandle.Init.RepetitionCounter = 1 - 1;

    /* Auto-reload register not buffered  */
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK) {
        APP_ErrorHandler();
    }

    TIM_OC_InitTypeDef sConfig;

    /* Set output compare mode: PWM1  */
    sConfig.OCMode = TIM_OCMODE_PWM1;

    /* OC channel output high level effective */
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

    /* Disable OC FastMode */
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;

    /* OCN channel output high level effective */
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;

    /* Idle state OC1N output low level */
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    /* Idle state OC1 output low level*/
    sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;

    sConfig.Pulse = 50;

    /* Channel 1 configuration */
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) !=
        HAL_OK) {
        APP_ErrorHandler();
    }
}

/**
 * @brief  System clock configuration function
 * @param  None
 * @retval None
 */
static void APP_SystemClockConfig(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Oscillator configuration */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI |
        RCC_OSCILLATORTYPE_LSI |
        RCC_OSCILLATORTYPE_LSE; /* Select oscillator HSE, HSI, LSI, LSE */
    RCC_OscInitStruct.HSIState = RCC_HSI_ON; /* Enable HSI */
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1; /* HSI 1 frequency division */
    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_24MHz; /* Configure HSI clock 24MHz */
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS_DISABLE; /* Close HSE bypass */
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;            /* Close LSI */
    /*RCC_OscInitStruct.LSICalibrationValue = RCC_LSICALIBRATION_32768Hz;*/
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF; /* Close LSE */
    /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
    /* Configure oscillator */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        APP_ErrorHandler();
    }

    /* Clock source configuration */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1; /* Choose to configure clock HCLK, SYSCLK, PCLK1 */
    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_HSISYS; /* Select HSISYS as the system clock */
    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;                              /* AHB clock 1 division */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; /* APB clock 1 division */
    /* Configure clock source */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        APP_ErrorHandler();
    }
}

/**
 * @brief  Error executing function.
 * @param  None
 * @retval None
 */
void APP_ErrorHandler(void) {
    while (1) {
    }
}

/**
 * 设置串口支持printf函数输出内容
 */
#ifdef HAL_UART_MODULE_ENABLED

#if (defined(__CC_ARM)) || \
    (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
/**
 * @brief  writes a character to the usart
 * @param  ch
 *         *f
 * @retval the character
 */
int fputc(int ch, FILE* f) {
    /* Send a byte to USART */
    HAL_UART_Transmit(&UartHandle, (uint8_t*)&ch, 1, 1000);

    return (ch);
}

/**
 * @brief  get a character from the usart
 * @param  *f
 * @retval a character
 */
int fgetc(FILE* f) {
    int ch;
    HAL_UART_Receive(&UartHandle, (uint8_t*)&ch, 1, 1000);
    return (ch);
}

#elif defined(__ICCARM__)
/**
 * @brief  writes a character to the usart
 * @param  ch
 *         *f
 * @retval the character
 */
int putchar(int ch) {
    /* Send a byte to USART */
    HAL_UART_Transmit(&UartHandle, (uint8_t*)&ch, 1, 1000);

    return (ch);
}
#elif defined(__GNUC__)
/**
 * @brief  writes a character to the usart
 * @param  ch
 * @retval the character
 */
int __io_putchar(int ch) {
    /* Send a byte to USART */
    HAL_UART_Transmit(&UartHandle, (uint8_t*)&ch, 1, 1000);

    return ch;
}

int _write(int file, char* ptr, int len) {
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        __io_putchar(*ptr++);
    }
    return len;
}
#endif

#endif

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* Users can add their own printing information as needed,
       for example: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* Infinite loop */
    while (1) {
    }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
