#include <Wire.h>

#define ADDRESS_TTC 94

uint8_t i2cTxBuf[8] = {1,2,3,4,5,6,7,1};
int mesCount = 1;

void setup() {
  SystemClock_Config();

  Wire.begin(ADDRESS_TTC);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Wire.setTimeout(1000);


  Serial.begin(115200);
}

void receiveEvent(int howMany) {
// Print to Serial Monitor
  Serial.print("Data Received from I2C Master ... [");
  // Read while data received
  while (0 < Wire.available()) {
    for(int i=0; i<8; i++){
      byte x = Wire.read();
      Serial.print(x);
      if(i != 7){
        Serial.print(", ");
      } else {
        Serial.println("]");
      }
    }
  }
}

void requestEvent(){
  Serial.println("Request Sended"+mesCount);
  for(int i = 0; i<8; i++){
    Wire.write(i2cTxBuf[i]);
  }
  i2cTxBuf[7]++;
  mesCount++;
}

void loop() {
  Serial.println("I2C Test");
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}