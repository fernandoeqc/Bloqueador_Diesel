#define TEMPOCICLOLEDS 30

#define MAX_WDT_UART_TIME 5
#define MAX_TRANSITION_TIME 20

#define TIME_BEFORE_BLOQ 5

#define UART_BLOQ_COMMAND 'B'
#define UART_DESBLOQ_COMMAND 'D'
#define UART_RESTART_COMMAND 'R'
#define QT_COMMANDS 3

#define DEBUG 

#include <time_manager.h>
#include <stddef.h>
#include <functions.h>
#include <tasks.h>

static unsigned int8 valid_commands[QT_COMMANDS] = {
   UART_BLOQ_COMMAND, 
   UART_DESBLOQ_COMMAND, 
   UART_RESTART_COMMAND
};

unsigned char actual_data = '-';
unsigned char com_uart;

//status do motor
#define ERROR 0
#define OPENED 1
#define CLOSED 2
#define TRANSITION 3

//movimento do motor
#define POSIX_PARKED 0
#define POSIX_OPENING 1
#define POSIX_CLOSING 2
#define POSIX_ERROR 3


#pragma INT_RDA
void RDA_isr(void) {  
   actual_data = getch();
}


struct flags {
   int1 power:1; //alimentacao
   int1 uart:1;  //comandos de bloq e desbloq
   int1 com_time:1; //tempo sem comunicacao
   int1 restart:1;
}flagsControl;


unsigned int8 getFlags(struct flags flag) {
   unsigned int8 flg = flag; //== NAO MEXER, TA FUNCIONANDO ==
   return flg;
}

