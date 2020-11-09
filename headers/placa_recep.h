//********************************
//*   PINOS                      *
//********************************

#error ************ conferir pinos da nova placa ************

#define AVISO         PIN_A1 //O
#define IRQ           PIN_A2 //I
#define BLOQ          PIN_A3 //I
#define LED2          PIN_A4 //O

// PORTC
#define SCK           PIN_C0 //O
#define SDI           PIN_C1 //I
#define SDO           PIN_C2 //O
#define CS            PIN_C3 //O
#define LED1          PIN_C5 //O

//TRISA NAO ESTA SENDO USADO. CCS SE ENCARREGA DE ATUALIZAR OS IOs        
//TRISA
#define CE_TRIS       TRISA,1 //O
#define IRQ_TRIS      TRISA,2 //I

//TRISC
#define SCK_TRIS      TRISC,0 //O
#define SDI_TRIS      TRISC,1 //I
#define SDO_TRIS      TRISC,2 //O
#define CS_TRIS       TRISC,3 //O

//*****************
//*   VARIABLES   *
//*****************
#BYTE TRISA     =  0x0C //00000110
//#BYTE TRISB     =  0x86
#BYTE TRISC     =  0x02 //00000010
#BYTE INTCON    =  0x00


//********************************
//*   EEPROM                     *
//********************************
//DEFINE EEPROM_NOME PRIMEIRO ENDERECO,TAMANHO BYTES

//conta transmissoes perdidas na CAN - bytes 0 a 3
#ROM 0xF008 = {0x00,0x00,0x00,0x0b}
#define EP_NAO_TRANS 0x00,0x04

//conta quantas horas ligado desde o ultimo reset - bytes 4 a 5
#ROM 0xF00C = {0x00,0x0d}
#define EP_HORA_LIGADO 0x04,0x02

//conta erros de comunicacao com MCP - bytes 6 a 9
#ROM 0xF00E = {0x00,0x00,0x00,0x11}
#define EP_MCP 0x06,0x04

//SELETOR DE FREQUENCIA - byte 0A
#ROM 0xF012 = {0x12}
#define EP_ID 0x0A,0x01

#define int_per_sec 2

//********************************

volatile unsigned int32 erro_nao_trans = 0,
                        erro_mcp = 0;

volatile unsigned int16 horas_ligado = 0;

volatile unsigned int counter = 0,
                       sec = 0,
                       min = 0;
                                              
volatile unsigned int1 um_segundo = 0b0, 
                       um_minuto = 0b0,
                       uma_hora = 0b0,
                       flag_interr = 0b0,
                       flag_envio = 0b0,
                       flag_receb = 0b0,
                       erro_nao_trans_flag = 0,
                       erro_mcp_flag = 0;
  
void external_can_interrupt ();
int detecta_freq(void);
void setup_can(void);
void piscaLed(char nPisca, int16 delay, int led);
void eeprom_grava(unsigned int address, unsigned int tamanho, unsigned int32 hexa);
unsigned int32 eeprom_le(unsigned int address, unsigned int tamanho);


#INT_TIMER1
void timer1_isr(){  // interrupt routine    
   set_timer1(53036);
   counter--;  // decrements counter which is set to it_per_sec 

   //MEIO SEGUNDO
   
   //SEGUNDOS
   if(counter==0){         
      sec++;                
      counter=int_per_sec; //resets counter
      //contador_seg++;
      um_segundo = 0b01;
   }

   //MINUTOS
   if(sec==60){ 
      sec=0;       
      min++;
      um_minuto = 0b1;
   }

   //HORAS
   if(min==60) { 
      min=0;
      //atualizar dados da eeprom aqui
   }
}

#int_ext
void external_can_interrupt ()
{
   flag_interr = 0b1;
}


void piscaLed(char nPisca, unsigned int16 delay, unsigned int led)
{
     char i = 0;
     
     for(i = 0; i < nPisca; i++){
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
