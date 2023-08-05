#include <RH_RF24.h>

#define RF_RX_SWC PA12
#define RF_TX_SWC PB3
#define HPA_PWR PA1

RH_RF24 rf4463_TX(PA4, PB1, PB0); 

uint8_t UserInput;

static int counter;


uint8_t RF_RX_BUF[191];                             // RF Receive Buffer
uint8_t len_RF_RX_BUF = sizeof(RF_RX_BUF);

uint8_t RF_TX_BUF_1_1[5]      = {0x80, 1, 1, 0, 1}; // Activate 5V SWC-1
uint8_t RF_TX_BUF_1_2[5]      = {0x80, 1, 2, 0, 1}; // Activate 5V SWC-2
uint8_t RF_TX_BUF_1_3[5]      = {0x80, 1, 3, 0, 1}; // Activate 3V SWC-1
uint8_t RF_TX_BUF_1_4[5]      = {0x80, 1, 4, 0, 1}; // Activate 3v SWC-2
uint8_t RF_TX_BUF_1_1_ALT[5]  = {0x80, 1, 1, 0, 0}; // Deactivate 5V SWC-1
uint8_t RF_TX_BUF_1_2_ALT[5]  = {0x80, 1, 2, 0, 0}; // Deactivate 5V SWC-2
uint8_t RF_TX_BUF_1_3_ALT[5]  = {0x80, 1, 3, 0, 0}; // Deactivate 3V SWC-1
uint8_t RF_TX_BUF_1_4_ALT[5]  = {0x80, 1, 4, 0, 0}; // Deactivate 3v SWC-2
uint8_t RF_TX_BUF_2_1[5]      = {0x80, 2, 1, 0, 0}; // Read Read Solar Panel 1 Voltage
uint8_t RF_TX_BUF_2_2[5]      = {0x80, 2, 2, 0, 0}; // Read Read Solar Panel 2 Voltage
uint8_t RF_TX_BUF_2_3[5]      = {0x80, 2, 3, 0, 0}; // Read Read Solar Panel 3 Voltage
uint8_t RF_TX_BUF_2_4[5]      = {0x80, 2, 4, 0, 0}; // Read Solar Panel Total Voltage
uint8_t RF_TX_BUF_2_5[5]      = {0x80, 2, 5, 0, 0}; // Read Battery Capacity
uint8_t RF_TX_BUF_2_6[5]      = {0x80, 2, 6, 0, 0}; // Read EPS Board Temperature
uint8_t RF_TX_BUF_2_7[5]      = {0x80, 2, 7, 0, 0}; // Read BMS Board Temperature

uint8_t RF_TX_BUF_3_1[5] = {0x7F, 3, 1, 0, 0};  // Read Telemetry and Data from Satellite through TTC
uint8_t RF_TX_BUF_4_1[5] = {0x7F, 4, 1, 0, 0};  // Read TTC Board Temperature

uint8_t RF_TX_BUF_5_1[5] = {0x7E, 5, 1, 0, 0};  // Redundant Takeover

uint8_t RF_TX_BUF_6_1[5] = {0x81, 6, 1, 0, 0};  // Read Telemetry and Data from Satellite through S-BAND
uint8_t RF_TX_BUF_6_2[5] = {0x81, 6, 2, 0, 0};  // Read Data Received by ADS-B
uint8_t RF_TX_BUF_7_1[5] = {0x81, 7, 1, 0, 0};  // Read ADS-B Board Temperature

void setup() {
  SystemClock_Config();

  pinMode(RF_RX_SWC, OUTPUT); digitalWrite(RF_RX_SWC, LOW);
  pinMode(RF_TX_SWC, OUTPUT); digitalWrite(RF_TX_SWC, HIGH);
  pinMode(HPA_PWR, OUTPUT);   digitalWrite(HPA_PWR, HIGH);

  Serial.begin(115200);

  while(!rf4463_TX.init())
    Serial.println("Init TX Failed");
  Serial.println("Init TX Success");  
  rf4463_TX.setTxPower(0x7F);  

  Clr_RF_RX_BUF();                                 
}

void loop() {
      Serial.println("Choose Command to Send to Satellite ...");
      Serial.println("1. Activate 5V SWC-1");
      Serial.println("2. Activate 5V SWC-2");
      Serial.println("3. Activate 3V SWC-1");
      Serial.println("4. Activate 3V SWC-2");
      Serial.println("5. Deactivate 5V SWC-1");
      Serial.println("6. Deactivate 5V SWC-2");
      Serial.println("7. Deactivate 3V SWC-1");
      Serial.println("8. Deactivate 3V SWC-2");
      Serial.println("9. Read Solar Panel 1 Voltage");
      Serial.println("10. Read Solar Panel 2 Voltage");
      Serial.println("11. Read Solar Panel 3 Voltage");
      Serial.println("12. Read Solar Panel Total Voltage");
      Serial.println("13. Read Battery Capacity");
      Serial.println("14. Read EPS Board Temperature");
      Serial.println("15. Read BMS Board Temperature");
      Serial.println("16. Read Telemetry and Data from Satellite through TTC");
      Serial.println("17. Read TTC Board Temperature");
      Serial.println("18. Redundant Takeover");
      Serial.println("19. Read Telemetry and Data from Satellite through S-BAND");
      Serial.println("20. Read Data Received by ADS-B");
      Serial.println("21. Read ADS-B Board Temperature");

      while(Serial.available() == 0){}
      UserInput = Serial.parseInt();
      while(Serial.available() > 0) Serial.read();


      switch(UserInput){
        case 1 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_1[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_1, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 2 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_2[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_2, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 3 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_3[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_3, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 4 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_4[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_4, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 5 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_1_ALT[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_1_ALT, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 6 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_2_ALT[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_2_ALT, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 7 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_3_ALT[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_3_ALT, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 8 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_1_4_ALT[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_1_4_ALT, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 9 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_2_1[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_2_1, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;
        
        case 10 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_2_2[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_2_2, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 11 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_2_3[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_2_3, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;  

        case 12 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_2_4[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_2_4, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;  

        case 13 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_2_5[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_2_5, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;
                  
        case 14 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_2_6[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_2_6, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;    

        case 15 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_2_7[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_2_7, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;

        case 16 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_3_1[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_3_1, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;      

        case 17 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_4_1[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_4_1, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;  

        case 18 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_5_1[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_5_1, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;  

        case 19 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_6_1[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_6_1, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;  

        case 20 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_6_2[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_6_2, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;  

        case 21 :  Serial.print("Ground Segment, Data Transmit : [");
                  for(int i=0; i<5; i++){
                    Serial.print(" ");
                    Serial.print(RF_TX_BUF_7_1[i], HEX);
                  }
                  Serial.println(" ]");
      
                  if(rf4463_TX.send(RF_TX_BUF_7_1, 5)){
                    counter++;
                  }else{
                    Serial.println("Transmit Failed");
                  }

                  Serial.print("Totals : ");
                  Serial.print(counter);
                  Serial.println(" Data");
                  Serial.println("+++++++");
                  break;  

        default : Serial.print("Input Invalid : ");
                  Serial.println(UserInput);
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

void Clr_RF_RX_BUF(){
  for(int i=0; i<sizeof(RF_RX_BUF); i++){
    RF_RX_BUF[i] = 0xFF;
  }
}