//=======================================================================
//Retorna 1 se mudou de 0 pra 1+, ou de 1+ pra 0; retorna 0 se nao mudou
//Sempre vai retornar 1 no inicio do programa
//=======================================================================
unsigned int1 verifyFlags (struct flags flag) {
   static unsigned int8 last_flag = 0xFF; 
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


void ativaMotor(struct taskData *tk) { 

   //tk->active = FALSE;
   //tk->flag = TRUE;
   
   if (tk->command == POSIX_OPENING) {
      tk->state = TRANSITION;
      output_low(MOTOR1);
      delay_ms(100);
      output_high(MOTOR2);

      output_low(LED2);
   }

   else if (tk->command == POSIX_CLOSING) {
      tk->state = TRANSITION;
      output_low(MOTOR2);
      delay_ms(100);
      output_high(MOTOR1);

      output_high(LED1);
   }
}


enum {NEW_DATA, GET_LAST};
char getCommand(unsigned int8 get) {
   static unsigned char last_data = 0;
   
   if (isValidCommand(actual_data)) {
      last_data = actual_data;

      switch (get) {
         case NEW_DATA:
            actual_data = 0;
            return last_data;
               
         case GET_LAST:
            return last_data;
      }
   }

   else {
      return 0;
   }
}


void checkTimeMessage(struct taskData *tk) {

   tk->command = getCommand(NEW_DATA);
   
   //se recebeu um comando valido
   if (tk->command) {
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
   

   com_uart = getCommand(GET_LAST);

   if (com_uart == UART_BLOQ_COMMAND) {
      flagsControl.uart = TRUE;
      //bit_set (com, 2);
      tk->command == com_uart;
   }

   else if (com_uart == UART_DESBLOQ_COMMAND) {
      flagsControl.uart = FALSE;
      //bit_clear (com, 2);
      tk->command == com_uart;
   }

   else if (com_uart == UART_RESTART_COMMAND) {
      flagsControl.restart = TRUE;
   }
}


void checkPowerIn(struct taskData *tk) {   
   
   if(input(POWER_IN)) {
      flagsControl.power = FALSE;
      //output_high(LED2);
   }
   else {
      flagsControl.power = TRUE;
      //output_low(LED2);
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

struct taskFunc toggleOpenClose (struct taskFunc tk) {
   
   if (flagsControl.com_time) {
         tk.sec = TIME_BEFORE_BLOQ;
         tk.count_sec = 0;
         tk.data.command = POSIX_CLOSING;
      }
      
      else if (flagsControl.power) {
         tk.sec = TIME_BEFORE_BLOQ;
         tk.count_sec = 0;
         tk.data.command = POSIX_CLOSING;
      }

      else if (flagsControl.uart) {
         tk.sec = 0;
         tk.count_sec = 0;
         tk.data.command = POSIX_CLOSING;
      }

      else if (flagsControl.restart){ 
         flagsControl.restart = FALSE;
         
         eeprom_grava (EP_MOTOR_COMMAND, tk.data.command);
         delay_ms(50);
         eeprom_grava (EP_CONTROL_FLAGS, getFlags(flagsControl));
         delay_ms(50);
         reset_cpu ();
      }
      

      //desbloqueio
      else {
         tk.sec = 0;
         tk.count_sec = 0;
         tk.data.command = POSIX_OPENING;
      }
      
      if(tk.data.state == tk.data.command) {
         tk.data.active = FALSE;
      }
      else {
         tk.data.active = TRUE;
      }

      return tk;
}

void statusMotor(struct taskFunc *tk) {
   static unsigned int8  count_err_trans = 0;
   static unsigned int8 flg_transition = 0, position_motor = 0;

   position_motor = checkMotorPosition();
   
   if (
         ((tk->data.state == OPENED) || (tk->data.state == CLOSED)) && 
         (verifyFlags(flagsControl))
      ) {
      
      *tk = toggleOpenClose(*tk);
      
   }

   else if (tk->data.state == TRANSITION) {

      if (position_motor == POSIX_PARKED) {
         if(!flg_transition) {
            //motor ja está na posição ====== fazer testes
            //tk->data.state = flg_transition;
            //tk->data.active = FALSE;
            //flg_transition = 0;
         
            printf("aguardando M |\r\n");
         }
         else {
            //fim da transicao
            tk->data.state = flg_transition;
            tk->data.active = FALSE;
            flg_transition = 0;
            
            count_err_trans = 0;
            printf("M fim _\r\n");
         }
      }
      
      else if (position_motor == POSIX_OPENING) {
         //motor esta abrindo
         flg_transition = position_motor;
         printf("M abrindo /\r\n");
      }
      
      else if (position_motor == POSIX_CLOSING) {
         //motor esta fechando
         flg_transition = position_motor;
         printf("M fechando \\\r\n");
      }

      count_err_trans++;
      if (count_err_trans > MAX_TRANSITION_TIME) {
         printf("error_max_timeout\r\n");
         reset_cpu();
         //tk->data.state = ERROR;
         //tk->data.state = FALSE;
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

   struct taskFunc contaBloq;
   contaBloq.sec = 0x00;
   contaBloq.count_sec = 0x00;
   contaBloq.data.command = POSIX_OPENING;
   contaBloq.data.state = OPENED;
   contaBloq.data.flag = FALSE;
   contaBloq.data.active = FALSE;
   contaBloq.func_time = ativaMotor;

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
   uart.data.active = TRUE;
   uart.func_time = checkCommandsUart; 


   struct taskFunc powerIn;
   powerIn.sec = 0x00;
   powerIn.count_sec = 0x00;
   powerIn.data.command = 't';
   powerIn.data.state = 'X';
   powerIn.data.flag = FALSE;
   powerIn.data.active = TRUE;
   powerIn.func_time = checkPowerIn; 
   
   //initTasks (); //necessario?
   
   addTask (&uart);
   addTask (&timeReceive);
   addTask (&powerIn);
   addTask (&contaBloq);

   unsigned int8 i;
   i = eeprom_le(EP_MOTOR_COMMAND);
   if (i != 0xFF) contaBloq.data.command = i;

//COLOCAR NAS FLAGS
/*    i = eeprom_le(EP_CONTROL_FLAGS);
   if (i != 0xFF) contaBloq.data.command = i; */
  
   output_low(MOTOR1);
   delay_ms(100);
   output_high(MOTOR2);

   output_low (LED1);
   output_low (LED2);

   for(i = 0; i < 6; i++) {
      output_toggle(LED1);
      output_toggle(LED2);
      delay_ms(150);
   }
   
   //===========REGISTRADORES===================================
   disable_interrupts(GLOBAL);               // habilitar interr global
   enable_interrupts(INT_RDA);               //UART
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // setar timer1 para interno
   enable_interrupts(INT_TIMER1);            // habilita Timer1
   set_timer1(0);                            // limpar flag TMR1H & TMR1L
   counter = int_per_sec;
   enable_interrupts(GLOBAL);                // habilitar interr global
   //----------------------------------------------------------
   
   
   while (TRUE) {      
      

      if(um_segundo) {
         um_segundo = 0b0;
         printf("flags: %u%u%u\r\n", flagsControl.power, 
                                    flagsControl.uart, 
                                    flagsControl.com_time);
         statusMotor(&contaBloq);
         runTasks();
         
 
         
         output_toggle(LED1);
      }
   }
}
