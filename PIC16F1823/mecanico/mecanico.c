#define DEBUG

#include   <mecanico.h>

#define COMANDO_BLOQUEIO       'B'
#define COMANDO_DESBLOQUEIO    'D'
#define COMANDO_RESTART        'R'

#define BLOCK_TIMER            10

#define LIMITE_BATERIA         220 // bateria com 8.7v 
#define LIMITE_ALIMENTACAO     90  // power com 11v
//constante da funcao piscaLedStatus()
//1 TEMPOCICLOLEDS == 200mS na configuracao atual
#define TEMPOCICLOLEDS 30
#include <functions.h>



unsigned int data = 'D', last_data = 0;

//precisa iniciar com valores diferentes 
unsigned int8 control_flags = 0b111, 
              last_control_flags = 0b000,
              count_timer = 0; 

#ifdef DEBUG
char *err_message = "err_message", //ascii 'ERR' quando pic recebe mensagem fora do intervalo 0 - 7
     *err_movement = "err_movement";
#endif


#pragma INT_RDA
void RDA_isr(void) {  
   data = getch();
}


struct adc {
   unsigned int8 bateria;
   unsigned int8 alimentacao;
};


void leAdc (struct adc *leitura_adc) {
   static unsigned int8 control_adc = 0x01;
   struct adc adc_copy;
   adc_copy = *leitura_adc;

   if (control_adc) {
      control_adc = 0x00;
      adc_copy.alimentacao = read_adc();
      delay_us(20);
      set_adc_channel(ADC_BAT);
   }
   else {
      control_adc = 0x01;
      adc_copy.bateria = read_adc();
      delay_us(20);
      set_adc_channel(ADC_ALIM);
   }

   *leitura_adc = adc_copy;
}


//======================================================
// Transforma valores de leitura adc em estados binarios
// recebe: 
//    struct adc com dois valores unsigned int8
// retorna:
//    unsigned int8 com dois últimos bitd válidos
//    ADD.:David
//======================================================
unsigned int8 trataAdc (struct adc dados) {
  unsigned int8 control = 0b0000;

  if (dados.bateria >= LIMITE_BATERIA && dados.alimentacao >= LIMITE_ALIMENTACAO) {
    control = 0b0011;

  } if (dados.bateria < LIMITE_BATERIA && dados.alimentacao >= LIMITE_ALIMENTACAO) {
    control = 0b0010;

  } if (dados.bateria >= LIMITE_BATERIA && dados.alimentacao < LIMITE_ALIMENTACAO) {
    control = 0b0001;

  } if (dados.bateria < LIMITE_BATERIA && dados.alimentacao < LIMITE_ALIMENTACAO) {
    control = 0b0000;
  }

  return control;
}


void seriaTimelMonitor(void) {
   count_timer++;
   if(count_timer > BLOCK_TIMER){
      count_timer = 0;
      //bloqueia no proximo segundo
      data = 'B';
   }
}


unsigned char trataUart(void) {
   static unsigned int8 retorno = 0b100;
   last_data = data;

   if(data == COMANDO_BLOQUEIO) {
      output_high(LED2);
      retorno = 0;
   }
   
   else if(data == COMANDO_DESBLOQUEIO) {
      output_low(LED2);
      count_timer = 0;
      retorno = 0b100;
   }

   else if (data == COMANDO_RESTART) {
      reset_cpu();
   }
  
   else{
      //comando desconhecido
      //atualmente bloqueia imediato se receber
      //comando desconhecido
   }
   output_toggle(LED1);

   return retorno;
}


unsigned char trataUartTeste(unsigned char data_t) {
   unsigned int8 retorno;

   retorno = data_t - 48;
   if(data_t != last_data) {
      if(retorno > 7){
         printf("err_message\n");
         data = last_data;
         retorno = last_data - 48;
         return retorno;
      }   
      last_data = data_t;
   }
   return retorno;
}


void turnBattery(unsigned int8 command){
   output_bit(CARGA_BAT,command);
}


