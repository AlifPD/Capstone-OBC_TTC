#include <RH_RF24.h>
#include <RHSoftwareSPI.h>
#include <ax25.h>

#define Max_Packet_Size 128
#define rf_receive PA12
#define rf_transmit PB3
#define power_hpa PA1

byte rcvSequence[128];
uint8_t len = sizeof(rcvSequence);

RHSoftwareSPI spi2;

char message[Max_Packet_Size]; // Test Message
char *message_ptr = &message[Max_Packet_Size]; // Pointer for variable "message"
uint8_t lastRssi;

uint8_t buf[Max_Packet_Size];
uint8_t bufLen = sizeof(buf);

// uint8_t userInput;
// uint8_t userInput = 1; // TX
uint8_t userInput = 2; // RX

static int counter; // Variable to count number of transmitted data
static int invalids;  // Variable to count number of failed received data

AX25 rf4463_TX(PA4, PB1, PB0);
AX25 rf4463_RX(PB12, PA11, PA8, spi2);

void setup() {
  SystemClock_Config();
  spi2.setPins(PB14, PB15, PB13);
  Serial.begin(115200);

  setupPinMode();

  while(!rf4463_TX.powerAndInit())
    Serial.println("Init TX Module Failed");
  Serial.println("Init TX Module Success"); 

  while(!rf4463_RX.powerAndInit())
    Serial.println("Init RX Module Failed");
  Serial.println("Init RX Module Success");
  Serial.println("Hello");
}

void loop() {
  Serial.println("Waiting Data from User Input ...");

  // while (Serial.available() > 0) {}
  // userInput = Serial.parseInt();

  switch(userInput){
    case 1 :  setupTransmit();
              while(1){
                sprintf(message, "TTC Board Test, Message number: %d", counter+1);
                Serial.print(F("Sending Data..."));
                Serial.println(message);
                
                rf4463_TX.transmit(message, sizeof(message));
                counter++;

                Serial.print(F("Totals : "));
                Serial.print(counter);
                Serial.println(" Data");
                Serial.println("+++++++");
                delay(50);
              }
              break;
    case 2 :  setupReceive();
              while(1){
                if (rf4463_RX.receive(rcvSequence, &len)) {
                  Serial.print("Tries : ");
                  Serial.print(counter);
                  Serial.print(" --> Received Message: ");
                  for(int i = 0; i<256; i++){
                    Serial.print(rcvSequence[i]);
                    Serial.print(", ");
                  }
                  Serial.println();
                  rf4463_RX.demod(rcvSequence, 450);
                  // Serial.println();
                  // Serial.print((char*)buf);
                } else {
                  Serial.print("Tries : ");
                  Serial.print(counter);
                  Serial.println(" --> recv failed");
                  invalids++;
                }
                counter++;
                delay(100);
              }
              break;
    default : Serial.print("Input Invalid");
              Serial.println(userInput);
  }  
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

void setupTransmit(){
  digitalWrite(rf_receive, LOW);
  digitalWrite(rf_transmit, HIGH);
  digitalWrite(power_hpa, HIGH);
  // rf4463_TX.setTxMode();
}

void setupReceive(){
  digitalWrite(rf_receive, HIGH);
  digitalWrite(rf_transmit, LOW);
  digitalWrite(power_hpa, LOW);
  rf4463_RX.setRxMode();
}

void setupPinMode(){
  pinMode(rf_receive, OUTPUT);
  pinMode(rf_transmit, OUTPUT);
  pinMode(power_hpa, OUTPUT);
}