#include <RH_RF24.h>

#define RF_RX_SWC PA12
#define RF_TX_SWC PB3
#define HPA_PWR PA1

RH_RF24 rf4463(PA4, PB1, PB0);

static uint8_t counter = 1;

byte AX25_FRAME_PRE_CRC[56] = {0xFF};
byte AX25_FRAME_PST_CRC[59] = {0xFF};

byte FLAG = 0x7E;

char DST_ADDRESS[6];
byte DST_SSID = 0B01101010;

char SRC_ADDRESS[6];
byte SRC_SSID = 0B11101011;

byte CONTROL = 0x03;

byte PID = 0xF0;

byte iField[40] = {
  0x01, 0x2C, 0x74, 0xE6, 0x70, 0xC9, 0x33, 0x15,
  0xD1, 0x65, 0xE6, 0xAA, 0x17, 0xD4, 0xED, 0xC5,
  0x22, 0x52, 0x85, 0xF2, 0xEB, 0x47, 0x2A, 0xB8,
  0x72, 0x78, 0x39, 0xD4, 0x2A, 0x7F, 0x4B, 0xAE,
  0xC4, 0x14, 0x20, 0x5D, 0x4D, 0x0E, 0x86, 0xD4
};

void setup() {
  SystemClock_Config();

  pinMode(RF_RX_SWC, OUTPUT);
  pinMode(RF_TX_SWC, OUTPUT);
  pinMode(HPA_PWR, OUTPUT);

  digitalWrite(RF_RX_SWC, LOW);
  digitalWrite(RF_TX_SWC, HIGH);
  digitalWrite(HPA_PWR, HIGH);

  Serial.begin(115200);

  while (!rf4463.init())
    Serial.println("Init Failed");
  Serial.println("Init Success");  
  rf4463.setTxPower(0x7F);                                     
}

void loop() {
  SetAddress("GROUND", "SPACE");
  int index = 0;
  int indexFinal = 0;

  AX25_FRAME_PST_CRC[indexFinal++] = FLAG;

  for(int i=0; i<7; i++){
    if(i != 6){
      AX25_FRAME_PRE_CRC[index++] = reverseBits(DST_ADDRESS[i]<<1);
      AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(DST_ADDRESS[i]<<1);
    }else{
      AX25_FRAME_PRE_CRC[index++] = reverseBits(DST_SSID);
      AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(DST_SSID);
    }
  }

  for(int i=0; i<7; i++){
    if(i != 6){
      AX25_FRAME_PRE_CRC[index++] = reverseBits(SRC_ADDRESS[i]<<1);
      AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(SRC_ADDRESS[i]<<1);
    }else{
      AX25_FRAME_PRE_CRC[index++] = reverseBits(SRC_SSID);
      AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(SRC_SSID);

    }
  }

  AX25_FRAME_PRE_CRC[index++] = reverseBits(CONTROL);
  AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(CONTROL);

  AX25_FRAME_PRE_CRC[index++] = reverseBits(PID);
  AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(PID);

  for(int i=0; i<sizeof(iField); i++){
    AX25_FRAME_PRE_CRC[index++] = reverseBits(iField[i]);
    AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(iField[i]);
  }

  uint16_t crcValue = calculateCRC(AX25_FRAME_PRE_CRC, sizeof(AX25_FRAME_PRE_CRC));

  AX25_FRAME_PST_CRC[indexFinal++] = (crcValue>>8) & 0xFF;
  AX25_FRAME_PST_CRC[indexFinal++] = (crcValue>>0) & 0xFF;

  Serial.print("Transmitting Data: [");
  for(int i=0; i<sizeof(AX25_FRAME_PST_CRC); i++){
    Serial.print(" ");
    Serial.print(AX25_FRAME_PST_CRC[i], HEX);
  }
  Serial.println("]");
  Serial.print("CRC-16 Value : ");
  Serial.println(crcValue, HEX);
  
  if(rf4463.send(AX25_FRAME_PST_CRC, 210)){
    iField[0] = ++counter;
  }else{
    Serial.println("Transmit Failed");
  }

  Serial.print("Totals : ");
  Serial.print(counter);
  Serial.println(" Data");
  Serial.println("+++++++\n");

  AX25_ClearBuffer();
  delay(5000);
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

void SetAddress(String dst, String src){
  if(dst.length()<6){
    for(int i=dst.length(); i<6; i++){
      dst += " ";
    }
  }
  if(src.length()<6){
    for(int i=src.length(); i<6; i++){
      src += " ";
    }
  }

  for(int i=0; i<6; i++){
    DST_ADDRESS[i] = dst[i];
    SRC_ADDRESS[i] = src[i];
  }
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

byte reverseBits(byte b) {
  byte reversed = 0;
  
  for (int i = 0; i < 8; i++) {
    reversed <<= 1;
    reversed |= (b & 1);
    b >>= 1;
  }
  
  return reversed;
}

void AX25_ClearBuffer(){
  for(int i=0; i<sizeof(AX25_FRAME_PRE_CRC); i++){
    AX25_FRAME_PRE_CRC[i] = 0xFF;
  }
  for(int i=0; i<sizeof(AX25_FRAME_PST_CRC); i++){
    AX25_FRAME_PST_CRC[i] = 0xFF;
  }
}