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
// SUBSYSTEM ADDRESS
const uint8_t ADDR_MAIN	= 90;
const uint8_t ADDR_RDT	= 92;
const uint8_t ADDR_TTC	= 94;
const uint8_t ADDR_EPS	= 96;

// PAYLOAD ADDRESS
const uint8_t ADDR_ADSB	= 98;
const uint8_t ADDR_LORA	= 100;
const uint8_t ADDR_AIS	= 102;

// HEADER
const uint8_t HEAD_MAIN	= 0x7D; // OBC to ALL
const uint8_t HEAD_RDT	= 0x7E; // RDT to OBC
const uint8_t HEAD_TTC	= 0x7F; // TTC to OBC
const uint8_t HEAD_EPS	= 0x80; // EPS to OBC
const uint8_t HEAD_ADSB	= 0x81; // ADSB to OBC
const uint8_t HEAD_LORA	= 0x82; // LORA to OBC
const uint8_t HEAD_AIS	= 0x83; // AIS to OBC

// BUFFER
uint8_t ADDR_I2C		=	0xFF;	// BUFFER to store Address to send
uint8_t I2C_RX_BUF[55]; // BUFFER to Receive Data from other Payloads/Boards
uint8_t I2C_TX_BUF[191]; // BUFFER to Transmit Command and Data to other Payloads/Boards
uint8_t NOR_BUF[186];
int RAND_NUM;

// Status
HAL_StatusTypeDef RES_TX_I2C;
HAL_StatusTypeDef RES_RX_I2C;

// Dummy
uint8_t dum1[186] =
{	0xE4, 0x54, 0xF2, 0x26, 0x76, 0xDB, 0x67, 0x67,
    0xBA, 0xC7, 0x17, 0xF6, 0xFD, 0xEE, 0x04, 0x7D,
    0x58, 0x8A, 0x34, 0xB9, 0x2A, 0x7F, 0x25, 0xB1,
    0xE3, 0x30, 0xFA, 0x83, 0xDF, 0x0A, 0x14, 0x23,
    0x2E, 0xBE, 0xF1, 0xE1, 0xC9, 0x06, 0x81, 0xF3,
    0x92, 0x0D, 0x27, 0x1F, 0x61, 0x72, 0xCA, 0x13,
    0x88, 0x5A, 0x29, 0x1B, 0x85, 0x75, 0x97, 0xE5,
    0xCF, 0x1E, 0x40, 0x70, 0x89, 0x12, 0x08, 0xA2,
    0x82, 0xB5, 0x05, 0xA5, 0x41, 0x01, 0xA8, 0x18,
    0x60, 0xBB, 0xEA, 0x49, 0xAA, 0xFB, 0xD9, 0x01,
    0x50, 0x4C, 0x4C, 0x9F, 0x65, 0x96, 0x55, 0xC2,
    0xCB, 0x68, 0xB3, 0xCF, 0x6E, 0xDC, 0x0F, 0x90,
    0xD9, 0xA9, 0x5C, 0xEA, 0xDD, 0x8F, 0xB0, 0x8A,
    0xEF, 0xF9, 0x95, 0x94, 0x5E, 0xCF, 0x91, 0xD1,
    0xA7, 0xBB, 0xEA, 0x44, 0x42, 0x97, 0x5A, 0x2A,
    0x5E, 0x1E, 0x83, 0x99, 0xF4, 0xC2, 0x57, 0x9C,
    0xDE, 0x7D, 0x73, 0x7F, 0x0F, 0x48, 0x47, 0x6E,
    0xC4, 0x9B, 0x08, 0x42, 0xC5, 0x12, 0x2D, 0x10,
    0x30, 0xEF, 0xC5, 0x14, 0xE1, 0x6C, 0x0B, 0x54,
    0xF3, 0x76, 0x98, 0x94, 0xAB, 0xD9, 0xFF, 0xAD,
    0x35, 0x27, 0x3A, 0xD4, 0x98, 0xB1, 0x5F, 0xB5,
    0x3D, 0xF1, 0x6C, 0xE2, 0x12, 0x43, 0x8B, 0xD4,
    0xA3, 0xE2, 0x1D, 0x0B, 0x71, 0x18, 0x9D, 0xC1,
    0xC3, 0xE7
};

