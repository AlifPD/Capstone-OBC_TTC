extern byte DATA_FRAME[273];

byte message[5] = "HELLO";
byte message2[5] = "HOLLA";

void setup() {
  Serial.begin(115200);

  AX25_Encapsulate("HELLO", 5, "GROUND", "SPACE");
  AX25_Test_Print();

  AX25_Encapsulate("HOLLA", 5, "GROUND", "SPACE");
  AX25_Test_Print();
}

void loop() {
  
}