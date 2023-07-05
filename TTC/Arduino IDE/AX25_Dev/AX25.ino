byte DATA_FRAME[275] = {0xFF};
byte DATA_FRAME_CRC[272];
byte iField[256] = {0xFF};

char DST_ADDRESS[6];
char SRC_ADDRESS[6];

byte DST_SSID = 0B01101010;
byte SRC_SSID = 0B11101011;

byte FLAG = 0x7E;
byte CONTROL = 0x03;
byte PID = 0xF0;

void AX25_Encapsulate(byte* message, int messageSize, String dst, String src){
  SetAddress(dst, src);
  SetIField(message, messageSize);
  int index = 0;

  DATA_FRAME[index++] = FLAG;

  for(int i=0; i<7; i++){
    if(i != 6){
      DATA_FRAME[index++] = reverseBits(DST_ADDRESS[i]<<1);
    }else{
      DATA_FRAME[index++] = reverseBits(DST_SSID);
    }
  }

  for(int i=0; i<7; i++){
    if(i != 6){
      DATA_FRAME[index++] = reverseBits(SRC_ADDRESS[i]<<1);
    }else{
      DATA_FRAME[index++] = reverseBits(SRC_SSID);
    }
  }

  DATA_FRAME[index++] = reverseBits(CONTROL);
  DATA_FRAME[index++] = reverseBits(PID);

  for(int i=0; i<sizeof(iField); i++){
    DATA_FRAME[index++] = reverseBits(iField[i]);
  }

  bitStuffing(DATA_FRAME);

  // for(int i=0; i<272; i++){
  //   DATA_FRAME_CRC[i] = DATA_FRAME[i+1];
  // }
  
  // uint16_t crcValue = calculateCRC(DATA_FRAME_CRC, sizeof(DATA_FRAME_CRC));
  // DATA_FRAME[index++] = crcValue>>8;
  // DATA_FRAME[index++] = crcValue & 0x00FF;
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

void bitStuffing(byte* dataArr) {
  int arrSize = 275;  // Current size of the array
  
  for (int i = 0; i < arrSize; i++) {
    byte data = dataArr[i];
    byte mask = 0b00011111;  // Bit mask to check for bit stuffing
    byte prevBit = 0;  // Previous bit value
    byte consecutiveOnes = 0;  // Counter for consecutive ones
    byte stuffedBits = 0;  // Counter for stuffed bits
    
    for (int j = 0; j < 8; j++) {
      byte currentBit = (data >> j) & 0b00000001;
      
      if (currentBit == 1) {
        consecutiveOnes++;
        
        if (consecutiveOnes >= 5 && prevBit == 1) {
          // Bit stuffing detected
          if (stuffedBits < 2) {
            // Insert a stuffed bit (0)
            data = (data << 1) | 0b00000001;
            stuffedBits++;
          } else {
            // Shift the remaining bits to the left to make room for a new stuffed bit
            for (int k = 7; k > j; k--) {
              byte bit = (data >> (k - 1)) & 0b00000001;
              data = (data & ~(0b00000001 << k)) | (bit << k);
            }
            
            // Insert a new stuffed bit (0) at the current bit position
            data = (data & ~(0b00000001 << j)) | (0b00000001 << j);
            
            // Increase the array size
            arrSize++;
            
            // Shift the elements after the current index to the right
            for (int k = arrSize - 1; k > i; k--) {
              dataArr[k] = dataArr[k - 1];
            }
          }
        }
      } else {
        consecutiveOnes = 0;  // Reset the counter if the bit is 0
      }
      
      prevBit = currentBit;
    }
    
    // Update the array with the modified data
    dataArr[i] = data;
  }
}

void AX25_ClearBuffer(){
  for(int i=0; i<sizeof(DATA_FRAME); i++){
    DATA_FRAME[i] = 0xFF;
  }
  for(int i=0; i<sizeof(iField); i++){
    iField[i] = 0xFF;
  }
}

void AX25_Test_Print(){
  // Serial.println("AX25 Library");
  for(int i=0; i<sizeof(DATA_FRAME); i++){
    Serial.print(DATA_FRAME[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}