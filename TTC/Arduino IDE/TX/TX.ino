#include <RH_RF24.h> // include Si446x.h library

#define Max_Packet_Size 128
#define rf_receive PA12
#define rf_transmit PB3
#define power_hpa PA1

uint8_t message[8] = {1,2,3,4,5,6,7,0}; // Test Message
static int counter; // Variable to count number of transmitted data

RH_RF24 rf4463(PA4, PB1, PB0); // Initiali ze RF4463 Object

// Main Setup function
void setup() {
  SystemClock_Config();

  pinMode(rf_receive, OUTPUT);
  pinMode(rf_transmit, OUTPUT);
  pinMode(power_hpa, OUTPUT);

  digitalWrite(rf_receive, LOW);
  digitalWrite(rf_transmit, HIGH);
  digitalWrite(power_hpa, HIGH);

  Serial.begin(115200);

  while (!rf4463.init())
    Serial.println("Init Failed");
  Serial.println("Init Success");  
  rf4463.setTxPower(0x7F);                                     
}

// Main Loop function
void loop() {
  Serial.print("TTC Test, Data: ");
  for(int i=0; i<8; i++){
    Serial.print(message[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  if(rf4463.send(message, sizeof(message))){
    counter++;
    message[7]++;
  }else{
    Serial.println(F("Transmit Failed"));
  }

  Serial.print(F("Totals : "));
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