uint8_t dum2[186] =
{	0x8C, 0x0E, 0xF9, 0x6B, 0xD3, 0xC6, 0xC0, 0xA9,
    0xEB, 0x85, 0xDE, 0xA2, 0xDE, 0x56, 0x04, 0x45,
    0x70, 0x1D, 0x82, 0x50, 0xEE, 0x83, 0x65, 0x51,
    0x2A, 0x99, 0x6E, 0x64, 0x8C, 0x7D, 0x32, 0xAC,
    0xB8, 0xC7, 0x67, 0x9D, 0x5F, 0x95, 0xAC, 0x00,
    0x82, 0x8E, 0x36, 0xBB, 0x50, 0x9E, 0x69, 0x77,
    0x99, 0x79, 0xC6, 0x28, 0x6E, 0xA4, 0x15, 0x75,
    0x49, 0x28, 0x4A, 0x64, 0x3E, 0x55, 0x18, 0x89,
    0xC3, 0x22, 0xE0, 0x8D, 0xF4, 0xE4, 0xAC, 0xC6,
    0xD0, 0x77, 0xD5, 0x3B, 0x84, 0x83, 0xD5, 0xA4,
    0xF2, 0xB5, 0x37, 0x7B, 0x1B, 0x24, 0xD9, 0x28,
    0x19, 0x27, 0x1F, 0x03, 0xDF, 0x87, 0xBE, 0x90,
    0x5C, 0x06, 0x2C, 0x36, 0xAB, 0xD7, 0x4A, 0x39,
    0xC3, 0x7B, 0xBF, 0x3A, 0x50, 0xC0, 0xE7, 0xEC,
    0xC2, 0x86, 0x23, 0xBB, 0x26, 0x27, 0xA8, 0x1A,
    0xC2, 0x27, 0xA7, 0x7A, 0x70, 0x36, 0x2A, 0xD6,
    0x1E, 0x2F, 0x25, 0x16, 0x65, 0xE4, 0xE1, 0xF0,
    0x8F, 0xB0, 0xED, 0xEC, 0xFF, 0x0F, 0x20, 0x3C,
    0xD0, 0xE6, 0x50, 0x65, 0xB7, 0xF9, 0x65, 0x8A,
    0x6C, 0x4F, 0x47, 0x24, 0xD8, 0x69, 0xA2, 0x31,
    0x38, 0x1A, 0xE5, 0x88, 0xED, 0x04, 0x03, 0x93,
    0xC8, 0xF9, 0xE0, 0x7D, 0x78, 0x49, 0x94, 0x6B,
    0x07, 0xEA, 0xC9, 0x54, 0x7A, 0x4E, 0xB5, 0x22,
    0x44, 0x96
};

