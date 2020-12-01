#include <16F1823.h>
#device ADC=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOMCLR                   //Master Clear pin used for I/O
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES BORV25                   //Brownout reset at 2.5V

#use delay(internal=4000000)
#use rs232(baud=9600, uart1, ERRORS)

#byte APFCON = 0x11D
#bit TXSEL = APFCON.2
#bit RXSEL = APFCON.7


//PORTA
#define FIM_CURSO_1  PIN_A2
//#define FIM_CURSO_2  PIN_A3
#define MOTOR1       PIN_A4
#define MOTOR2       PIN_A5


//PORTC
#define ADC_BAT      4 //PIN_C0
#define POWER_IN     PIN_C1
#define FIM_CURSO_2  PIN_C2
#define CARGA_BAT    PIN_C3
#define LED1         PIN_C4
#define LED2         PIN_C5

//#define PISCALEDSTATUS_F
#define EEPROM_F
#define TIMER0_F

//EEPROM===========================
//FLAGS DE CONTROLE - byte 0A
//#pragma ROM 0xF000 = {0xFF}
#define EP_CONTROL_FLAGS 0x00,0x01

//ULTIMO COMANDO DADO AO MOTOR
//#pragma ROM 0xF001 = {0xFF}
#define EP_MOTOR_COMMAND 0x01,0x01

//ERROS LOGICA - #define DEBUG
//#pragma ROM 0xF015 = {0xFF}
#define EP_DEBUG 0x02,0x01


