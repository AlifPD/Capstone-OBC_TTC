/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include <stdio.h>
#include "w25qxx.h"
#include "dummy_data.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int RAND_NUM;
uint8_t buf[256];
int valid;
int error;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  W25qxx_Init();

  if(w25qxx.ID == 7){
	  printf("ID		--> W25Q64\n\r");
  }else{
	  printf("ID		--> Other\n\r");
  }

  printf("Capacity	--> %lu Bytes\n\r", (w25qxx.CapacityInKiloByte)*1024);
  printf("Block		--> %lu Block, %lu Bytes each\n\r", w25qxx.BlockCount, w25qxx.BlockSize);
  printf("Sector		--> %lu Sector, %lu Bytes each\n\r", w25qxx.SectorCount, w25qxx.SectorSize);
  printf("Page		--> %lu Page, %hu Bytes each\n\n\r", w25qxx.PageCount, w25qxx.PageSize);

  printf("Clearing NOR\n\r");
  for(int i=0; i<2048; i++){
	  W25qxx_EraseSector(i);
  }

  printf("Writing to NOR\n\r");
  for(int i=0; i<32768; i++){
	  srand(i);
	  RAND_NUM = rand() % 8;

	  switch(RAND_NUM){
	  	  case	0	:	W25qxx_WritePage(dum_1, i, 0, 256); break;
	  	  case	1	:	W25qxx_WritePage(dum_2, i, 0, 256); break;
	  	  case	2	:	W25qxx_WritePage(dum_3, i, 0, 256); break;
	  	  case	3	:	W25qxx_WritePage(dum_4, i, 0, 256); break;
	  	  case	4	:	W25qxx_WritePage(dum_5, i, 0, 256); break;
	  	  case	5	:	W25qxx_WritePage(dum_6, i, 0, 256); break;
	  	  case	6	:	W25qxx_WritePage(dum_7, i, 0, 256); break;
	  	  case	7	:	W25qxx_WritePage(dum_8, i, 0, 256); break;
	  }
  }
  printf("Data Written to NOR\n\n\r");

  printf("Validating Data\n\r");
  for(int i=0; i<32768; i++){
	  W25qxx_ReadPage(buf, i, 0, 256);
	  switch(buf[0]){
	  	  case	0x01 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_1[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;
	  	  case	0x02 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_2[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;
	  	  case	0x03 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_3[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;
	  	  case	0x04 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_4[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;
	  	  case	0x05 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_5[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;
	  	  case	0x06 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_6[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;
	  	  case	0x07 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_7[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;
	  	  case	0x08 :	for(int j=0; j<256; j++){
	  		  	  	  		if(buf[j] != dum_8[j]){
	  		  	  	  			printf("Data Number %d ERROR\n\r", i);
	  		  	  	  			error++;
	  		  	  	  			break;
	  		  	  	  		}
	  	  	  	  	  	}
	  	  	  	  	  	break;

	  }
	  printf("Data Number %d VALID\n\r", i);
	  valid++;
  }

  printf("\n\r+++ SUMMARY +++\n\r");
  printf("Data Error	: %d\n\r", error);
  printf("Data Valid	: %d\n\r", valid);
  printf("Error Rate	: %f\n\n\r", (error/valid)*100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 90;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SELECTOR_GPIO_Port, SELECTOR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : FLASH_CS_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SELECTOR_Pin */
  GPIO_InitStruct.Pin = SELECTOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SELECTOR_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