uint8_t dum3[186] =
{	0x2A, 0x53, 0x40, 0x46, 0x88, 0x4D, 0x67, 0x53,
    0xF3, 0x5D, 0x07, 0x7A, 0x3C, 0xA6, 0xEF, 0x06,
    0xA4, 0xE2, 0x29, 0xC2, 0xDB, 0x05, 0x68, 0xDE,
    0x20, 0x4A, 0x91, 0x7D, 0x96, 0x3E, 0x60, 0xC4,
    0xAC, 0x35, 0xCD, 0x90, 0x12, 0x5E, 0x09, 0x43,
    0x99, 0x6C, 0xB2, 0xF8, 0x4A, 0x05, 0x81, 0x7E,
    0xE3, 0xE1, 0x1C, 0xCB, 0xBF, 0x88, 0x09, 0x5E,
    0x98, 0xD1, 0x38, 0x2E, 0x69, 0x90, 0x4B, 0x2F,
    0x2E, 0xF4, 0x31, 0xAB, 0x2F, 0xD8, 0x63, 0x64,
    0x16, 0xDF, 0x1F, 0xE1, 0x6D, 0x93, 0x7D, 0x67,
    0x66, 0x24, 0x07, 0xF1, 0x65, 0x5B, 0x13, 0x51,
    0x06, 0xA1, 0xC2, 0x7E, 0xBD, 0x0C, 0x35, 0x06,
    0xE0, 0xF7, 0x96, 0x2E, 0x79, 0x75, 0x7F, 0xB7,
    0x70, 0x4C, 0xB6, 0xB3, 0xE2, 0x62, 0xDF, 0x42,
    0x10, 0xDE, 0x8C, 0x4E, 0x1F, 0x2A, 0xD6, 0xE9,
    0xB1, 0xDC, 0x21, 0xDB, 0xA1, 0x18, 0xAE, 0x2F,
    0xBA, 0xCC, 0x2F, 0x98, 0x66, 0xA4, 0xBD, 0xDE,
    0xEB, 0xB9, 0x35, 0x08, 0xC8, 0x10, 0xD5, 0xF5,
    0xFC, 0x4D, 0x5F, 0x67, 0xC2, 0x25, 0x9E, 0x4A,
    0x75, 0xFC, 0xE9, 0x99, 0x0F, 0xCD, 0x16, 0xB6,
    0xD5, 0x9C, 0x21, 0x5C, 0x98, 0x74, 0xEE, 0x82,
    0xC3, 0x86, 0xD5, 0xEA, 0xC9, 0x7E, 0xE8, 0x21,
    0xD5, 0x7D, 0x87, 0x18, 0xFF, 0x84, 0x4B, 0xEA,
    0xC0, 0xDF
};

uint8_t dum4[186] =
{	0xB1, 0x92, 0x6B, 0x2C, 0x36, 0x09, 0x29, 0xE0,
    0x13, 0x23, 0x58, 0xFA, 0xF3, 0x3A, 0x25, 0x87,
    0xCD, 0x9D, 0x92, 0x73, 0xDE, 0x07, 0x80, 0xFF,
    0x9D, 0x90, 0x6E, 0x34, 0x18, 0xC6, 0x5A, 0xFC,
    0xE5, 0x9E, 0xE4, 0xAE, 0xF8, 0xC7, 0xE6, 0x71,
    0xD6, 0xBE, 0xCC, 0x2E, 0x74, 0x13, 0x84, 0x0F,
    0x13, 0xC9, 0x55, 0xBC, 0xF3, 0x89, 0xFD, 0x54,
    0xCB, 0x4F, 0xA2, 0xBB, 0x74, 0xEE, 0xE4, 0x85,
    0xA0, 0x78, 0x49, 0x5E, 0xAC, 0xC1, 0xA8, 0x3F,
    0x64, 0xFD, 0x30, 0x9D, 0x70, 0x44, 0xB8, 0x5C,
    0x7D, 0x81, 0xF0, 0x35, 0xA2, 0x91, 0x3B, 0x7C,
    0xB3, 0x3C, 0xD8, 0xD5, 0x89, 0x2E, 0x3B, 0x25,
    0xA3, 0x74, 0x18, 0xCF, 0xF2, 0x59, 0xF0, 0xC9,
    0x9D, 0xBF, 0xE0, 0xB6, 0x3E, 0x7B, 0x77, 0x17,
    0xA9, 0x7B, 0xD9, 0x5E, 0x39, 0xBF, 0xA7, 0x2E,
    0xE0, 0x78, 0x1E, 0x7B, 0x9F, 0xB0, 0x74, 0xE2,
    0x04, 0xB7, 0x18, 0xC8, 0x1C, 0xAB, 0x03, 0x39,
    0x0E, 0x99, 0x55, 0x56, 0x1A, 0x56, 0x8A, 0xFC,
    0x9C, 0x10, 0x35, 0x32, 0x50, 0x57, 0x7A, 0x43,
    0x0A, 0xDF, 0xDA, 0x3E, 0x62, 0x4A, 0xEA, 0x47,
    0xF4, 0xAA, 0xBB, 0x31, 0x00, 0xB1, 0xCA, 0x79,
    0x5F, 0xEA, 0x7D, 0x66, 0x90, 0xA4, 0x7E, 0x22,
    0x48, 0xEF, 0xB8, 0xC9, 0xC9, 0x45, 0x11, 0xB6,
    0xC3, 0x9F
};

