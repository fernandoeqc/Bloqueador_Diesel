#define TEMPOCICLOLEDS 30
#define MAX_UNANSWERED_TIME 10
#define DEBUG

#include <time_manager.h>
#include <ep_debug.h>
#include <stddef.h>
#include <functions.h>
#include <tasks.h>

static unsigned int8 flags_control = 0;

static unsigned int8 new_data_time = 0;

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

#pragma INT_RDA
void RDA_isr(void) {  
   actual_command = getch();
   new_data_time = 0;

}
*/


int1 waitEndMotor (struct taskData *tk) {
   struct taskData t_tk;
   static unsigned int8 count_wait_init = 0;
   t_tk = *tk;

   if (FIM_CURSO_IN) {   
      count_wait_init++;
   }

   if (count_wait_init > 0) {
      count_wait_init = 0;
      t_tk.flag = FALSE;
   }
   return 0;
}


void checkFlags () {
   unsigned int8 i;
   for (i = 0; i < TASK_LENGHT; i++) {
      /* if (taskList[i]->data.flag) {
         taskList[i]->func_time (&taskList[i]->data);
      } */
   }
}

int1 ativaMotor(struct taskData *tk) { 
   struct taskData t_tk;
   //t_tk = *tk;

   t_tk.active = FALSE;
   t_tk.flag = TRUE;
   
   if (t_tk.command == 'O') {
      output_low(MOTOR1);
      output_high(MOTOR2);
   }
   else if (t_tk.command == 'C') {
      output_low(MOTOR2);
      output_high(MOTOR1);
   }
   else {
      t_tk.flag = FALSE;
   }

   //*tk = t_tk;
}


void ativaBat(struct taskData *tk) {
   struct taskData t_tk;
   //t_tk = *tk;


   //*tk = t_tk;
}
char getCommand() {
   actual_command = getch();

   if
}

void checkUart(struct taskData *tk) {
   struct taskData t_tk;
   static unsigned char actual_command = '0';
   static unsigned char last_command = 0;
   
   t_tk = *tk;

   if (last_command != actual_command) {
      last_command = actual_command;
      actual_command = '0';

      if (last_command == 'B') {
         bit_set (flags_control, 2);
         t_tk.command == last_command;
      }
      
      else if (last_command == 'D') {
         bit_clear (flags_control, 2);
         t_tk.command == last_command;
      }
      
      else {
         return '?';
      }  
   }
   
   *tk = t_tk;

   return actual_command
}


void checkPowerIn(struct taskData *tk) {   
   output_toggle(PIN_C3);
   if(POWER_IN) {
      bit_set(flags_control, 1);
   }
   else {
      bit_clear(flags_control, 1);
   }
}


void checkTimeMessage(struct taskData *tk) {
   struct taskData t_tk;   
   t_tk = *tk;

   
   if (new_data_time > MAX_UNANSWERED_TIME) {
      //para de contar quando completa um byte
      if (new_data_time != 0xff) {
         new_data_time++;
      }
      //bit_set (flags_control, 2);
      output_high(PIN_C5);  
   }

   if(t_tk.state > new_data_time) {
      //tempo foi resetado
      //bit_clear (flags_control, 2);
      output_low(PIN_C5);
   }
   
   t_tk.state = new_data_time;

   *tk = t_tk;
}


void stateMotor() {

}

int main (void) {

   struct taskFunc uart;
   uart.data.sec = 0x00;
   uart.data.count_sec = 0x00;
   uart.data.command = '0';
   uart.data.command = '0';
   uart.data.flag = FALSE;
   uart.data.active = TRUE;
   uart.func_time = checkUart;
   
   struct taskFunc timeReceive;
   timeReceive.data.sec = 0x00;
   timeReceive.data.count_sec = 0x00;
   timeReceive.data.command = '0';
   timeReceive.data.state = 0;
   timeReceive.data.flag = FALSE;
   timeReceive.data.active = TRUE;
   timeReceive.func_time = checkTimeMessage;


 /*

   struct taskFunc contaBloq;
   contaBloq.data.sec = 0x00;
   contaBloq.data.count_sec = 0x00;
   contaBloq.data.command = 'D';
   contaBloq.data.state = 'O';
   contaBloq.data.flag = FALSE;
   contaBloq.data.active = TRUE;
   contaBloq.func_time = ativaMotor;


   struct taskFunc contaBat;
   contaBat.data.sec = 0x00;
   contaBat.data.count_sec = 0x00;
   contaBat.data.command = '0';
   contaBat.data.state = '0';
   contaBat.data.flag = FALSE;
   contaBat.data.active = TRUE;
   contaBat.func_time = ativaBat;

   struct taskFunc powerIn;
   powerIn.data.sec = 0x00;
   powerIn.data.count_sec = 0x00;
   powerIn.data.command = '0';
   powerIn.data.state = '0';
   powerIn.data.flag = FALSE;
   powerIn.data.active = TRUE;
   powerIn.func_time = checkPowerIn; */
   
   //initTasks ();
   //addTask (&contaBat);
   //addTask (&contaBloq);
   //addTask (&uart);
   addTask (&timeReceive);
   //addTask (&powerIn);


  
   //===========REGISTRADORES===================================
   disable_interrupts(GLOBAL);               // habilitar interr global
   enable_interrupts(INT_RDA);               //UART
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // setar timer1 para interno
   enable_interrupts(INT_TIMER1);            // habilita Timer1
   set_timer1(0);                            // limpar flag TMR1H & TMR1L
   counter = int_per_sec;
   enable_interrupts(GLOBAL);                // habilitar interr global
   //----------------------------------------------------------

   output_low (LED2);
   output_low (LED2);
   
   while (TRUE) {      
      runTasks(); 
     
      /* if(flag_interr)
      {          
         flag_interr = 0b0;
      }

       if(um_periodo)
      {
         um_periodo = 0;
      }   
      */

      if(um_segundo) {
         um_segundo = 0b0;
         stateMotor();
         
 
         
         //piscaLed(1,50,LED2);
        

      }
   }
}
