#define TEMPOCICLOLEDS 30

#define MAX_WDT_UART_TIME 5
#define MAX_TRANSITION_TIME 10

#define TIME_BEFORE_BLOQ 5


#define OPEN_DIESEL 'C'
#define CLOSE_DIESEL 'O'

#define UART_BLOQ_COMMAND 'B'
#define UART_DESBLOQ_COMMAND 'D'
#define UART_RESTART_COMMAND 'R'
#define QT_COMMANDS 3

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

*/

static unsigned int8 valid_commands[QT_COMMANDS] = {
   UART_BLOQ_COMMAND, 
   UART_DESBLOQ_COMMAND, 
   UART_RESTART_COMMAND
};

unsigned char actual_data = '-';
static unsigned int8 flags_control = 0;

enum status_motor {
   PARKED,
   TRANSITION,
   ERROR
};

#pragma INT_RDA
void RDA_isr(void) {  
   actual_data = getch();
}


struct flags {
   int1 power:1; //alimentacao
   int1 uart:1;  //comandos de bloq e desbloq
   int1 com_time:1; //tempo sem comunicacao
}flagsControl;


unsigned int8 getFlags(struct flags flag) {
   unsigned int8 flg = flag; //== NAO MEXER, TA FUNCIONANDO ==
   return flg;
}


unsigned int1 verifyFlags (struct flags flag) {
   static unsigned int8 last_flag = 0;
   unsigned int8 flg; 
   
   flg = getFlags(flag);

   //se mudou de 0 pra >0, ou de >0 pra 0
   if (last_flag ^ flg) {
      last_flag = flg;
      return 1;
   }
   else return 0;
}


unsigned int1 isValidCommand(unsigned char command) {
   unsigned int8 i;

   for (i = 0; i < QT_COMMANDS; i++) {
      if (valid_commands[i] == command) {
         return 1;
      }
   }
   return 0;
}


int1 waitEndMotor (struct taskData *tk) {
   static unsigned int8 count_wait_init = 0;

   //if (FIM_CURSO_IN) {   
   //   count_wait_init++;
   //}

   if (count_wait_init > 0) {
      count_wait_init = 0;
      tk->flag = FALSE;
   }
   return 0;
}


void ativaMotor(struct taskData *tk) { 

   //tk->active = FALSE;
   //tk->flag = TRUE;
   
   if (tk->command == OPEN_DIESEL) {
      tk->state = TRANSITION;
      output_low(MOTOR1);
      delay_ms(100);
      output_high(MOTOR2);

   }

   else if (tk->command == CLOSE_DIESEL) {
      tk->state = TRANSITION;
      output_low(MOTOR2);
      delay_ms(100);
      output_high(MOTOR1);
   }
}


void ativaBat(struct taskData *tk) {
   struct taskData t_tk;
   //t_tk = *tk;


   //*tk = t_tk;
}


enum {NEW_DATA, GET_LAST};
char getCommand(unsigned int8 get) {
   static unsigned char last_data = '0';
   
   switch (get) {
      case NEW_DATA:

         if (isValidCommand(actual_data)) {
            last_data = actual_data;
            actual_data = '0';
            return last_data;
         }

         else {
            return '0';
         }
      
      case GET_LAST:
         return last_data;
   }
}


void checkTimeMessage(struct taskData *tk) {

   tk->command = getCommand(NEW_DATA);
   
   //se recebeu um comando valido
   if (tk->command != '0') {
      tk->state = 0;
      flagsControl.com_time = FALSE;
      return;
   }
   //se atingiu o tempo maximo sem comunicacao 
   else if (tk->state >= MAX_WDT_UART_TIME) {
      flagsControl.com_time = TRUE;
      return;
   }
   //conta sem comunicacao
   tk->state++;
}


void checkCommandsUart(struct taskData *tk) {
   unsigned char cmd; 
 
   cmd = getCommand(GET_LAST);

   if (cmd == UART_BLOQ_COMMAND) {
      flagsControl.uart = TRUE;
      //bit_set (flags_control, 2);
      tk->command == cmd;
   }

   else if (cmd == UART_DESBLOQ_COMMAND) {
      flagsControl.uart = FALSE;
      //bit_clear (flags_control, 2);
      tk->command == cmd;
   }
}


void checkPowerIn(struct taskData *tk) {   
   
   if(input(POWER_IN)) {
      flagsControl.power = FALSE;
      //bit_set(flags_control, 1); //flags_control obsoleto
   }
   else {
      flagsControl.power = TRUE;
      //bit_clear(flags_control, 1);
   }
}


unsigned char checkMotorPosition () {
   unsigned int8 reading = 0;
   
   if (input(FIM_CURSO_1)) {
      bit_set (reading, 0);
   }

   if (input(FIM_CURSO_2)) {
      bit_set (reading, 1);
   }

   //se os dois estao nivel alto, motor esta com defeito 
   return reading;
}