void blockMotor(unsigned int8 command) {
   static unsigned int8 status_motor = 0;
   unsigned int1 fim_curso = 0;
   
   #ifdef DEBUG
   if(input(FIM_CURSO_IN)) {
      puts("err_movement\r\n");
      //erro: motor em transicao fora do tempo DEBUG
      return;
   }
   #endif

   if(command == status_motor) {
      #ifdef DEBUG
         printf("cmd = status_motor\r\n");
      #endif
      return;
   }
   else
   {
      turnBattery(TRUE);

      status_motor = command;
      if(command == TRUE) {
         #ifdef DEBUG
            printf("motor bloq\r\n");
         #endif
         output_low(MOTOR1);
         delay_ms(500);
         output_high(MOTOR2);
      }
      else {
         #ifdef DEBUG
            printf("motor desbloq\r\n");
         #endif
         output_low(MOTOR2);
         delay_ms(500);
         output_high(MOTOR1);
      }

      do {
         #ifdef DEBUG
            printf("_\r\n");
         #endif
         
         delay_ms(1000);

         //tempo de espera iniciar transicao
         while(input(FIM_CURSO_IN)) {
            #ifdef DEBUG
               printf("/\r\n");
            #endif
            
            //tempo de transicao do motor
            delay_ms(1000);
            fim_curso = 1;
         }
         
      }while(!fim_curso);

      #ifdef DEBUG
         printf("|\r\n");
      #endif

      fim_curso = 0;
   }
}


void controlState(void) {
   
   if(control_flags != last_control_flags){
      #ifdef DEBUG
         printf("alt: %u\r\n", last_control_flags ^ control_flags);
      #endif
      last_control_flags = control_flags;

      if(control_flags > 7){
         //ERRO! tamanho max. 3 bits
         eeprom_grava(EP_CONTROL_FLAGS,control_flags);
      }

      if(control_flags == 0b111) {
         blockMotor(FALSE);
         turnBattery(FALSE);
      }

      else if(control_flags == 0b110) {
         blockMotor(FALSE);
         turnBattery(TRUE);
      }
      
      //bloqueia mecanico
      else{
         blockMotor(TRUE);
         turnBattery(TRUE);
      }

      //desliga bateria quando carga esta completa
      if(bit_test(control_flags,0)){
         turnBattery(FALSE);
      }
   }
}


void main() {
   unsigned int8 adc1 = 0,           //batteria
                 adc2 = 0,           //alimentacao
                 ctrl_adc = 0,
                 ctrl_uart = 0,      //bits de controle de dados
                 ctrl_fim_curso = 0; 
                 
   unsigned char command = 0;        //dados de comando pelo UART

  
   //flags de controle de fim de curso, dados, tensao veiculo, tensao bateria
      

   //setup_adc_ports(sAN2 | sAN4); //nao e necessario quando se usa set_adc_channel()
   setup_adc(ADC_CLOCK_DIV_32);

   //===========REGISTRADORES===================================
   disable_interrupts(GLOBAL);               // habilitar interr global
   enable_interrupts(INT_RDA);               //UART
   //enable_interrupts(INT_EXT_H2L);         // interrupcao CAN
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // setar timer1 para interno
   enable_interrupts(INT_TIMER1);            // habilita Timer1
   set_timer1(0);                            // limpar flag TMR1H & TMR1L
   counter = int_per_sec;
   enable_interrupts(GLOBAL);                // habilitar interr global
   //----------------------------------------------------------

   set_adc_channel(ADC_ALIM);
   struct adc leitura_adc;
   leitura_adc.alimentacao = 255;
   leitura_adc.bateria = 255;

   output_low(LED1);
   output_low(LED2);

   //DEABLOQUADO INICIALMENTE
   output_low(MOTOR2);
   output_toggle(LED2);
   delay_ms(500);
   output_toggle(LED2);
   delay_ms(500);

   output_high(MOTOR1);
   output_toggle(LED2);
   delay_ms(500);
   output_toggle(LED2);
   delay_ms(500);

   while (TRUE)
   {
      if (um_segundo) {
         um_segundo = 0;

         leAdc(&leitura_adc);

         ctrl_adc = trataAdc(leitura_adc); 
         ctrl_uart = trataUart();
         control_flags = ctrl_adc + ctrl_uart;

         printf("%u, %u, %c: %u; \r\n",leitura_adc.alimentacao, leitura_adc.bateria, data, control_flags);
         
         output_toggle(LED1);
         controlState();

         seriaTimelMonitor();
      }
   }
}
