#define TEMPOCICLOLEDS 30
#define MAX_UNANSWERED_TIME 5
#define QT_COMMANDS 3
#define DEBUG

#include <time_manager.h>
#include <ep_debug.h>
#include <stddef.h>
#include <functions.h>
#include <tasks.h>
unsigned char actual_data = '-';

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

*/
#pragma INT_RDA
void RDA_isr(void) {  
   actual_data = getch();
}

static unsigned int8 flags_control = 0;
static unsigned int8 valid_commands[QT_COMMANDS] = {'B', 'D', 'R'};

struct flags {
   int1 power:1;
   int1 uart:1;
   int1 com_time:1;
}flagsControl;

unsigned int1 verifyFlags (struct flags *flag) {
   int8 flg = *flag; //== NAO MEXER, TA FUNCIONANDO ==

   if(flg) {
      return TRUE;
   }
   else {
      return FALSE;
   }
}

unsigned int1 isValidCommand(unsigned char command) {
   unsigned int8 i;

   for (i = 0; i < QT_COMMANDS; i++) {
      if (i == command) {
         return 1;
      }
   }
   return 0;
}

int1 waitEndMotor (struct taskData *tk) {
   static unsigned int8 count_wait_init = 0;

   if (FIM_CURSO_IN) {   
      count_wait_init++;
   }

   if (count_wait_init > 0) {
      count_wait_init = 0;
      tk->flag = FALSE;
   }
   return 0;
}


int1 ativaMotor(struct taskData *tk) { 

   tk->active = FALSE;
   tk->flag = TRUE;
   
   if (tk->command == 'O') {
      output_low(MOTOR1);
      output_high(MOTOR2);
   }
   else if (tk->command == 'C') {
      output_low(MOTOR2);
      output_high(MOTOR1);
   }
   else {
      tk->flag = FALSE;
   }


}


void ativaBat(struct taskData *tk) {
   struct taskData t_tk;
   //t_tk = *tk;


   //*tk = t_tk;
}

enum {NEW_DATA, GET_LAST};

char getCommand(unsigned int8 get) {
   static unsigned char actual_data = '0';
   static unsigned char last_data = '0';
   
   switch (get) {
      case NEW_DATA:
         if (actual_data != last_data) {
            last_data = actual_data;
            return actual_data;
         }
         else {
            return '0';
         }
      
      case GET_LAST:
         return last_data;
   }
}

void checkUart(struct taskData *tk) {
   unsigned char cmd; 
 
   cmd = getCommand(GET_LAST);

   if (cmd == 'B') {
      flagsControl.uart = TRUE;
      //bit_set (flags_control, 2);
      tk->command == cmd;
   }

   else if (cmd == 'D') {
      flagsControl.uart = FALSE;
      //bit_clear (flags_control, 2);
      tk->command == cmd;
   }
}


void checkPowerIn(struct taskData *tk) {   
   
   if(POWER_IN) {
      flagsControl.power = FALSE;
      //bit_set(flags_control, 1);
   }
   else {
      flagsControl.power = TRUE;
      //bit_clear(flags_control, 1);
   }
}


void checkTimeMessage(struct taskData *tk) {

   tk->command = getCommand(NEW_DATA);
   
   //se recebeu um comando valido
   if (isValidCommand(tk->command)) {
      tk->state = 0;
      flagsControl.com_time = FALSE;
      return;
   }
   //se atingiu o tempo maximo sem comunicacao 
   else if (tk->state > MAX_UNANSWERED_TIME) {
      flagsControl.com_time = TRUE;
      return;
   }
   //conta sem comunicacao
   tk->state++;
}


void stateMotor() {
   if (verifyFlags(&flagsControl)) {
      
   }
}


int main (void) {
   
   flagsControl.com_time = FALSE;
   flagsControl.power = FALSE;
   flagsControl.uart = FALSE;

   
   struct taskFunc timeReceive;
   timeReceive.sec = 0x00;
   timeReceive.count_sec = 0x00;
   timeReceive.data.command = 't';
   timeReceive.data.state = 0;
   timeReceive.data.flag = FALSE;
   timeReceive.data.active = TRUE;
   timeReceive.func_time = checkTimeMessage;

 /*

   struct taskFunc uart;
   uart.sec = 0x00;
   uart.count_sec = 0x00;
   uart.data.command = '0';
   uart.data.command = '0';
   uart.data.flag = FALSE;
   uart.data.active = TRUE;
   uart.func_time = checkUart;

   struct taskFunc contaBloq;
   contaBloq.sec = 0x00;
   contaBloq.count_sec = 0x00;
   contaBloq.data.command = 'D';
   contaBloq.data.state = 'O';
   contaBloq.data.flag = FALSE;
   contaBloq.data.active = TRUE;
   contaBloq.func_time = ativaMotor;


   struct taskFunc contaBat;
   contaBat.sec = 0x00;
   contaBat.count_sec = 0x00;
   contaBat.data.command = 't';
   contaBat.data.state = 'd';
   contaBat.data.flag = FALSE;
   contaBat.data.active = TRUE;
   contaBat.func_time = ativaBat;
   */

   struct taskFunc powerIn;
   powerIn.sec = 0x00;
   powerIn.count_sec = 0x00;
   powerIn.data.command = 't';
   powerIn.data.state = 'b';
   powerIn.data.flag = FALSE;
   powerIn.data.active = TRUE;
   powerIn.func_time = checkPowerIn; 
   
   //initTasks ();
   
   //addTask (&contaBat);
   //addTask (&contaBloq);
   //addTask (&uart);
   //addTask (&timeReceive);
   addTask (&powerIn);

  
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
      //verifyFlags(&flagsControl);
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
