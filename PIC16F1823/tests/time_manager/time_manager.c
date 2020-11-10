#define TEMPOCICLOLEDS 30
#define DEBUG

#include <time_manager.h>
#include <ep_debug.h>
#include <stddef.h>
#include <functions.h>
#include <tasks.h>





/* volatile unsigned int8 contaPeriodo = TEMPOCICLOLEDS;
void piscaLedStatus(struct latx *lat){
   struct latx latCopy;
   latCopy = *lat;

   //responsavel por desligar o led no segundo periodo
   //se led esta ligado, desliga
   if(bit_test(*latCopy.porta,latCopy.pino))
   {
      bit_clear(*latCopy.porta,latCopy.pino);
      return;
   }



   *lat = latCopy;
}


void gerenciaTempo(struct tempo *tmp) {
   int a = 0;
   struct tempo b_tmp;
   b_tmp = *tmp;

   b_tmp.conta_seg ++;
   a = b_tmp.conta_seg;
   
   if(a == b_tmp.seg)
   {
      b_tmp.conta_seg = 0;
      b_tmp.flag = 1;
      a = 0;
   }

   *tmp = b_tmp;
}
*/

void waitEndMotor (struct task *tk) {
   static int8 count_wait_init = 0;

   if(!FIM_CURSO_IN) {
      
   }
}


int main(void) {

   struct task contaBloq;
   contaBloq.sec = 0x06;
   contaBloq.count_sec = 0x00;
   contaBloq.command = 'W';
   contaBloq.flag = FALSE;
   contaBloq.active = TRUE;
   contaBloq.func = ativaMotor;
   
   struct task contaBat;
   contaBat.sec = 0x03;
   contaBat.count_sec = 0x00;
   contaBat.command = 'O';
   contaBat.flag = FALSE;
   contaBat.active = TRUE;
   contaBat.func = ativaBat;
   
   struct task waitMotor;
   contaBat.sec = 0x05;
   contaBat.count_sec = 0x00;
   contaBat.command = 'O';
   contaBat.flag = FALSE;
   contaBat.active = TRUE;
   contaBat.func = waitEndMotor;


   initTasks ();
   
   addTask (&contaBat);
   addTask (&contaBloq);

  
   disable_interrupts(GLOBAL);                 // habilitar interr global
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);   // setar timer1 para interno
   enable_interrupts(INT_TIMER1);              // habilita Timer1 
   set_timer1(0);                              // limpar flag TMR1H & TMR1L 
   counter=int_per_sec;
   enable_interrupts(GLOBAL);                  // habilitar interr global

   output_low(LED2);
   output_low(LED2);
   
   while(TRUE)
   {      

      
      /* if(flag_interr)
      {          
         flag_interr = 0b0;
      }

      if(contaBloq.flag)
      {


      }

       if(um_periodo)
      {
         um_periodo = 0;

         //=passagem LATA.4 como parametro
         //piscaLedStatus(&LATA4);
         
         contaPeriodo--;
         if(contaPeriodo == 0)
         {
            contaPeriodo = TEMPOCICLOLEDS;
         }
      }   
      */

      if(um_segundo) {
         um_segundo = 0b0;

         //timeManager(&contaBloq, ativaMotor);
         runTasks();
                 
         //piscaLed(1,50,LED2);
         
         //can_putd(0x71F,dadosEnv,2,0,0,0);

      }
   }
}
