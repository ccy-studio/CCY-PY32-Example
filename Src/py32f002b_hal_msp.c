/*
 * @Description: 
 * @Blog: saisaiwa.com
 * @Author: ccy
 * @Date: 2024-06-20 09:37:45
 * @LastEditTime: 2024-06-20 16:51:41
 */
/**
  ******************************************************************************
  * @file    py32f002b_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
  * @brief Initialize global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief Initialize MSP for USART.
  * @param huart：Initialize the handle of USART
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();
  
  /* GPIO initialization
  PA4：RX,
  PA3：TX
  */
  GPIO_InitStruct.Pin       = GPIO_PIN_4;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* USART1 interrupt enable */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}



/**
  * @brief Initialize TIM MSP
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /* Enable TIM1 Clock */
  __HAL_RCC_TIM14_CLK_ENABLE();                              
  /* Enable GPIOB Clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();                             
  /* Mode AF-PP */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                   
  /* PULLUP */
  GPIO_InitStruct.Pull = GPIO_PULLUP;                       
  /* SPEED HIGH */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  /* GPIO PB5 Initialize*/
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Alternate = GPIO_AF5_TIM14;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
