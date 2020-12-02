#define TEMPOCICLOLEDS 30

//valor de seguranca para tempo sem comunicacao uart
//antes de ativar respectiva flag:
#define MAX_TIME_WAITING_UART 5//30   /// valor max: 255

//valor de seguranca para tempo sem alimentacao
//antes de ativar respectiva flag:
#define MAX_TIME_WAITING_POWER 5//10   /// valor max: 255

//tempo de delay do motor antes da transicao
#define TIME_BEFORE_BLOQ 5//30

//tempo de bateria carregando
#define TIME_BATTERY 3600

#if TIME_BEFORE_BLOQ + MAX_TIME_WAITING_POWER > 60
   #error tempo deve levar em conta descarga da bateria.
#endif

//tempo maximo em transicao do motor: refator - verificar condicoes
#define MAX_TRANSITION_TIME 20 ///valor max: 255


#define UART_BLOQ_COMMAND 'B'
#define UART_DESBLOQ_COMMAND 'D'
#define UART_RESTART_COMMAND 'R'
#define QT_COMMANDS 3


#define DEBUG 

#include <time_manager.h>
//#include <stddef.h>
#include <functions.h>
#include <tasks.h>

unsigned int8 valid_commands[QT_COMMANDS] = {
   UART_BLOQ_COMMAND, 
   UART_DESBLOQ_COMMAND, 
   UART_RESTART_COMMAND
};

char actual_data = '-';
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

#ifdef DEBUG
   unsigned char msgs[4] = {'E', 'A', 'F', 'T'};
#endif

unsigned int8 bloq_state = CLOSED, bloq_command = OPENED;

/* #INT_RDA
void RDA_isr(void) {  
    
   if(kbhit()) {
      actual_data = getc(); 
      clear_interrupt(INT_RDA);
   }
}
 */
void readUart (void) {
   if(kbhit()) {
      actual_data = getc(); 
      //clear_interrupt(INT_RDA);
   }
}

struct flags {
   int1 power:1; //alimentacao
   int1 uart:1;  //comandos de bloq e desbloq
   int1 com_time:1; //tempo sem comunicacao
   int1 restart:1;
}flagsControl;


unsigned int8 getFlags() {
   unsigned int8 flg = flagsControl; //== NAO MEXER, TA FUNCIONANDO ==
   return flg;
}

