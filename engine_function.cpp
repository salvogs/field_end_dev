#include <sys/types.h>
#include <Arduino.h>
#include "./header/engine_function.h"


PCA9539 ioport(0x74);  // Base address starts at 0x74 for A0 = L and A1 = L

// contatori per pulsanti MOTORI UP/DOWN
TimeCounter counters[4];

uint32_t e1_time = millis();
uint32_t e2_time = millis();
u_short pression_1 = 0;
u_short pression_2 = 0;
float full_time_motore1 = 0, full_time_motore2 = 0, step_1 = 0, step_2 = 0;


bool alzo1 = false, abbasso1 = false;
bool alzo2 = false, abbasso2 = false;
bool stop1 = false, stop2 = false;
// stato motori: 0 - fermo, 1 - movimento
//engine_state[0] -> motore 1
//engine_state[1] ->
// bool engine_state[2]; 

void engineUp(uint8_t e_id, uint8_t counter) {
    

  //controllo se finecorsa superiore motore non raggiunto e non sto già alzando
  if(ioport.PCARead(e_id == 1 ? FINECORSA_UP1 : FINECORSA_UP2) == 1 && !(e_id == 1 ? alzo1 : alzo2)) {
    // se finecorsa inferiore raggiunto => alzo tutto
    if(e_id == 1) {
      Serial.println("ALZO MOTORE 1");
      e1_time = millis();
      alzo1 = true;
    } else {
      Serial.println("ALZO MOTORE 2");
      e2_time = millis();
      alzo2 = true;
    }
  } 
  /***  TODO ALZO MOTORE DI UNO STEP (per X tempo)***/

}

void engineDown(uint8_t e_id, uint8_t counter) {
  //controllo se finecorsa inferiore motore non raggiunto
  if(ioport.PCARead(e_id == 1 ? FINECORSA_DOWN1 : FINECORSA_DOWN2) == 1 && !(e_id == 1 ? abbasso1 : abbasso2)) {
    // se finecorsa superiore raggiunto => abbasso tutto
    if(e_id == 1) {
      Serial.println("ABBASSO MOTORE 1");
      e1_time = millis();
      abbasso1 = true;
    } else {
      Serial.println("ABBASSO MOTORE 2");
      e2_time = millis();
      abbasso2 = true;
    }
  } 
}


void engineStop(uint8_t e_id, bool force_stop) {

  if(e_id == 1 && (force_stop || ((ioport.PCARead(FINECORSA_UP1) == 0) || (ioport.PCARead(FINECORSA_DOWN1) == 0)))) {
    if(alzo1 || abbasso1) Serial.printf("STOP MOTORE 1 --- tempo: %.2f\n", (millis()-e1_time)/1000.00);
    alzo1 = false;
    abbasso1 = false;
    counters[0].counter = 0;
    counters[2].counter = 0;
    pression_1 = 0;
       
  } else if(force_stop || (ioport.PCARead(FINECORSA_UP2) == 0) || (ioport.PCARead(FINECORSA_DOWN2) == 0)) {
     if(alzo2 || abbasso2) Serial.printf("STOP MOTORE 2 --- tempo: %.2f\n", (millis()-e2_time)/1000.00);
    alzo2 = false;
    abbasso2 = false;
    counters[1].counter = 0;
    counters[3].counter = 0;
    pression_2 = 0;
  }

}


void doAction(uint8_t counter, uint8_t index) {

  if(counter > 0) {
    switch(index) {
      case UP1: {
        Serial.printf("UP MOTORE 1 --- %d\n",counter);
        pression_1 = counter;
        engineUp(1,counter);
      }
      break;

      case DOWN1: {
        Serial.printf("DOWN MOTORE 1 --- %d\n",counter);
        pression_1 = counter;
        engineDown(1,counter);
      }
      break;
      case UP2: {
        Serial.printf("UP MOTORE 2 --- %d\n",counter);
        pression_2 = counter;
        engineUp(2,counter);
      }
      break;
      case DOWN2: {
        Serial.printf("DOWN MOTORE 2 --- %d\n",counter);
        pression_2 = counter;
        engineDown(2,counter);
      }
      break;
    }
  }
}