uint8_t dum5[186] =
{	0x18, 0x98, 0xAF, 0x64, 0x1E, 0xBE, 0xEF, 0xEF,
    0xA1, 0x3A, 0xF2, 0x98, 0x35, 0xA2, 0x7F, 0xF2,
    0xDB, 0x99, 0xEC, 0x22, 0x4F, 0x5D, 0x32, 0x75,
    0x77, 0x6F, 0xE5, 0xDA, 0x9E, 0x41, 0x8F, 0xC0,
    0x71, 0x34, 0x12, 0xB4, 0xF4, 0x3B, 0xAE, 0x07,
    0xA1, 0xA2, 0x71, 0x69, 0x83, 0xCE, 0xAF, 0x51,
    0x46, 0x38, 0xA5, 0x97, 0x43, 0x8D, 0x01, 0x2F,
    0x4F, 0xBB, 0xBA, 0xB0, 0x97, 0x93, 0xA8, 0x0B,
    0xA8, 0xFD, 0x7E, 0xCE, 0x44, 0xC9, 0xBE, 0x2B,
    0x6F, 0x17, 0x5A, 0xF3, 0x37, 0xE8, 0x50, 0xF9,
    0xD8, 0x5F, 0x0F, 0x84, 0x08, 0x1B, 0x01, 0x5D,
    0x3C, 0x8F, 0xF3, 0x0B, 0x71, 0xCC, 0x11, 0x8B,
    0xB4, 0x0C, 0x9D, 0x55, 0xDD, 0xE5, 0x78, 0x8E,
    0x09, 0xFC, 0xEE, 0xC0, 0x91, 0x5D, 0x53, 0x80,
    0x72, 0x0A, 0xF4, 0xE4, 0xEA, 0xF3, 0x49, 0x06,
    0x61, 0xEB, 0x54, 0xC5, 0x67, 0x3D, 0x23, 0x6D,
    0x33, 0xE2, 0xFB, 0x73, 0x97, 0x38, 0x59, 0x4F,
    0xEB, 0xA2, 0xA4, 0x74, 0xE8, 0xFB, 0x84, 0x2E,
    0xD3, 0x33, 0x65, 0x23, 0xF7, 0x93, 0x67, 0x97,
    0x84, 0xC4, 0xB3, 0x95, 0x93, 0x2F, 0x6B, 0x95,
    0x65, 0xC7, 0xD4, 0x17, 0x6C, 0x92, 0x09, 0x66,
    0x27, 0xB1, 0x30, 0xF0, 0x48, 0x56, 0xD0, 0x47,
    0xCA, 0x41, 0x51, 0x43, 0x2F, 0xD4, 0x28, 0x3A,
    0xE3, 0x90
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);
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

