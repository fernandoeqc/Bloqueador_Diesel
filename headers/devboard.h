//!#IF 5 == 5
//!   #ERROR LCD e LED_D usando a mesma porta!
//!#ENDIF

int1 listen_bus(void);
int detecta_freq(void);
void setup_can(void);
void setup_lcd(void);
void setup_iterrupt(void);
void setup_io(void);



#ifdef LED_B
   #define LED0  PIN_B0
   #define LED1  PIN_B1
   #define LED2  PIN_B2
   #define LED3  PIN_B3
   #define LED4  PIN_B4
   #define LED5  PIN_B5
   #define LED6  PIN_B6
   #define LED7  PIN_B7
#else
   #ifdef LED_D
      #define LED0 PIN_D0
      #define LED1 PIN_D1
      #define LED2 PIN_D2
      #define LED3 PIN_D3
      #define LED4 PIN_D4
      #define LED5 PIN_D5
      #define LED6 PIN_D6
      #define LED7 PIN_D7
   #endif
#endif

#ifdef SPI1_ON
   #define CS    PIN_C2
   #define SDO   PIN_C5
   #define SDI   PIN_C4  
   #define SCK   PIN_C3
#endif

