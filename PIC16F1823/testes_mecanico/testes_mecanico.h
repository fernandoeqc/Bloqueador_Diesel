#include <16F1823.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES PUT                      //Power Up Timer
#FUSES NOMCLR                   //Master Clear pin used for I/O
#FUSES BROWNOUT_NOSL            //Brownout enabled during operation, disabled during SLEEP
#FUSES BORV25                   //Brownout reset at 2.5V
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(internal=4000000)