void statusMotor(struct taskFunc *tk) {
   static unsigned int1 flg_transition = 0, count_err_trans = 0;

   if (tk->data.state == PARKED) {
      if (verifyFlags(flagsControl)) {

         if (checkMotorPosition()){
            printf("error_init_state\r\n");
            tk->data.state = ERROR;
         }
         //em ordem de menos importante para mais importante
         if (flagsControl.com_time) {
            tk->sec = TIME_BEFORE_BLOQ;
            tk->count_sec = 0;
            tk->data.command = CLOSE_DIESEL;
            tk->data.active = TRUE;
         }
         
         else if (flagsControl.power) {
            tk->sec = TIME_BEFORE_BLOQ;
            tk->count_sec = 0;
            tk->data.command = CLOSE_DIESEL;
            tk->data.active = TRUE;
         }

         else if (flagsControl.uart) {
            tk->sec = TIME_BEFORE_BLOQ;
            tk->count_sec = 0;
            tk->data.command = CLOSE_DIESEL;
            tk->data.active = TRUE;
         }

         //desbloqueio
         else {
            tk->sec = 0;
            tk->count_sec = 0;
            tk->data.command = OPEN_DIESEL;
            tk->data.active = TRUE;
         }
      }
      //parado, mas inputs inalterados
   }

   else if (tk->data.state == TRANSITION) {
      if (!checkMotorPosition()) {
         if(!flg_transition) {
            //esperando inicio do giro do motor
            printf("|\r\n");
         }
         else {
            //fim da transicao
            flg_transition = 0;
            tk->data.state = PARKED;
            tk->data.active = FALSE;
            
            count_err_trans = 0;
            printf("_\r\n");
         }
      }
      else {
         //motor esta em transicao
         flg_transition = 1;
         printf("/\r\n");
      }

      count_err_trans++;
      if (count_err_trans > MAX_TRANSITION_TIME) {
         printf("error_max_timeout\r\n");
         tk->data.state = ERROR;
      }
   }
   
   else if (tk->data.state == ERROR) {
      
   }
   //se nao entrou em nenhum, erro!

}


int main (void) {
   
   flagsControl.power = FALSE;
   flagsControl.uart = FALSE;
   flagsControl.com_time = FALSE;

   struct taskFunc timeReceive;
   timeReceive.sec = 0x00;
   timeReceive.count_sec = 0x00;
   timeReceive.data.command = 't';
   timeReceive.data.state = 0;
   timeReceive.data.flag = FALSE;
   timeReceive.data.active = TRUE;
   timeReceive.func_time = checkTimeMessage;

   struct taskFunc uart;
   uart.sec = 0x00;
   uart.count_sec = 0x00;
   uart.data.command = '0';
   uart.data.command = '0';
   uart.data.flag = FALSE;
   uart.data.active = FALSE;
   uart.func_time = checkCommandsUart; 

   struct taskFunc contaBloq;
   contaBloq.sec = 0x00;
   contaBloq.count_sec = 0x00;
   contaBloq.data.command = OPEN_DIESEL;
   contaBloq.data.state = PARKED;
   contaBloq.data.flag = FALSE;
   contaBloq.data.active = FALSE;
   contaBloq.func_time = ativaMotor;

   struct taskFunc powerIn;
   powerIn.sec = 0x00;
   powerIn.count_sec = 0x00;
   powerIn.data.command = 't';
   powerIn.data.state = 'X';
   powerIn.data.flag = FALSE;
   powerIn.data.active = TRUE;
   powerIn.func_time = checkPowerIn; 
   
   //initTasks (); //necessario?
   
   //addTask (&uart);
   addTask (&timeReceive);
   //addTask (&powerIn);
   addTask (&contaBloq);

  
   //===========REGISTRADORES===================================
   disable_interrupts(GLOBAL);               // habilitar interr global
   enable_interrupts(INT_RDA);               //UART
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // setar timer1 para interno
   enable_interrupts(INT_TIMER1);            // habilita Timer1
   set_timer1(0);                            // limpar flag TMR1H & TMR1L
   counter = int_per_sec;
   enable_interrupts(GLOBAL);                // habilitar interr global
   //----------------------------------------------------------

   output_low (LED1);
   output_low (LED2);
   
   while (TRUE) {      
      //verifyFlags(&flagsControl);
      
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

         runTasks();
         statusMotor(&contaBloq);
         printf("flags: %u%u%u\r\n", flagsControl.power, 
                                    flagsControl.uart, 
                                    flagsControl.com_time);
 
         
         //piscaLed(1,50,LED2);
        

      }
   }
}
