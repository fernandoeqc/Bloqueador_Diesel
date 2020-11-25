#include <testes_mecanico.h>

#define LED_B 0x1E
#define LED_D 0x1D
#define LED_R
#define LED_0

#byte MCU_LATC = 0x10E

void output_leds (unsigned int8 bin) {
   output_bit(PIN_A2, bit_test(bin,0)); 
   output_bit(PIN_C0, bit_test(bin,1)); 
   output_bit(PIN_C1, bit_test(bin,2)); 
   output_bit(PIN_C2, bit_test(bin,3)); 
   output_bit(PIN_C3, bit_test(bin,4)); 
}

void main(void) {
   unsigned int8 x = 0;
   while(TRUE) {
      output_leds(x);
      x++;
      delay_ms(500);
      output_toggle(PIN_A0);
   }
}