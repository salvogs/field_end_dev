#ifndef MACROS_H_
#define MACROS_H_


//DEFINIZIONE I/O DEL PORT EXPANDER ----------------------------------------------------------------
// ----CONNETTORE  P1 0-7
#define PIN0 INPUT
#define PIN1 INPUT
#define PIN2 INPUT
#define PIN3 INPUT
#define PIN4 INPUT
#define PIN5 INPUT
#define PIN6 INPUT
#define PIN7 INPUT
// ----CONNETTORE  P2  0-7
#define PIN8 OUTPUT
#define PIN9 OUTPUT
#define PIN10 OUTPUT
#define PIN11 OUTPUT
#define PIN12 OUTPUT
#define PIN13 OUTPUT
#define PIN14 OUTPUT
#define PIN15 OUTPUT



#define UP1 0
#define DOWN1 4
#define UP2 1
#define DOWN2 5
#define FINECORSA_UP1 2
#define FINECORSA_DOWN1 6
#define FINECORSA_UP2 3
#define FINECORSA_DOWN2 7


#define MAX_CONF_TIME 60
#define STEPS 5

#define ACT1 8
#define ACT2 9
#define ACT3 10
#define ACT4 11


// pin motori
#define OUT_UP1 GPIO7
#define OUT_DOWN1 GPIO8
#define OUT_UP2 GPIO9
#define OUT_DOWN2 GPIO10


#define ADDR_T1 1
#define ADDR_T2 5

#endif
