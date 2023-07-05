extern byte DATA_FRAME[275];

void setup() {
  Serial.begin(115200);

  AX25_Encapsulate("HELLO", 5, "GROUND", "SPACE");
  AX25_Test_Print();
}

void loop() {
  
}