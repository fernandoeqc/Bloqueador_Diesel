//********************************
//*   PINOS                      *
//********************************
//DEFINE ORDENACAO E FUNCAO DE PINOS 
//int lede1;
// PORTA
#define AVISO         PIN_A1 //O
#define IRQ           PIN_A2 //I
#define BOTAO         PIN_A3 //I
#define LED1          PIN_A4 //O

///#pragma word lede1 = 0x10C
///#pragma bit LED1_IN = lede1.4

// PORTC
#define SCK           PIN_C0 //O
#define SDI           PIN_C1 //I
#define SDO           PIN_C2 //O
#define CS            PIN_C3 //O
#define LED2          PIN_C5 //O


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
#pragma byte TRISA     =  0x0C //00000110
//#BYTE TRISB     =  0x86
#pragma byte TRISC     =  0x02 //00000010
#pragma byte INTCON    =  0x00

//********************************
//*   EEPROM                     *
//********************************
//DEFINE EEPROM_NOME : PRIMEIRO ENDERECO,TAMANHO BYTES

//conta transmissoes perdidas na CAN - bytes 0 a 3
#pragma ROM 0xF008 = {0x00,0x00,0x00,0x00}
#define EP_NAO_TRANS 0x00,0x04

//conta quantas horas ligado desde o ultimo reset - bytes 4 a 5
#pragma ROM 0xF00C = {0x00,0x00}
#define EP_HORA_LIGADO 0x04,0x02

//conta erros de comunicacao com MCP - bytes 6 a 9
#pragma ROM 0xF00E = {0x00,0x00,0x00,0x0}
#define EP_MCP 0x06,0x04

//SELETOR DE FREQUENCIA - byte 0A
#pragma ROM 0xF012 = {0x00}
#define EP_ID 0x0A,0x01

//********************************

volatile unsigned int32 erro_nao_trans = 0,
                        erro_mcp = 0;

volatile unsigned int16 horas_ligado = 0;
                                              
volatile unsigned int1 flag_interr = 0b0,
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

#pragma int_ext
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


