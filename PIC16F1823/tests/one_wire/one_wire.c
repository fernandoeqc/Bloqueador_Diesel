#include <one_wire.h>
#include <placa_recep.h>

#byte MCU_LATA = 0x10C#bit LED1_IN = MCU_LATA.5

unsigned int flag_status = 0, flag_pausa = 0;
////////////////////////////////////////////////////////////////////////
//
// piscaLedStatus()
//
// Recebe: status_led = qte de piscadas
//
//////////////////////////////////////////////////////////////////////////////
void piscaLedStatus(status_led){
   //responsavel por desligar o led no segundo ciclo
   if (LED1_IN)
   {
      output_low(LED1);
      return;
   }

   //entra no inicio de cada ciclo, pausa por (flag_pausa) segundos, e liga led
   if (flag_status == 0)
   {
      flag_status = status_led;
      flag_pausa = 10;      
   }

   //responsavel por pausar antes de cada ciclo
   if (flag_pausa != 0)
   {
      flag_pausa--;
      return;
   }

   output_high(LED1);
   flag_status--;
}


/*    se led == 1
      led = 0
      break
      
   se flag_status == 0
      flag_status = status_led;

   led = 1
   flag_status -= 1
   
       */


void main()
{
   disable_interrupts(GLOBAL);                 // habilitar interr global
   //enable_interrupts(INT_EXT_H2L);             // interrupcao CAN
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);   // setar timer1 para interno
   enable_interrupts(INT_TIMER1);              // habilita Timer1 
   set_timer1(0);                              // limpar flag TMR1H & TMR1L 
   counter=int_per_sec;
   enable_interrupts(GLOBAL);                  // habilitar interr global

   int teste = 0;
   
   while(TRUE)
   {
      
      if(flag_interr)
      {          
         flag_interr = 0b0;
      }
      
      teste = input_c();
      if(um_segundo)
      {      
         um_segundo = 0b0;
         piscaLedStatus(teste);
         //output_toggle(LED1);   
      }


   }
}

