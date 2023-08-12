byte AX25_FRAME_PRE_CRC[272] = {0xFF};
byte AX25_FRAME_PST_CRC[275] = {0xFF};

byte FLAG = 0x7E;

char DST_ADDRESS[6];
byte DST_SSID = 0B01101010;

char SRC_ADDRESS[6];
byte SRC_SSID = 0B11101011;

byte CONTROL = 0x03;

byte PID = 0xF0;

byte iField[256] = {0xFF};

void AX25_Encapsulate(byte* message, int messageSize, String dst, String src){
  SetAddress(dst, src);
  SetIField(message, messageSize);
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
  AX25_FRAME_PRE_CRC[index++] = reverseBits(PID);
  AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(CONTROL);
  AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(PID);

  for(int i=0; i<sizeof(iField); i++){
    AX25_FRAME_PRE_CRC[index++] = reverseBits(iField[i]);
    AX25_FRAME_PST_CRC[indexFinal++] = reverseBits(iField[i]);
  }

  uint16_t crcValue = calculateCRC(AX25_FRAME_PRE_CRC, sizeof(AX25_FRAME_PRE_CRC));
  Serial.println(crcValue, HEX);

  AX25_FRAME_PST_CRC[indexFinal++] = (crcValue>>8) & 0xFF;
  AX25_FRAME_PST_CRC[indexFinal++] = (crcValue>>0) & 0xFF;
}

void SetIField(byte* message, int messageSize){
  for(int i=0; i<sizeof(iField); i++){
    if(i >= messageSize){
      iField[i] = 0xFF;
    }else{
      iField[i] = message[i];
    }
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

void bitStuffing(){
} // TODO !!

void AX25_ClearBuffer(){
  for(int i=0; i<sizeof(AX25_FRAME_PRE_CRC); i++){
    AX25_FRAME_PRE_CRC[i] = 0xFF;
  }
  for(int i=0; i<sizeof(iField); i++){
    iField[i] = 0xFF;
  }
}

void AX25_Test_Print(){
  int counter = 0;
  for(int i=0; i<sizeof(AX25_FRAME_PRE_CRC); i++){
    Serial.print(AX25_FRAME_PRE_CRC[i], HEX);
    Serial.print(" ");
    counter++;
  }
  Serial.println();
  Serial.print("DATA LEN: ");
  Serial.println(counter);
}