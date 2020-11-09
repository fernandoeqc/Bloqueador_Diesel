#include <devboard.h>


void setup_lcd(void)
{
   lcd_init();
   lcd_cursor_on(1);
   lcd_putc("\fReady...\n"); delay_ms(500);
   lcd_send_byte(0,0x01);
}

void setup_interrupt(void)
{
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32);      //1,0 s overflow
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
}

void setup_io(void)
{
   //VEJA placa_plus.h
   set_tris_a(0b00001100);
   set_tris_b(0b00010001);
   set_tris_c(0b00010000);
   set_tris_d(0b00000000);
   
   output_high(LED1);
   delay_ms(200);
   output_low(LED1);
   delay_ms(200);
   output_high(LED1);
   delay_ms(200);
   output_low(LED1);
   delay_ms(200);
   
   output_low(LED1);
   output_low(LED2);
}


#ifdef TECLADO_ON
const char map_key[3][5] = {"<741", "0852", ">963"};
char key = 0,
     prev_key = 0;
    

char teclado()
{
   int i = 0,
        j = 0,
        portd = 0;
   
   //faz a leitura inicial do teclado
   output_low(pin_b0);
   output_low(pin_b1);
   output_low(pin_b2);
   portd = input_d();
   output_high(pin_b0);
   output_high(pin_b1);
   output_high(pin_b2);
   
   //se alguma tecla for pressionada:
   if(portd != 0x0f)
   {
      for(i = 0; i < 3; i++)
      {
         output_low(pin_b0 + i);
         portd = input_d();
         for(j = 0; j < 4; j++)
         {
            if(!bit_test(portd, j))
            {
               key = map_key[i][j];
               //previne a releiturada tecla repetidamente
               if(key != prev_key)
               {
                  prev_key = key;
                  return key; 
               }
            }
         }
         output_high(pin_b0 + i);
      }   
   }
   else {prev_key = 0;}
   return 0;
}
#endif

void piscaLed(char nPisca, unsigned int16 delay, unsigned int led)
{
     char i = 0;
     
     for(i = 0; i < 5; i++){
         output_high(led);
         delay_ms(20);
         output_low(led);
         delay_ms(delay);
     }
}

void eeprom_grava(unsigned int address, unsigned int tamanho, unsigned int32 hexa)
{
   unsigned int i = 0, bytes[4];               
   
   //separa bytes
   for (i = 0; i < tamanho; i++)
   {
     bytes[i] = hexa;
     hexa >>= 8;
   }
     
   //imprime na ordem inversa
   while(tamanho) 
   {
      write_eeprom(address++,bytes[--tamanho]);
      delay_ms(1);
   }
}

unsigned int32 eeprom_le(unsigned int address, unsigned int tamanho)
{
   unsigned int i = 0;
   unsigned int32 bytes = 0;
   
   for(i = tamanho; i > 0; i--)
   {
      bytes <<= 8;
      bytes |= read_eeprom(address++); 
      delay_ms(1);
   }
   return bytes;
}

