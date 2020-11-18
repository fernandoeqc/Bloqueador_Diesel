#include <testes_mecanico.h>

#USE RS232 (baud=9600, parity=N, xmit=PIN_A0, rcv=PIN_A1, bits=8, stream=COM_A)
#USE RS232 (baud=9600, parity=N, xmit=PIN_C4, rcv=PIN_C5, bits=8, stream=COM_B)

char uart1 = 0, uart2 = 0;

#INT_RDA
void  RDA_isr(void) 
{
   
}



void main(void) {
   char c;
   
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   
   printf(COM_A, "Online\n\r");
   printf(COM_B, "Online\n\r");
   
   while(TRUE) {
      uart1 = getc(COM_A);  
      uart2 = getc(COM_B);
   
      putc(uart2, COM_A);
      putc(uart1, COM_B);
   }
}
