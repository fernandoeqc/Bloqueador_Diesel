#include <16F1823.h>
#device ADC=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOMCLR                   //Master Clear pin used for I/O
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(internal=4000000)
#pragma use rs232(baud=9600,parity=N,xmit=PIN_A0,rcv=PIN_A1,bits=8,stream=PORT1)

#define TIMER0_F
#define LED1 PIN_A5
#define LED2 PIN_A4
#define SAIDA1 PIN_C5 //RL rele

//PORTA
#define FIM_CURSO_IN PIN_A2
//#define FIM_CURSO    PIN_A3
#define MOTOR1       PIN_A4
#define MOTOR2       PIN_A5


//PORTC
#define ADC_BAT      4 //PIN_C0
#define ADC_ALIM     5 //PIN_C1
//#define LED2         PIN_C2
#define CARGA_BAT    PIN_C3
#define LED1         PIN_C4
#define LED2         PIN_C5

#define PISCALEDSTATUS_F
#define EEPROM_F
#define TIMER0_F

//EEPROM===========================
//SELETOR DE FREQUENCIA - byte 0A
#pragma ROM 0xF000 = {0x00}
#define EP_CONTROL_FLAGS 0x00,0x01

//SELETOR DE FREQUENCIA - byte 0A
#pragma ROM 0xF014 = {0x00}
#define EP_ID 0x01,0x01

//ERROS LOGICA - #define DEBUG
#pragma ROM 0xF015 = {0x00}
#define EP_DEBUG 0x02,0x01


