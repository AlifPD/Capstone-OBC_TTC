#include <RH_RF24.h>
#include <RHSoftwareSPI.h>

#define RF_RX_SWC PA12
#define RF_TX_SWC PB3
#define HPA_PWR PA1

RHSoftwareSPI spi2;
RH_RF24 rf4463(PB12, PA11, PA8, spi2);

static int counter;
static int invalids; 

uint8_t RF_RX_BUF[59];
uint8_t len_RF_RX_BUF = sizeof(RF_RX_BUF);

byte FLAG;

char DST_ADDRESS[6];
byte DST_SSID;

char SRC_ADDRESS[6];
byte SRC_SSID;

byte CONTROL;
byte PID;

byte iField[40];

uint16_t crcValue;
uint16_t crcCalc;
uint8_t RF_RX_BUF_CRC[56];

void setup() {
  SystemClock_Config();

  spi2.setPins(PB14, PB15, PB13);

  pinMode(RF_RX_SWC, OUTPUT);
  pinMode(RF_TX_SWC, OUTPUT);
  pinMode(HPA_PWR, OUTPUT);

  digitalWrite(RF_RX_SWC, HIGH);
  digitalWrite(RF_TX_SWC, LOW);
  digitalWrite(HPA_PWR, LOW);

  Serial.begin(115200);

  if (!rf4463.init())
    Serial.println("Init Failed");
  Serial.println("Init Success");
  rf4463.setModeRx();

  Clr_Buf();
}

void loop() {
  if (rf4463.recv(RF_RX_BUF, &len_RF_RX_BUF)) {
    Serial.println("Message Received");

    AX25_Decapsulate();
  }
  counter++;

  Clr_Buf();
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

byte reverseBits(byte b) {
  byte reversed = 0;
  
  for (int i = 0; i < 8; i++) {
    reversed <<= 1;
    reversed |= (b & 1);
    b >>= 1;
  }
  
  return reversed;
}

uint16_t calculateCRC(const byte* data, size_t length){
  uint16_t crc = 0xFFFF;

  for (size_t i = 0; i < length; ++i) {
    crc ^= data[i] << 8;

    for (byte bit = 0; bit < 8; ++bit) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021; 
      } else {
        crc <<= 1;
      }
    }
  }

  return crc;
}

void Clr_Buf(){
  for(int i=0; i<sizeof(RF_RX_BUF); i++){
    RF_RX_BUF[i] = 0xFF;
  }

  for(int i=0; i<sizeof(iField); i++){
    iField[i] = 0xFF;
  }

  for(int i=0; i<sizeof(RF_RX_BUF_CRC); i++){
    RF_RX_BUF_CRC[i] = 0xFF;
  }

}

void AX25_Decapsulate(){
  int index = 0;

  FLAG = RF_RX_BUF[index++];

  for(int i=0; i<7; i++){
    if(i != 6){
      DST_ADDRESS[i] = (reverseBits(RF_RX_BUF[index++]))>>1;
    }else{
      DST_SSID = reverseBits(RF_RX_BUF[index++]);
    }
  }

  for(int i=0; i<7; i++){
    if(i != 6){
      SRC_ADDRESS[i] = (reverseBits(RF_RX_BUF[index++]))>>1;
    }else{
      SRC_SSID = reverseBits(RF_RX_BUF[index++]);
    }
  }

  CONTROL = reverseBits(RF_RX_BUF[index++]);
  PID = reverseBits(RF_RX_BUF[index++]);

  for(int i=0; i<sizeof(iField); i++){
    iField[i] = reverseBits(RF_RX_BUF[index++]);
  }

  for(int i=0; i<sizeof(RF_RX_BUF_CRC); i++){
    RF_RX_BUF_CRC[i] = RF_RX_BUF[i+1];
  }

  crcValue = RF_RX_BUF[58];
  crcValue |= ((uint16_t)RF_RX_BUF[57] << 8);

  crcCalc = calculateCRC(RF_RX_BUF_CRC, sizeof(RF_RX_BUF_CRC));

  Serial.print("DATA RECEIVED : [");
  for(int i=0; i<sizeof(RF_RX_BUF); i++){
    Serial.print(" ");
    Serial.print(RF_RX_BUF[i], HEX);
  }
  Serial.println("]");

  Serial.print("FLAG : [");
  Serial.print(FLAG, HEX);
  Serial.println("]");
  
  Serial.print("DST ADDRESS : [");
  for(int i=0; i<sizeof(DST_ADDRESS); i++){
    Serial.print(" ");
    Serial.print(DST_ADDRESS[i]);
  }
  Serial.println("]");

  Serial.print("DST SSID : [");
  Serial.print(DST_SSID, HEX);
  Serial.println("]");

  Serial.print("SRC ADDRESS : [");
  for(int i=0; i<sizeof(SRC_ADDRESS); i++){
    Serial.print(" ");
    Serial.print(SRC_ADDRESS[i]);
  }
  Serial.println("]");

  Serial.print("SRC SSID : [");
  Serial.print(SRC_SSID, HEX);
  Serial.println("]");

  Serial.print("CONTROL : [");
  Serial.print(CONTROL, HEX);
  Serial.println("]");

  Serial.print("PID : [");
  Serial.print(PID, HEX);
  Serial.println("]");

  Serial.print("Info Field : [");
  for(int i=0; i<sizeof(iField); i++){
    Serial.print(" ");
    Serial.print(iField[i], HEX);
  }
  Serial.println("]");

  Serial.print("CRC Received: [");
  Serial.print(crcValue, HEX);
  Serial.println("]");

  Serial.print("CRC Calculated: [");
  Serial.print(crcCalc, HEX);
  Serial.print("] --> ");
  if(crcCalc == crcValue){
    Serial.println("DATA INTEGRITY OK\n");
  }else{
    Serial.println("DATA INTEGRITY ERROR\n");
  }
}