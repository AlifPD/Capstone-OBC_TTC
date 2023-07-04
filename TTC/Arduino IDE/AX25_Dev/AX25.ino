byte iField[256];
byte DATA_FRAME[274];

char DST_ADDRESS[6];
char SRC_ADDRESS[6];

byte DST_SSID = 0B01101010;
byte SRC_SSID = 0B11101011;

byte FLAG = 0x7E;
byte CONTROL = 0x03;
byte PID = 0xF0;

int index = 0;

void Add_AX25_Header(){
  DATA_FRAME[index++] = FLAG;

  for(int i=0; i<7; i++){
    if(i != 6){
      DATA_FRAME[index++] = DST_ADDRESS[i]<<1;
    }else{
      DATA_FRAME[index++] = DST_SSID;
    }
  }

  for(int i=0; i<7; i++){
    if(i != 6){
      DATA_FRAME[index++] = SRC_ADDRESS[i]<<1;
    }else{
      DATA_FRAME[index++] = SRC_SSID;
    }
  }

  DATA_FRAME[index++] = CONTROL;
  DATA_FRAME[index++] = PID;

  for(int i=0; i<sizeof(iField); i++){
    DATA_FRAME[index++] = iField[i];
  }

  DATA_FRAME[index++] = FLAG;
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

void SetIField(byte* message, int messageSize){
  for(int i=0; i<sizeof(iField); i++){
    if(i >= messageSize){
      iField[i] = 0xFF;
    }else{
      iField[i] = message[i];
    }
  }
}

int GetDataFrameSize(){
  return sizeof(DATA_FRAME);
}

void ClearBuffer(){
  for(int i=0; i<sizeof(DATA_FRAME); i++){
    DATA_FRAME[i] = 0xFF;
  }
  for(int i=0; i<sizeof(iField); i++){
    iField[i] = 0xFF;
  }
}

void Test_Print(){
  Serial.println("AX25 Library");
}