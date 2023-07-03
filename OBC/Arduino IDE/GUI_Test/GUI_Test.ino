char userInput;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

int getDelayTime(){
  String blinkTimeString;
  int blinkTimeInt;
  delay(2000);
  blinkTimeString = Serial.readString();
  return blinkTimeString.toInt();
}

void loop() {
  if(Serial.available() > 0){
    userInput = Serial.read();

    if(userInput == 'o'){
      digitalWrite(LED_BUILTIN, HIGH);
    }

    if(userInput == 'x'){
      digitalWrite(LED_BUILTIN, LOW);
    }

    if(userInput == 'b'){
      int delayTime = getDelayTime();
      for(int i=0; i<10; i++){
        delay(delayTime);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(delayTime);
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
}
