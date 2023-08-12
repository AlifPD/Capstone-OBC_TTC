void setup() {
  Serial.begin(115200);

  AX25_Encapsulate((byte*)"HELLO", 5, "GROUND", "SPACE");
  AX25_Test_Print();
}

void loop() {
   
}
