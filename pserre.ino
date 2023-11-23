#include "./header/engine_function.h"
#include <Arduino.h>
// #include "../libs/PCA9539.h"
#include "./header/macros.h"
#include <EEPROM.h>
#include "./header/loraHandler.h"

uint32_t t = 0;
uint16_t address = 0;

void setup() {

  Serial.begin(115200);

  Serial.println("setup...");

  // init EEPROM
  EEPROM.begin(16);

  Serial.println("reading eeprom...");
  
  if(EEPROM.read(0) != 0) {
    float t;

    Serial.print("reading time 1: ");
    EEPROM.get(ADDR_T1,t);
    Serial.println(t);
    setFullTime1(t);
    setStep1(t / STEPS);

    Serial.print("reading time 2: ");
    EEPROM.get(ADDR_T2,t);
    Serial.println(t);
    setFullTime2(t);
    setStep2(t / STEPS);
  }
  EEPROM.end();
  
  delay(100);

  Serial.println("init pins...");
  initPin();
  
  delay(100);

  Serial.println("LoRa setup...");

  loraSetup();

  delay(100);

  Serial.println("all done");
  
}


void loop() {


  // controllo se si vuole entrare in config mode
  // i motori devono trovarsi in finecorsa_down 
  if(checkConfigMode() == 1) configMode();
  

  for(uint8_t i = 0; i < 8; i++) {

    if(i == UP1 || i == DOWN1 || i == UP2 || i == DOWN2) contaPressioni(i);
    
    if(i == FINECORSA_UP1 || i == FINECORSA_DOWN1 || i == FINECORSA_UP2 || i == FINECORSA_DOWN2) engineStop((i % 2) + 1,false);
    
  }
  if(checkStopEngine1() == 1) engineStop(1,true);
  if(checkStopEngine2() == 1) engineStop(2,true);


  loraLoop();

}





  
     
  
  






