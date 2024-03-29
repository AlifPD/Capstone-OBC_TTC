#include <RH_RF24.h>
#include <RHSoftwareSPI.h>

#define rf_receive PA12
#define rf_transmit PB3
#define power_hpa PA1

RHSoftwareSPI spi2;

uint8_t buf[191];
uint8_t len = sizeof(buf);

static int counter;
static int invalids; 


RH_RF24 rf4463(PB12, PA11, PA8, spi2);

void setup() {
  SystemClock_Config();

  spi2.setPins(PB14, PB15, PB13);

  pinMode(rf_receive, OUTPUT);
  pinMode(rf_transmit, OUTPUT);
  pinMode(power_hpa, OUTPUT);

  digitalWrite(rf_receive, HIGH);
  digitalWrite(rf_transmit, LOW);
  digitalWrite(power_hpa, LOW);

  Serial.begin(115200);

  if (!rf4463.init())
    Serial.println("Init Failed");
  Serial.println("Init Success");
  rf4463.setModeRx();

  Clr_Buf();
}

void loop() {
  if (rf4463.recv(buf, &len)) {
    Serial.print("Tries : ");
    Serial.print(counter);
    Serial.print(" --> Received Message: ");
    
    for(int i=0; i<191; i++){
    Serial.print(" ");
    Serial.print(buf[i], HEX);
    }
    Serial.println();
    
    HAL_NVIC_SystemReset();
  } else {
    Serial.print("Tries : ");
    Serial.print(counter);
    Serial.println(" --> recv failed");
    invalids++;
  }
  counter++;
  Clr_Buf();

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

void Clr_Buf(){
  for(int i=0; i<191; i++){
    buf[i] = 0xFF;
  }
}