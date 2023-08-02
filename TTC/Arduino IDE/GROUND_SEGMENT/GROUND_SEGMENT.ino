#include <RH_RF24.h> // include Si446x.h library

#define RF_RX_SWC PA12
#define RF_TX_SWC PB3
#define HPA_PWR PA1

RH_RF24 rf4463(PA4, PB1, PB0); // Initialize RF4463 Object

uint8_t Max_Packet_Size = 128;
uint8_t RF_TX_BUF[5] = {0xFF}; // Test Message

static int counter; // Variable to count number of transmitted data

// Main Setup function
void setup() {
  SystemClock_Config();

  pinMode(RF_RX_SWC, OUTPUT); digitalWrite(RF_RX_SWC, LOW);
  pinMode(RF_TX_SWC, OUTPUT); digitalWrite(RF_TX_SWC, HIGH);
  pinMode(HPA_PWR, OUTPUT);   digitalWrite(HPA_PWR, HIGH);

  Serial.begin(115200);

  while(!rf4463.init())
    Serial.println("Init Failed");
  Serial.println("Init Success");  
  rf4463.setTxPower(0x7F);                                     
}

// Main Loop function
void loop() {
  Serial.print("Ground Segment Test, Data Transmit: ");
  for(int i=0; i<sizeof(RF_TX_BUF); i++){
    Serial.print(RF_TX_BUF[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  if(rf4463.send(RF_TX_BUF, sizeof(RF_TX_BUF))){
    counter++;
  }else{
    Serial.println("Transmit Failed");
  }

  Serial.print("Totals : ");
  Serial.print(counter);
  Serial.println(" Data");
  Serial.println("+++++++");

  delay(500);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}