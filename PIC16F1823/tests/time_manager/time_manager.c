#define TEMPOCICLOLEDS 30

#define MAX_WDT_UART_TIME 5
#define MAX_TRANSITION_TIME 10

#define TIME_BEFORE_BLOQ 5

#define UART_BLOQ_COMMAND 'B'
#define UART_DESBLOQ_COMMAND 'D'
#define QT_COMMANDS 2


#include <time_manager.h>
#include <functions.h>
#include <tasks.h>


static unsigned int8 valid_commands[QT_COMMANDS] = {
   UART_BLOQ_COMMAND, 
   UART_DESBLOQ_COMMAND
};

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

unsigned char actual_data = '-';
unsigned char status_motor = OPENED, command_motor = OPENED;


#pragma INT_RDA
void RDA_isr(void) {  
   actual_data = getch();
}


struct flags {
   int1 power:1; //alimentacao
   int1 uart:1;  //comandos de bloq e desbloq
   int1 com_time:1; //tempo sem comunicacao
   int1 endline:1; //mudança no fim de curso
}flagsControl;


unsigned int8 getFlags(void) {
   unsigned int8 flg; //== NAO MEXER, TA FUNCIONANDO ==
   
   if (flagsControl.power)
   return flg;
}


//=======================================================================
//Retorna 1 se mudou de 0 pra 1+, ou de 1+ pra 0; retorna 0 se nao mudou
//=======================================================================
unsigned int1 verifyFlags (void) {
   static unsigned int8 last_flag = 0;
   unsigned int8 flg;
   
   flg = getFlags();

   //se mudou de 0 pra >0, ou de >0 pra 0
   if (last_flag ^ flg) {
      last_flag = flg;
      return 1;
   }
   else return 0;
}

/* 
unsigned int1 isValidCommand (unsigned char command) {
   unsigned int8 i;

   for (i = 0; i < QT_COMMANDS; i++) {
      if (valid_commands[i] == command) {
         return 1;
      }
   }
   return 0;
}
 */

