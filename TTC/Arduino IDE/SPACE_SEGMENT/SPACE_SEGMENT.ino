#include <RH_RF24.h>  // include Si446x.h library
#include <RHSoftwareSPI.h>
#include <Wire.h>

#define RF_RX_SWC PA12
#define RF_TX_SWC PB3
#define HPA_PWR PA1

RHSoftwareSPI spi2;
RH_RF24 rf4463(PB12, PA11, PA8, spi2);  // Initialize RF4463 Object

uint8_t ADDRESS_TTC = 94;
uint8_t I2C_TX_BUF[60] = {0xFF};
uint8_t I2C_RX_BUF[191] = {0xFF};

uint8_t Max_Packet_Size = 128;
uint8_t RF_RX_BUF[5] = {0xFF};
uint8_t len_RF_RX_BUF = sizeof(RF_RX_BUF);
 
static int counter;   // Variable to count number of received data
static int invalids;  // Variable to count number of failed received data

// Main Setup function
void setup() {
  SystemClock_Config();

  spi2.setPins(PB14, PB15, PB13);

  Wire.begin(ADDRESS_TTC);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  pinMode(RF_RX_SWC, OUTPUT); digitalWrite(RF_RX_SWC, HIGH);
  pinMode(RF_TX_SWC, OUTPUT); digitalWrite(RF_TX_SWC, LOW);
  pinMode(HPA_PWR, OUTPUT);   digitalWrite(HPA_PWR, LOW);

  Serial.begin(115200);

  while(!rf4463.init())
    Serial.println("Init Failed");
  Serial.println("Init Success");
  rf4463.setModeRx();
}

void receiveEvent(int count) {
  Serial.print("Data Received from OBC...[");
  while (0 < Wire.available()) {
    for(int i=0; i<sizeof(I2C_RX_BUF); i++){
      byte x = Wire.read();
      Serial.print(x);
      if(i != 190){
        Serial.print(", ");
      } else {
        Serial.println("]");
      }
    }
  }
}

void requestEvent(){
  Serial.println("Data Transmited to OBC...[");

  for(int i = 0; i<sizeof(I2C_TX_BUF); i++){
    Wire.write(I2C_TX_BUF[i]);
  }

  for(int i = 0; i<sizeof(I2C_TX_BUF); i++){
    I2C_TX_BUF[i] = {0xFF};
  }
}

// Main Loop function
void loop() {
  if (rf4463.recv(RF_RX_BUF, &len_RF_RX_BUF)) {
    Serial.print("Tries : ");
    Serial.print(counter);
    Serial.print(" --> Received Message: ");

    for(int i=0; i<len_RF_RX_BUF; i++){
    Serial.print(RF_RX_BUF[i]);
    Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.print("Tries : ");
    Serial.print(counter);
    Serial.println(" --> recv failed");
    invalids++;
  }
  counter++;
  delay(100);
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