void ParseCMD();
void Clr_I2C_TX_BUF();
void Clr_I2C_RX_BUF();
void Clr_I2C_ADDR();
void Clr_NOR_BUF();
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
	Clr_I2C_RX_BUF();
	Clr_I2C_TX_BUF();
	Clr_NOR_BUF();
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  W25qxx_Init();
  W25qxx_WritePage(dum1, 0, 0, 186);
  W25qxx_WritePage(dum2, 1, 0, 186);
  W25qxx_WritePage(dum3, 2, 0, 186);
  W25qxx_WritePage(dum4, 3, 0, 186);
  W25qxx_WritePage(dum5, 4, 0, 186);

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  printf("Waiting Data from Ground\r\n");
	  if(HAL_I2C_Master_Receive(&hi2c1, ADDR_TTC<<1, I2C_RX_BUF, 55, HAL_MAX_DELAY) == HAL_OK){
		  printf("Data Received by OBC from Ground: [");
		  for(int i=0; i<sizeof(I2C_RX_BUF); i++){
			  printf(" %d", I2C_RX_BUF[i]);
		  }
		  printf("]\n\r");

		  if(I2C_RX_BUF[4] == 1){
			  ParseCMD();

			  printf("Address to Send Data: %d\n\r", ADDR_I2C);

			  printf("Data Transmitted by OBC: [");
			  for(int i=0; i<sizeof(I2C_TX_BUF); i++){
				  printf(" %d", I2C_TX_BUF[i]);
			  }
			  printf("]\n\r");

			  if(HAL_I2C_Master_Transmit(&hi2c1, ADDR_TTC<<1, I2C_TX_BUF, 191, HAL_MAX_DELAY) == HAL_OK){
				  printf("Data Returned to Ground\n\r");

				  if(ADDR_I2C == ADDR_RDT){
					  HAL_GPIO_WritePin(MUX_SEL_GPIO_Port, MUX_SEL_Pin, GPIO_PIN_SET);
					  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
					  HAL_SuspendTick();
					  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
				  }
			  }
			  Clr_I2C_TX_BUF();
		  }
	  } else {
		  printf("NO DATA RECEIVED, Continue Waiting\n\r");
	  }
	  Clr_I2C_RX_BUF();
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
  hi2c1.Init.OwnAddress1 = ADDR_MAIN;
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MUX_SEL_GPIO_Port, MUX_SEL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_CS_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MUX_SEL_Pin */
  GPIO_InitStruct.Pin = MUX_SEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MUX_SEL_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void ParseCMD(){
	I2C_TX_BUF[0] = HEAD_MAIN;

	if(I2C_RX_BUF[5] == 0x80){
		ADDR_I2C = ADDR_EPS;
		if(I2C_RX_BUF[6] == 1){
			switch(I2C_RX_BUF[7]){
				case 1	:	I2C_TX_BUF[1] = 1;
							I2C_TX_BUF[2] = 1;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 1;
							break;

				case 2	:	I2C_TX_BUF[1] = 1;
							I2C_TX_BUF[2] = 2;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 1;
							break;

				case 3	:	I2C_TX_BUF[1] = 1;
							I2C_TX_BUF[2] = 3;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 1;
							break;

				case 4	:	I2C_TX_BUF[1] = 1;
							I2C_TX_BUF[2] = 4;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 1;
							break;
			}
		}
		else if(I2C_RX_BUF[6] == 2){
			switch(I2C_RX_BUF[7]){
				case 1	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 1;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[16];
							I2C_TX_BUF[6] = NOR_BUF[17];
							I2C_TX_BUF[7] = NOR_BUF[18];
							I2C_TX_BUF[8] = NOR_BUF[19];
							break;

				case 2	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 2;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[20];
							I2C_TX_BUF[6] = NOR_BUF[21];
							I2C_TX_BUF[7] = NOR_BUF[22];
							I2C_TX_BUF[8] = NOR_BUF[23];
							break;

				case 3	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 3;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[24];
							I2C_TX_BUF[6] = NOR_BUF[25];
							I2C_TX_BUF[7] = NOR_BUF[26];
							I2C_TX_BUF[8] = NOR_BUF[27];
							break;

				case 4	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 4;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[28];
							I2C_TX_BUF[6] = NOR_BUF[29];
							I2C_TX_BUF[7] = NOR_BUF[30];
							I2C_TX_BUF[8] = NOR_BUF[31];
							break;

				case 5	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 5;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[32];
							I2C_TX_BUF[6] = NOR_BUF[33];
							I2C_TX_BUF[7] = NOR_BUF[34];
							I2C_TX_BUF[8] = NOR_BUF[35];
							break;

				case 6	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 6;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[8];
							I2C_TX_BUF[6] = NOR_BUF[9];
							I2C_TX_BUF[7] = NOR_BUF[10];
							I2C_TX_BUF[8] = NOR_BUF[11];
							break;

				case 7	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 7;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[12];
							I2C_TX_BUF[6] = NOR_BUF[13];
							I2C_TX_BUF[7] = NOR_BUF[14];
							I2C_TX_BUF[8] = NOR_BUF[15];
							break;
			}
		}
	}

	else if(I2C_RX_BUF[5] == 0x7F){
		ADDR_I2C = ADDR_TTC<<1;

		if(I2C_RX_BUF[6] == 3){
			switch(I2C_RX_BUF[7]){
				case 1	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 1;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							for(int i=0; i<186; i++){
								I2C_TX_BUF[i+5] = NOR_BUF[i];
							}
							break;
			}
		}
		else if(I2C_RX_BUF[6] == 4){
			switch(I2C_RX_BUF[7]){
				case 1	:	I2C_TX_BUF[1] = 2;
							I2C_TX_BUF[2] = 1;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							I2C_TX_BUF[5] = NOR_BUF[4];
							I2C_TX_BUF[6] = NOR_BUF[5];
							I2C_TX_BUF[7] = NOR_BUF[6];
							I2C_TX_BUF[8] = NOR_BUF[7];
							break;
			}
		}
	}

	else if(I2C_RX_BUF[5] == 0x7E){
		ADDR_I2C = ADDR_RDT;

		if(I2C_RX_BUF[6] == 5){
			switch(I2C_RX_BUF[7]){
				case 1 :	I2C_TX_BUF[1] = 1;
							I2C_TX_BUF[2] = 1;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							break;
			}
		}
	}

	else if(I2C_RX_BUF[5] == 0x81){
		ADDR_I2C = ADDR_ADSB;

		if(I2C_RX_BUF[6] == 6){
			switch(I2C_RX_BUF[7]){
				case 1 :	I2C_TX_BUF[1] = 1;
							I2C_TX_BUF[2] = 1;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 1;
							break;

				case 2 :	I2C_TX_BUF[1] = 1;
							I2C_TX_BUF[2] = 2;
							I2C_TX_BUF[3] = 0;
							I2C_TX_BUF[4] = 0;
							srand(I2C_RX_BUF[7]);
							RAND_NUM = rand() % 5;
							W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
							for(int i=0; i<46; i++){
								I2C_TX_BUF[i+5] = NOR_BUF[i+40];
							}
							break;
			}
		}
		else if(I2C_RX_BUF[6] == 7){
			switch(I2C_RX_BUF[7]){
			case 1 :	I2C_TX_BUF[1] = 2;
						I2C_TX_BUF[2] = 1;
						I2C_TX_BUF[3] = 0;
						I2C_TX_BUF[4] = 0;
						srand(I2C_RX_BUF[7]);
						RAND_NUM = rand() % 5;
						W25qxx_ReadPage(NOR_BUF, RAND_NUM, 0, 186);
						I2C_TX_BUF[5] = NOR_BUF[32];
						I2C_TX_BUF[6] = NOR_BUF[33];
						I2C_TX_BUF[7] = NOR_BUF[34];
						I2C_TX_BUF[8] = NOR_BUF[35];
						break;
			}
		}
	}
}

void Clr_I2C_TX_BUF(){
	for(int i=0; i<sizeof(I2C_TX_BUF); i++){
		I2C_TX_BUF[i] = 0xFF;
	}
}

void Clr_I2C_RX_BUF(){
	for(int i=0; i<sizeof(I2C_RX_BUF); i++){
		I2C_RX_BUF[i] = 0xFF;
	}
}

void Clr_I2C_ADDR(){
	ADDR_I2C = 0xFF;
}

void Clr_NOR_BUF(){
	for(int i=0; i<sizeof(NOR_BUF); i++){
		NOR_BUF[i] = 0xFF;
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