#define NEW_DATA 0
#define GET_LAST 1
char getCommand (unsigned int8 get) {
   static unsigned char last_data = 0;
   
   if ((actual_data == UART_BLOQ_COMMAND) || 
      (actual_data == UART_DESBLOQ_COMMAND)) {

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


//=======================================================================
//bloqueia quando atinge tempo max sem comunicao
//=======================================================================
int1 checkTimeMessage (void) {
   static unsigned int8 time = 0;
   unsigned int8 command; 
   command = getCommand(NEW_DATA);
   
   //se recebeu um comando valido
   if (command) {
      flagsControl.com_time = FALSE;
      return TRUE;
   }
   //se atingiu o tempo maximo sem comunicacao 
   else if (time >= MAX_WDT_UART_TIME) {
      flagsControl.com_time = TRUE;
      return FALSE;
   }
   //conta tempo sem comunicacao
   time++;
}


//=======================================================================
//recebe caracteres UART e executa comandos
//=======================================================================
int1 checkCommandsUart (void) {
   unsigned char cmd; 
 
   cmd = getCommand(GET_LAST);

   if (cmd == UART_BLOQ_COMMAND) {
      flagsControl.uart = TRUE;
   }

   else if (cmd == UART_DESBLOQ_COMMAND) {
      flagsControl.uart = FALSE;
   }

   return TRUE;
}


//=======================================================================
//verifica alimentacao externa do dispositivo
//=======================================================================
int1 checkPowerIn (void) {   
   
   if(input(POWER_IN)) {
      flagsControl.power = FALSE;
      //output_high(LED2);
   }
   else {
      flagsControl.power = TRUE;
      //output_low(LED2);
   }
   return TRUE;
}


unsigned int1 turnValve (void) { 
   
   if (command_motor == CLOSED) {
      output_low(MOTOR2);
      delay_ms(100);
      output_high(MOTOR1);

      output_high(LED1);
   }

   else if (command_motor == OPENED) {
      output_low(MOTOR1);
      delay_ms(100);
      output_high(MOTOR2);

      output_low(LED2);
   }

   controlValve.active = FALSE;
   return TRUE;
}


//=======================================================================
//retorna 0 se motor esta parado, 1 se es esta abrindo, 2 se esta fechando
//=======================================================================
unsigned char checkMotorPosition (void) {
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

int1 waitEndMotor (void) {
   unsigned int8 position_motor;
   unsigned int8 has_movement = FALSE;
   
   position_motor = checkMotorPosition();

   if(!position_motor) {
      //aguardando inicio
      
      if (has_movement) {
         //fim do ciclo
         endMotor.active = FALSE;  
         status_motor = has_movement;
         return TRUE;
      }
   }

   else {
      has_movement = position_motor;//abrindo
   }

   return FALSE;
}

//=======================================================================
//verifica qual flag mudou, ajusta tempo de espera do motor, e ativa coltrolValve
//ativo quando alguma flag muda, entra apenas uma vez 
//=======================================================================
int1 statusMotor (void) {

   //se valvula esta parada
   if (!controlValve.active) {
      if (flagsControl.com_time) {
         controlValve.sec = TIME_BEFORE_BLOQ;
         controlValve.count_sec = 0;
         command_motor = CLOSED;
      }
      
      else if (flagsControl.power) {
         controlValve.sec = TIME_BEFORE_BLOQ;
         controlValve.count_sec = 0;
         command_motor = CLOSED;
      }

      else if (flagsControl.uart) {
         controlValve.sec = 0;
         controlValve.count_sec = 0;
         command_motor = CLOSED;
      }

      //desbloqueio
      else {
         controlValve.sec = 0;
         controlValve.count_sec = 0;
         command_motor = OPENED;
      }
      
      //se motor ja esta na posicao
      if (status_motor == command_motor) {
         controlValve.active = FALSE;
      }

      else {
         controlValve.active = TRUE;
      }    
   }

   controlMotor.active = FALSE;
   return FALSE;
}


int1 statusFlags (void) {

   //houve mudanca de flags
   if (verifyFlags()) {
      controlMotor.active = TRUE;
   }

   //motor iniciou movimento
   if (controlValve.flag) {
      controlValve.flag = FALSE;
      endMotor.active = TRUE;
   }

   //fim do movimento
   if (endMotor.flag) {
      endMotor.flag = FALSE;
      
   }
    return FALSE;
}

void abc(){}
void cde (){}

void main (void) {
   flagsControl.power = FALSE;
   flagsControl.uart = FALSE;
   flagsControl.com_time = FALSE;

   controlValve.sec = 0x00;
   controlValve.count_sec = 0x00;
   controlValve.active = FALSE;
   controlValve.flag = FALSE;
   controlValve.func = turnValve;
  
   endMotor.sec = 0x00;
   endMotor.count_sec = 0x00;
   endMotor.active = FALSE;
   endMotor.flag = FALSE;
   endMotor.func = waitEndMotor;

   controlMotor.sec = 0x00;
   controlMotor.count_sec = 0x00;
   controlMotor.active = FALSE;
   controlMotor.flag = FALSE;
   controlMotor.func = statusMotor; 

/*    
 */

   struct taskFunc timeReceive;
   timeReceive.sec = 0x00;
   timeReceive.count_sec = 0x00;
   timeReceive.active = TRUE;
   timeReceive.flag = FALSE;
   timeReceive.func = abc;

   struct taskFunc controlFlag;
   controlFlag.sec = 0x00;
   controlFlag.count_sec = 0x00;
   controlFlag.active = TRUE;
   controlFlag.flag = FALSE;
   controlFlag.func = statusFlags; 

   struct taskFunc uart;
   uart.sec = 0x00;
   uart.count_sec = 0x00;
   uart.active = TRUE;
   uart.flag = FALSE;
   uart.func = checkCommandsUart; 

   struct taskFunc powerIn;
   powerIn.sec = 0x00;
   powerIn.count_sec = 0x00;
   powerIn.active = TRUE;
   powerIn.flag = FALSE;
   powerIn.func = checkPowerIn; 
/*    
   */

   addTask (&uart);
   addTask (&timeReceive);
   addTask (&powerIn);
   //addTask (&controlValve);
   //addTask (&endMotor);
   //addTask (&controlMotor); 
   //addTask (&controlFlag);

  
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

   for(unsigned int8 i = 0; i < 6; i++) {
      output_toggle(LED1);
      output_toggle(LED2);
      delay_ms(150);
   }
   

   while (TRUE) {      
      

      if(um_segundo) {
         um_segundo = 0b0;
         printf("flags: %u%u%u\r\n", flagsControl.power, 
                                    flagsControl.uart, 
                                    flagsControl.com_time);
         
         statusMotor();
         runTasks();
         
 
         
         output_toggle(LED1);
      }
   }
}
