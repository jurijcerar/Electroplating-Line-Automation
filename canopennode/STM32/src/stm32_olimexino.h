/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_OLIMEXINO_H
#define __STM32_OLIMEXINO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"


/* CAN Bus defines for STM32F10x */
#define RCC_APB2Periph_GPIO_CAN1    RCC_APB2Periph_GPIOB
#define GPIO_Remapping_CAN1         GPIO_Remap1_CAN1
#define GPIO_CAN1                   GPIOB
#define GPIO_Pin_CAN1_RX            GPIO_Pin_8
#define GPIO_Pin_CAN1_TX            GPIO_Pin_9

/** @defgroup OLIMEXINO_Exported_Types
  * @{
  */
  typedef enum 
  {
    LED1 = 0,
    LED2 = 1
  } Led_TypeDef;

  typedef enum 
  {
    COM1 = 0,
    COM2 = 1
  } COM_TypeDef;   
/**
  * @}
  */

  /** @addtogroup OLIMEXINO_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             2

#define LED1_PIN                         GPIO_Pin_5
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOA 
  
#define LED2_PIN                         GPIO_Pin_1
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOA  


/**
  * @}
  */

/** @addtogroup OLIMEXINO_LOW_LEVEL_COM
  * @{
  */
  #define COMn                             2
  
/**
 * @brief Definition for COM port1, connected to USART1
 */ 
  #define EVAL_COM1                        USART1
  #define EVAL_COM1_CLK                    RCC_APB2Periph_USART1
  #define EVAL_COM1_TX_PIN                 GPIO_Pin_9
  #define EVAL_COM1_TX_GPIO_PORT           GPIOA
  #define EVAL_COM1_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
  #define EVAL_COM1_RX_PIN                 GPIO_Pin_10
  #define EVAL_COM1_RX_GPIO_PORT           GPIOA
  #define EVAL_COM1_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
  #define EVAL_COM1_IRQn                   USART1_IRQn
  
/**
 * @brief Definition for COM port2, connected to USART2
 */ 
  #define EVAL_COM2                        USART2
  #define EVAL_COM2_CLK                    RCC_APB1Periph_USART2
  #define EVAL_COM2_TX_PIN                 GPIO_Pin_2
  #define EVAL_COM2_TX_GPIO_PORT           GPIOA
  #define EVAL_COM2_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
  #define EVAL_COM2_RX_PIN                 GPIO_Pin_3
  #define EVAL_COM2_RX_GPIO_PORT           GPIOA
  #define EVAL_COM2_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
  #define EVAL_COM2_IRQn                   USART2_IRQn
  
/**
  * @}
  */ 

/** @defgroup OLIMEXINO_LOW_LEVEL_Exported_Functions
  * @{
  */ 
  void LEDInit(Led_TypeDef Led);
  void LEDOn(Led_TypeDef Led);
  void LEDOff(Led_TypeDef Led);
  void LEDToggle(Led_TypeDef Led);

  void ADCInit(void);
  void ADCConfigure(void);
  uint16_t ReadADC(uint8_t ADC_Channel);

  void COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
/**
  * @}
  */
#endif /* __STM32_OLIMEXINO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
