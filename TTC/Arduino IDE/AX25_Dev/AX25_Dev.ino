void setup() {
  Serial.begin(115200);

  AX25_Encapsulate((byte*)"HELLO", 5, "SPACE", "GROUND");
  AX25_Test_Print();
}

void loop() {
   
}
