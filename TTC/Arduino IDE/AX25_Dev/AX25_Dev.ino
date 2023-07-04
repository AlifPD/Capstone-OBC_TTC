extern byte DATA_FRAME[274];

byte message[5] = "HELLO";

void setup() {
  Serial.begin(115200);
  
  SetAddress("GROUND", "SPACE");
  SetIField(message, 5);
  Add_AX25_Header();

  for(int i=0; i<sizeof(DATA_FRAME); i++){
    Serial.print(DATA_FRAME[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  
}