void contaPressioni(uint8_t index) {

  TimeCounter *t = &counters[index > 1 ? index - 2 : index];
  
  // inizio a contare
  if(ioport.PCARead(index) == 0) {
    if(!t->doCount) {
      t->doCount = true;
      t->time = millis();
      t->last = 0;
      t->counter = 0;
    }
    // pulsante premuto per meno di 3 secondi e sono passati almeno 200ms da ultima pressione (e max 600ms)
    if(t->doCount && (millis() - t->time) <= 3000) {
      
      if(millis() - t->last > 200 && (t->counter == 0 || millis() - t->last  < 600)) {
        t->counter++;
        t->last = millis();
      }

    }
 
  } else if(ioport.PCARead(index) == 1) {
      if(t->doCount && (millis() - t->time) > 3000) {
        t->doCount = false;
        doAction(t->counter,index);
        t->counter = 0;
      }

  }
}

int checkConfigMode() {
  if(ioport.PCARead(FINECORSA_DOWN1) == 0 && ioport.PCARead(FINECORSA_DOWN2) == 0 && \
     ioport.PCARead(UP1) == 0 && ioport.PCARead(UP2) == 0 && ioport.PCARead(DOWN1) == 0 && ioport.PCARead(DOWN2) == 0){
    
    return 1;
  }

  return 0;
}

void configMode() {
  Serial.println("Conf mode...");
  delay(1000);
  Serial.println("Alzo Motore 1");
  int t = millis();
  float temp1 = 0, temp2 = 0;    
  do {

    // problema motore se passati piu' di MAX_CONF_TIME secondi
    if((millis() - t) / 1000.00 > MAX_CONF_TIME) {
      Serial.println("Problema motore 1"); 
      break;
    }

  } while(ioport.PCARead(FINECORSA_UP1) == 1);

  if(ioport.PCARead(FINECORSA_UP1) == 0) {
      temp1 = (millis() - t) / 1000.00;
      Serial.printf("Stop Motore 1 -- tempo salita:%.2f\n", temp1);
  }

  delay(1000);

  Serial.println("Alzo Motore 2");
  t = millis();
  do {

    // problema motore se passati piu' di MAX_CONF_TIME secondi
    if((millis() - t) / 1000.00 > MAX_CONF_TIME) {
      Serial.println("Problema motore 2"); 
      break;
    }

  } while(ioport.PCARead(FINECORSA_UP2) == 1);

  if(ioport.PCARead(FINECORSA_UP2) == 0) {
      temp2 = (millis() - t) / 1000.00;
      Serial.printf("Stop Motore 2 -- tempo salita:%.2f\n", temp2);
  }

  // calcolo steps

  if(temp1 != 0) {
    full_time_motore1 = temp1;
    step_1 = full_time_motore1 / STEPS;
    Serial.printf("Step motore 1: %.2f\n", step_1);
  }

  if(temp2 != 0) {
    full_time_motore2 = temp2;
    step_2 = full_time_motore2 / STEPS;
    Serial.printf("Step motore 2: %.2f\n", step_2);
  }
}


int checkStopEngine1() {
  return (alzo1 || abbasso1) && e1_time > 0 && ((millis()-e1_time)/1000.00) > step_1 * pression_1 ? 1 : 0;
}

int checkStopEngine2() {
  return (alzo2 || abbasso2) && e2_time > 0 && ((millis()-e2_time)/1000.00) > step_2 * pression_2 ? 1 : 0;
}

void initPin() {
  ioport.PMode(0, PIN0);
  ioport.PMode(1, PIN1);
  ioport.PMode(2, PIN2);
  ioport.PMode(3, PIN3);
  ioport.PMode(4, PIN4);
  ioport.PMode(5, PIN5);
  ioport.PMode(6, PIN6);
  ioport.PMode(7, PIN7);
  ioport.PMode(8, PIN8);
  ioport.PMode(9, PIN9);
  ioport.PMode(10, PIN10);
  ioport.PMode(11, PIN11);
  ioport.PMode(12, PIN12);
  ioport.PMode(13, PIN13);
  ioport.PMode(14, PIN14);
  ioport.PMode(15, PIN15);

  for (int i = 0; i < 16; i++) {
    ioport.PCAWrite(i, 0);
  }
}