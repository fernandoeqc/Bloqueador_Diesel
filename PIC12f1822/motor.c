#include <motor.h>
#define TIMER0_F

#define FIM_CURSO_1 PIN_A5
#define FIM_CURSO_2 PIN_A4
#define MOTOR1      PIN_A3
#define MOTOR2      PIN_A2
#define LED_STATUS  PIN_A1                 
#define LED         PIN_A0

#define ERROR 0
#define OPENED 1
#define CLOSED 2
#define TRANSITION 3
#include <functions.h>

void fim_curso(int1 status)
{
   unsigned int sec_t = sec, count = 0;

   
   if (status == CLOSED) {
      output_low(FIM_CURSO_1);
      output_high(FIM_CURSO_2);
   }
   else {
      output_low(FIM_CURSO_2);
      output_high(FIM_CURSO_1);
   }

   while (count < 5)
   {
      if (sec != sec_t)
      {
         sec_t = sec;
         count++;
      }
   }

   output_low(FIM_CURSO_2);
   output_low(FIM_CURSO_1);

   output_low(LED);
   output_low(LED_STATUS);
}

void main()
{
   unsigned int8 status_bloqueio = OPENED, status_bloqueio_last = OPENED;

   unsigned int8 i = read_eeprom(0x00);
   if (i != 0xFF) status_bloqueio_last = i;

   status_bloqueio = status_bloqueio_last;

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

   output_low(FIM_CURSO_1);
   output_low(FIM_CURSO_2);

   output_low(LED);
   output_low(LED_STATUS);

   delay_ms(1000);

   while (TRUE)
   {
      if (input(MOTOR1) && !input(MOTOR2))
      {
         output_high(LED_STATUS);
         status_bloqueio = CLOSED;
      }

      if (!input(MOTOR1) && input(MOTOR2))
      {
         output_low(LED_STATUS);
         status_bloqueio = OPENED;
      }

      if (status_bloqueio != status_bloqueio_last) {
         status_bloqueio_last = status_bloqueio;

         write_eeprom (0x00, status_bloqueio_last);
         
         fim_curso(status_bloqueio);
      }

      if (um_segundo)
      {
         um_segundo = 0;
         output_toggle(LED);
      }
   }
}