//=======================================================================
//Retorna 1 se mudou de 0 pra 1+, ou de 1+ pra 0; retorna 0 se nao mudou
//Sempre vai retornar 1 no inicio do programa
//=======================================================================
unsigned int8 last_flag = 0xFF; 
unsigned int1 verifyFlags () {
   unsigned int8 flg; 
   
   flg = getFlags();

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

#pragma byte MCU_LATA = 0x10C
#pragma bit    M1_LATA = MCU_LATA.4
#pragma bit    M2_LATA = MCU_LATA.5

void ativaMotor() { 

   if (bloq_command == POSIX_CLOSING) {
      output_low (MOTOR2);
      delay_ms(500);
      output_high (MOTOR1);
      delay_ms(500);

      output_high(LED2);
   }

   //ABRE
   else {
      output_low (MOTOR1);
      delay_ms(500);
      output_high (MOTOR2);
      delay_ms(500);

      output_low(LED2);
   }
   bloq_state = TRANSITION;
} 

/* void ativaMotor(struct taskData *tk) { 

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
}  */


enum {NEW_DATA, GET_LAST};
unsigned char last_data = 0;
char getCommand(unsigned int8 get) {
   
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


unsigned char receive = 0;
unsigned int8 state = 0;
unsigned int8 count_state = 0;
void checkTimeMessage() {

   receive = getCommand(NEW_DATA);
   
   //se recebeu um comando valido
   if (receive) {
      state = 0;
      flagsControl.com_time = FALSE;
      return;
   }
   //se atingiu o tempo maximo sem comunicacao 
   else if (state >= MAX_TIME_WAITING_UART) {
      flagsControl.com_time = TRUE;
      return;
   }
   //conta sem comunicacao
   state++;
}


void checkCommandsUart() {
   
   com_uart = getCommand(GET_LAST);

   if (com_uart == UART_BLOQ_COMMAND) {
      flagsControl.uart = TRUE;
      //bit_set (com, 2);
      //uart.command == com_uart;
   }

   else if (com_uart == UART_DESBLOQ_COMMAND) {
      flagsControl.uart = FALSE;
      //bit_clear (com, 2);
      //uart.command == com_uart;
   }

   else if (com_uart == UART_RESTART_COMMAND) {
      //flagsControl.restart = TRUE;
   }
}



void checkPowerIn() {   
   if(input(POWER_IN)) {
      flagsControl.power = FALSE;
      count_state = 0;
      return;
      //output_high(LED2);
   }
   else if (count_state >= MAX_TIME_WAITING_POWER){
      flagsControl.power = TRUE;
      return;
      //output_low(LED2);
   }
   count_state++;
}


void saveStatusEeprom (void) {
      write_eeprom(0x01,bloq_command);
      delay_ms(100);

      write_eeprom(0x00, getFlags());
      delay_ms(100);
}

//POR ALGUM MOTIVO OBSCURO
//A VAR time_to_charge precisa ser global,
//senao ao e reconhecida pelo proteus
unsigned int16 time_to_charge = 0;
void chargeBat (void) 
{ 

   if (time_to_charge < TIME_BATTERY)
   {
      output_high (CARGA_BAT);  
      time_to_charge++;
   } 
   else
   {
      output_low (CARGA_BAT);
      
      time_to_charge = 0;
      battery.active = FALSE;
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

void toggleOpenClose (void) {
   
   if (flagsControl.restart){ 
      flagsControl.restart = FALSE;

      reset_cpu ();
   }

   if (flagsControl.com_time) {
      contaBloq.sec = TIME_BEFORE_BLOQ;
      contaBloq.count_sec = 0;
      bloq_command = POSIX_CLOSING;

      #ifdef DEBUG
         printf("\ntempo s/ comunicacao\r\n");
      #endif 
   }
   
   else if (flagsControl.power) {
      contaBloq.sec = TIME_BEFORE_BLOQ;
      contaBloq.count_sec = 0;
      bloq_command = POSIX_CLOSING;

      #ifdef DEBUG
         printf("\nsem aliment.\r\n");
      #endif 
   }

   else if (flagsControl.uart) {
      contaBloq.sec = 0;
      contaBloq.count_sec = 0;
      bloq_command = POSIX_CLOSING;

      #ifdef DEBUG
         printf("\ncomando bloq.\r\n");
      #endif 
   }

   //desbloqueio
   else {
      contaBloq.sec = 0;
      contaBloq.count_sec = 0;
      bloq_command = POSIX_OPENING;

      #ifdef DEBUG
         printf("\ncomando desbloq.\r\n");
      #endif 
   }
   
   if(bloq_state == bloq_command) {
      contaBloq.active = FALSE;
   }
   else {
      contaBloq.active = TRUE;
   }

   //return tk;
}


unsigned int8  count_err_trans = 0;
unsigned int8 flg_transition = 0, position_motor = 0;
void statusMotor (void) {

   position_motor = checkMotorPosition();
   
   if (
         ((bloq_state == OPENED) || (bloq_state == CLOSED)) && 
         (verifyFlags())
      ) {
      
      toggleOpenClose();
      
   }

   else if (bloq_state == TRANSITION) {

      if (position_motor == POSIX_PARKED) {
         if(!flg_transition) {
            //motor ja está na posição ====== fazer testes
            bloq_state = bloq_command;
            contaBloq.active = FALSE;
            flg_transition = 0;

            #ifdef DEBUG
               printf("<<<M ja esta na posicao>>> |\r\n");
            #endif
         }
         else {
            //fim da transicao
            bloq_state = flg_transition;
            contaBloq.active = FALSE;
            flg_transition = 0;
            
            battery.active = TRUE;
            count_err_trans = 0;
            
            saveStatusEeprom();

            #ifdef DEBUG
               printf("M fim _\r\n");
            #endif
         }

         return;
      }
      
      else if ((position_motor == bloq_command)) {
         //motor esta abrindo
         flg_transition = position_motor;
         contaBloq.active = FALSE;

         #ifdef DEBUG
            printf("M comando: %c\r\n", msgs[position_motor]);
         #endif
         return;

      }

      else {
         count_err_trans++;
         if (count_err_trans < MAX_TRANSITION_TIME) {
            #ifdef DEBUG
               printf("<<<ERRO! M : %c>>>\r\n", msgs[position_motor]);
            #endif            
         }

         else {
            #ifdef DEBUG
               printf("error_max_timeout\r\n");
            #endif
            count_err_trans = 0;
            reset_cpu();
         }
      }
   }
   
   //se nao entrou em nenhum, erro!
   //else if (bloq_state == ERROR) {}
}


int main (void) {


   setup_comparator(NC_NC_NC_NC);
   disable_interrupts(PERIPH);

   //selecao dos pinos rx e tx a0 / a1
   TXSEL = 1;
   RXSEL = 1;

   //===========REGISTRADORES===================================
   disable_interrupts(GLOBAL);               // habilitar interr global
   //enable_interrupts(INT_RDA);               //UART
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // setar timer1 para interno
   enable_interrupts(INT_TIMER1);            // habilita Timer1
   set_timer1(0);                            // limpar flag TMR1H & TMR1L
   counter = int_per_sec;
   enable_interrupts(GLOBAL);                // habilitar interr global
   //----------------------------------------------------------

   flagsControl.power = FALSE;
   flagsControl.uart = FALSE;
   flagsControl.com_time = FALSE;

   contaBloq.sec = 0x00;
   contaBloq.count_sec = 0x00;
   contaBloq.active = FALSE;
   contaBloq.func_time = ativaMotor;

   timeReceive.sec = 0x00;
   timeReceive.count_sec = 0x00;
   timeReceive.active = TRUE;
   timeReceive.func_time = checkTimeMessage;

   uart.sec = 0x00;
   uart.count_sec = 0x00;
   uart.active = TRUE;
   uart.func_time = checkCommandsUart; 


   powerIn.sec = 0x00;
   powerIn.count_sec = 0x00;
   powerIn.active = TRUE;
   powerIn.func_time = checkPowerIn; 
   
   battery.sec = 0x00;
   battery.count_sec = 0x00;
   battery.active = TRUE;
   battery.func_time = chargeBat; 
   

   addTask (&uart);
   addTask (&timeReceive);
   addTask (&powerIn);
   addTask (&battery);
   addTask (&contaBloq);
 
   unsigned int8 i;
   //i = read_eeprom(0x01);
   //if (i != 0xFF) bloq_state = i;
   
   i = read_eeprom(0x00);
   if (i != 0xFF) flagsControl = i;

   //ativaMotor(&(contaBloq));

   //ativaMotor(&(contaBloq));
   //verifyFlags(flagsControl);

   output_low (LED1);
   output_low (LED2);

   #ifdef DEBUG
      printf("\r\n...INICIO...\r\n");
   #endif
   
   for(i = 0; i < 6; i++) {
      output_toggle(LED1);
      output_toggle(LED2);
      delay_ms(150);
   }
   
   
   #ifdef DEBUG
      printf("\nLoop\r\n");
   #endif


   while (TRUE) {      
      
      //0.5 seg
      if (um_periodo) {
         readUart();
      }

      if(um_segundo) {
         um_segundo = 0b0;
         
         #ifdef DEBUG
         printf("flags: %u%u%u, ", flagsControl.power, 
                                    flagsControl.uart, 
                                    flagsControl.com_time);
         
         printf("cmd %c;\r\n", actual_data);

         #endif

         statusMotor();
         runTasks(); 

         output_toggle(LED1);   
      }
   }
}
