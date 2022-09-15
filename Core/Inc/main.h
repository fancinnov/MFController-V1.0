/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI_EXT_SCK_Pin GPIO_PIN_2
#define SPI_EXT_SCK_GPIO_Port GPIOE
#define SPI_EXT_CS_Pin GPIO_PIN_3
#define SPI_EXT_CS_GPIO_Port GPIOE
#define SPI_EXT_NSS_Pin GPIO_PIN_4
#define SPI_EXT_NSS_GPIO_Port GPIOE
#define SPI_EXT_MISO_Pin GPIO_PIN_5
#define SPI_EXT_MISO_GPIO_Port GPIOE
#define SPI_EXT_MOSI_Pin GPIO_PIN_6
#define SPI_EXT_MOSI_GPIO_Port GPIOE
#define FMU_GPIO1_Pin GPIO_PIN_14
#define FMU_GPIO1_GPIO_Port GPIOC
#define FMU_GPIO2_Pin GPIO_PIN_15
#define FMU_GPIO2_GPIO_Port GPIOC
#define FMU_GPIO3_Pin GPIO_PIN_0
#define FMU_GPIO3_GPIO_Port GPIOC
#define FMU_GPIO4_Pin GPIO_PIN_1
#define FMU_GPIO4_GPIO_Port GPIOC
#define FMU_OE1_Pin GPIO_PIN_2
#define FMU_OE1_GPIO_Port GPIOC
#define SUB_RESET_Pin GPIO_PIN_3
#define SUB_RESET_GPIO_Port GPIOC
#define UART4_TX_Pin GPIO_PIN_0
#define UART4_TX_GPIO_Port GPIOA
#define UART4_RX_Pin GPIO_PIN_1
#define UART4_RX_GPIO_Port GPIOA
#define BAT_C_SENS_Pin GPIO_PIN_2
#define BAT_C_SENS_GPIO_Port GPIOA
#define BAT_V_SENS_Pin GPIO_PIN_3
#define BAT_V_SENS_GPIO_Port GPIOA
#define P_SENS_Pin GPIO_PIN_4
#define P_SENS_GPIO_Port GPIOA
#define FRAM_SCK_Pin GPIO_PIN_5
#define FRAM_SCK_GPIO_Port GPIOA
#define FRAM_MISO_Pin GPIO_PIN_6
#define FRAM_MISO_GPIO_Port GPIOA
#define FRAM_MOSI_Pin GPIO_PIN_7
#define FRAM_MOSI_GPIO_Port GPIOA
#define FRAM_CS_Pin GPIO_PIN_4
#define FRAM_CS_GPIO_Port GPIOC
#define VDD_5V_SENS_Pin GPIO_PIN_5
#define VDD_5V_SENS_GPIO_Port GPIOC
#define LED_BLUE_Pin GPIO_PIN_0
#define LED_BLUE_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_1
#define LED_RED_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOB
#define FMU_LED1_Pin GPIO_PIN_9
#define FMU_LED1_GPIO_Port GPIOE
#define FMU_LED2_Pin GPIO_PIN_10
#define FMU_LED2_GPIO_Port GPIOE
#define FMU_LED3_Pin GPIO_PIN_11
#define FMU_LED3_GPIO_Port GPIOE
#define FMU_LED4_Pin GPIO_PIN_12
#define FMU_LED4_GPIO_Port GPIOE
#define FMU_LED5_Pin GPIO_PIN_13
#define FMU_LED5_GPIO_Port GPIOE
#define ALARM_Pin GPIO_PIN_14
#define ALARM_GPIO_Port GPIOE
#define SDIO_IPS_Pin GPIO_PIN_15
#define SDIO_IPS_GPIO_Port GPIOE
#define FMU_LED6_Pin GPIO_PIN_10
#define FMU_LED6_GPIO_Port GPIOB
#define FMU_LED7_Pin GPIO_PIN_11
#define FMU_LED7_GPIO_Port GPIOB
#define FMU_GPIO8_Pin GPIO_PIN_10
#define FMU_GPIO8_GPIO_Port GPIOD
#define FMU_GPIO7_Pin GPIO_PIN_11
#define FMU_GPIO7_GPIO_Port GPIOD
#define FMU_GPIO6_Pin GPIO_PIN_12
#define FMU_GPIO6_GPIO_Port GPIOD
#define FMU_GPIO5_Pin GPIO_PIN_13
#define FMU_GPIO5_GPIO_Port GPIOD
#define VDD_3V3_SENSORS_EN_Pin GPIO_PIN_14
#define VDD_3V3_SENSORS_EN_GPIO_Port GPIOD
#define WIFI_EN_Pin GPIO_PIN_15
#define WIFI_EN_GPIO_Port GPIOD
#define WIFI_TX_Pin GPIO_PIN_6
#define WIFI_TX_GPIO_Port GPIOC
#define WIFI_RX_Pin GPIO_PIN_7
#define WIFI_RX_GPIO_Port GPIOC
#define SDIO_D0_Pin GPIO_PIN_8
#define SDIO_D0_GPIO_Port GPIOC
#define SDIO_D1_Pin GPIO_PIN_9
#define SDIO_D1_GPIO_Port GPIOC
#define FMU_OE2_Pin GPIO_PIN_8
#define FMU_OE2_GPIO_Port GPIOA
#define USB_FS_VBUS_Pin GPIO_PIN_9
#define USB_FS_VBUS_GPIO_Port GPIOA
#define USB_RESET_Pin GPIO_PIN_10
#define USB_RESET_GPIO_Port GPIOA
#define USB_FS_DM_Pin GPIO_PIN_11
#define USB_FS_DM_GPIO_Port GPIOA
#define USB_FS_DP_Pin GPIO_PIN_12
#define USB_FS_DP_GPIO_Port GPIOA
#define FMU_SWDIO_Pin GPIO_PIN_13
#define FMU_SWDIO_GPIO_Port GPIOA
#define FMU_SWCLK_Pin GPIO_PIN_14
#define FMU_SWCLK_GPIO_Port GPIOA
#define SDIO_D2_Pin GPIO_PIN_10
#define SDIO_D2_GPIO_Port GPIOC
#define SDIO_D3_Pin GPIO_PIN_11
#define SDIO_D3_GPIO_Port GPIOC
#define SDIO_CK_Pin GPIO_PIN_12
#define SDIO_CK_GPIO_Port GPIOC
#define SDIO_CMD_Pin GPIO_PIN_2
#define SDIO_CMD_GPIO_Port GPIOD
#define CAN1_S_Pin GPIO_PIN_3
#define CAN1_S_GPIO_Port GPIOD
#define CAN2_S_Pin GPIO_PIN_4
#define CAN2_S_GPIO_Port GPIOB
#define I2C_EXT_SCL_Pin GPIO_PIN_8
#define I2C_EXT_SCL_GPIO_Port GPIOB
#define I2C_EXT_SDA_Pin GPIO_PIN_9
#define I2C_EXT_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
