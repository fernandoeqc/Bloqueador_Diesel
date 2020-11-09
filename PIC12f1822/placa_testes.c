#include <placa_testes.h>
#define TIMER0_F

#include <functions.h>

void tabelaVerdade(void) {
      

}


void main()
{
   
   //===========REGISTRADORES===================================
   disable_interrupts(GLOBAL); // habilitar interr global
   enable_interrupts(INT_RDA); //UART
   //enable_interrupts(INT_EXT_H2L);         // interrupcao CAN
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // setar timer1 para interno
   enable_interrupts(INT_TIMER1);            // habilita Timer1
   set_timer1(0);                            // limpar flag TMR1H & TMR1L
   counter = int_per_sec;
   enable_interrupts(GLOBAL); // habilitar interr global
   //----------------------------------------------------------
   
   while(true) {
      if (um_segundo) {
      
      
      }
   }

}
