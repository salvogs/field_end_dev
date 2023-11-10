#ifndef ENGINE_FUNCTION_H_
#define ENGINE_FUNCTION_H_

#include "../header/macros.h"
#include "../PCA9539.h"


struct  tc {
  bool doCount;
  int counter;
  int last;
  int time;
};

typedef struct tc TimeCounter;


void engineUp(uint8_t e_id, uint8_t counter);
void engineDown(uint8_t e_id, uint8_t counter);
void engineStop(uint8_t e_id, bool force_stop);
// void stampaPressioni(uint8_t counter, uint8_t index);
void contaPressioni(uint8_t index);
// uint8_t checkManualStop(uint8_t index);
void initPin();
int checkConfigMode();
void setFullTime1(float t);
void setFullTime2(float t);
void writeOpenTime(int addr, float value);
void setStep1(float step);
void setStep2(float step);
void configMode();
int checkStopEngine1();
int checkStopEngine2();


#endif

