#include "./header/engine_function.h"
#include <Arduino.h>
// #include "../libs/PCA9539.h"
#include "./header/macros.h"



uint32_t t = 0;


void setup() {

  Serial.begin(115200);



  Serial.println("setup...");
  delay(100);

  initPin();
  
  delay(100);

  Serial.println("done");
}


void loop() {


  // controllo se si vuole entrare in config mode
  // i motori devono trovarsi in finecorsa_down 
  if(checkConfigMode() == 1) configMode();
  

  for(uint8_t i = 0; i < 8; i++) {

    if(i == UP1 || i == DOWN1 || i == UP2 || i == DOWN2) contaPressioni(i);
    
    if(i == FINECORSA_UP1 || i == FINECORSA_DOWN1 || i == FINECORSA_UP2 || i == FINECORSA_DOWN2) engineStop((i % 2) + 1,false);
    
    if(checkStopEngine1() == 1) engineStop(1,true);
    if(checkStopEngine2() == 1) engineStop(2,true);
  }

  // stampa di debug
  // if(millis() - t > 5000) {
  //   // Serial.printf("\tUP --- DOWN\nmotore1\t%d  ---  %d\nmotore2\t%d  ---  %d\n",alzo1,abbasso1,alzo2,abbasso2);
  //   t = millis();
  // }
}





  
     
  
  






