byte iField[256] = {0xFF};
byte AX25_FRAME[272] = {0x00};
byte AX25_FRAME_FINAL[275] = {0x00};
byte AX25_FRAME_STUFFED[450] = {0x00};

char DST_ADDRESS[6];
char SRC_ADDRESS[6];

byte DST_SSID = 0B01101010;
byte SRC_SSID = 0B11101011;

byte FLAG = 0x7E;
byte CONTROL = 0x03;
byte PID = 0xF0;

void AX25_Send(byte* message, int messageSize, String dst, String src){
  SetAddress(dst, src);
  SetIField(message, messageSize);
  int index = 0;
  int indexFinal = 0;

  AX25_FRAME_FINAL[indexFinal++] = FLAG;

  for(int i=0; i<7; i++){
    if(i != 6){
      AX25_FRAME[index++] = reverseBits(DST_ADDRESS[i]<<1);
      AX25_FRAME_FINAL[indexFinal++] = reverseBits(DST_ADDRESS[i]<<1);
    }else{
      AX25_FRAME[index++] = reverseBits(DST_SSID);
      AX25_FRAME_FINAL[indexFinal++] = reverseBits(DST_SSID);
    }
  }

  for(int i=0; i<7; i++){
    if(i != 6){
      AX25_FRAME[index++] = reverseBits(SRC_ADDRESS[i]<<1);
      AX25_FRAME_FINAL[indexFinal++] = reverseBits(SRC_ADDRESS[i]<<1);
    }else{
      AX25_FRAME[index++] = reverseBits(SRC_SSID);
      AX25_FRAME_FINAL[indexFinal++] = reverseBits(SRC_SSID);

    }
  }

  AX25_FRAME[index++] = reverseBits(CONTROL);
  AX25_FRAME[index++] = reverseBits(PID);
  AX25_FRAME_FINAL[indexFinal++] = reverseBits(CONTROL);
  AX25_FRAME_FINAL[indexFinal++] = reverseBits(PID);

  for(int i=0; i<sizeof(iField); i++){
    AX25_FRAME[index++] = reverseBits(iField[i]);
    AX25_FRAME_FINAL[indexFinal++] = reverseBits(iField[i]);
  }
  
  // bitStuffing();

  uint16_t crcValue = calculateCRC(AX25_FRAME, sizeof(AX25_FRAME));
  Serial.println(crcValue, HEX);

  AX25_FRAME_FINAL[indexFinal++] = (crcValue>>8) & 0xFF;
  AX25_FRAME_FINAL[indexFinal++] = (crcValue>>0) & 0xFF;
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
  // int stuffedIndex = 0;
  // int count = 0;

  // for(int i=0; i<sizeof(AX25_FRAME); i++){
  //   byte currentByte = AX25_FRAME[i];

  //   for(int bit=7; bit>=0; bit--){
  //     bool currentBit = (currentByte>>bit) & 1;

  //     if(currentBit){
  //       count++;

  //       if(count == 6){
          
  //       }else{

  //       }

  //     }else{
  //       count = 0;
  //     }
  //   }
  // }  
} // TODO !!

void AX25_ClearBuffer(){
  for(int i=0; i<sizeof(AX25_FRAME); i++){
    AX25_FRAME[i] = 0xFF;
  }
  for(int i=0; i<sizeof(iField); i++){
    iField[i] = 0xFF;
  }
}

void AX25_Test_Print(){
  for(int i=0; i<sizeof(AX25_FRAME); i++){
    Serial.print(AX25_FRAME[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  for(int i=0; i<sizeof(AX25_FRAME_FINAL); i++){
    Serial.print(AX25_FRAME_FINAL[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // for(int i=0; i<sizeof(AX25_FRAME_STUFFED); i++){
  //   Serial.print(AX25_FRAME_STUFFED[i], HEX);
  //   Serial.print(" ");
  // }
  // Serial.println();
}