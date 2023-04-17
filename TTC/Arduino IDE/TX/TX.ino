#include <RH_RF24.h> // include Si446x.h library
// #include <RHSoftwareSPI.h>

#define Max_Packet_Size 128
#define rf_receive PA12
#define rf_transmit PB3
#define power_hpa PA1

char message[Max_Packet_Size]; // Test Message
char *message_ptr = &message[Max_Packet_Size]; // Pointer for variable "message"

static int counter; // Variable to count number of transmitted data

// RHSoftwareSPI spi2;
RH_RF24 rf4463(PA4, PB1, PB0); // Initialize RF4463 Object

// Main Setup function
void setup() {
  // spi2.setPins(PB14, PB15, PB13);
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
  sprintf(message, "TTC Board Test, Message number: %d", counter+1);
  Serial.print(F("Sending Data..."));
  Serial.println(message);
  

  if(rf4463.send((uint8_t*)message, sizeof(message))){
    counter++;
  }else{
    Serial.println(F("Transmit Failed"));
  }

  Serial.print(F("Totals : "));
  Serial.print(counter);
  Serial.println(" Data");
  Serial.println("+++++++");
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