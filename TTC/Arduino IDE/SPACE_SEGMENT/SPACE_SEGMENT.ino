#include <RH_RF24.h>
#include <RHSoftwareSPI.h>
#include <Wire.h>

#define RF_RX_SWC PA12
#define RF_TX_SWC PB3
#define HPA_PWR PA1

#define ADDR_TTC 94

RHSoftwareSPI spi2;
RH_RF24 rf4463_RX(PB12, PA11, PA8, spi2);

static int counter;
static int invalids;

uint8_t I2C_TX_BUF[55] = {0xFF};
uint8_t I2C_RX_BUF[191] = {0xFF};

uint8_t RF_RX_BUF[5] = {0xFF};
uint8_t len_RF_RX_BUF = sizeof(RF_RX_BUF);

void setup() {
  SystemClock_Config();

  Serial.begin(115200);

  spi2.setPins(PB14, PB15, PB13);

  pinMode(RF_RX_SWC, OUTPUT); digitalWrite(RF_RX_SWC, HIGH);
  pinMode(RF_TX_SWC, OUTPUT); digitalWrite(RF_TX_SWC, LOW);
  pinMode(HPA_PWR, OUTPUT);   digitalWrite(HPA_PWR, LOW);

  while(!rf4463_RX.init())
    Serial.println("Init RX Failed");
  Serial.println("Init RX Success");
  rf4463_RX.setModeRx();

  Wire.begin(ADDR_TTC);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Wire.setTimeout(1000);

  Clr_RF_RX_BUF();
  Clr_I2C_RX_BUF();
  Clr_I2C_TX_BUF();
}

void receiveEvent(int count) {
  Serial.print("Data Received from OBC : [");
  while (0 < Wire.available()) {
    for(int i=0; i<sizeof(I2C_RX_BUF); i++){
      byte x = Wire.read();

      Serial.print(" ");
      Serial.print(x, HEX);
    }
    Serial.println(" ]");
  }
  Clr_I2C_RX_BUF();
}

void requestEvent(){
  Serial.print("Data Transmited to OBC : [");
  for(int i=0; i<sizeof(I2C_TX_BUF); i++){
    Serial.print(" ");
    Serial.print(I2C_TX_BUF[i], HEX);
  }
  Serial.println("]");

  for(int i = 0; i<sizeof(I2C_TX_BUF); i++){
    Wire.write(I2C_TX_BUF[i]);
  }

  Clr_I2C_TX_BUF();
}

void loop() {
      if (rf4463_RX.recv(RF_RX_BUF, &len_RF_RX_BUF)) {
        Serial.print("Tries : ");
        Serial.print(counter);
        Serial.print(" --> Received Message : [");

        I2C_TX_BUF[0] = 0x7F;
        I2C_TX_BUF[1] = 1;
        I2C_TX_BUF[2] = 2;
        I2C_TX_BUF[3] = 0;
        I2C_TX_BUF[4] = 1;

        for(int i=0; i<len_RF_RX_BUF; i++){
          Serial.print(" ");
          Serial.print(RF_RX_BUF[i], HEX);

          I2C_TX_BUF[i+5] = RF_RX_BUF[i];
        }
        Serial.println("]");
      } else {
        Serial.print("Tries : ");
        Serial.print(counter);
        Serial.println(" --> recv failed");
        invalids++;
      }
      counter++;
      delay(250);
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

void Clr_RF_RX_BUF(){
  for(int i = 0; i<sizeof(RF_RX_BUF); i++){
    RF_RX_BUF[i] = {0xFF};
  }
}

void Clr_I2C_TX_BUF(){
  for(int i = 0; i<sizeof(I2C_TX_BUF); i++){
    I2C_TX_BUF[i] = {0xFF};
  }
}

void Clr_I2C_RX_BUF(){
  for(int i = 0; i<sizeof(I2C_RX_BUF); i++){
    I2C_RX_BUF[i] = {0xFF};
  }
}