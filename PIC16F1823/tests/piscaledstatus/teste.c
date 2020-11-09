#include <teste.h>
#define TEMPOCICLOLEDS 30

#include <placa_recep_antigo.h>
#include <can-mcp2515.c>


////////////////////////////////////////////////////////////////////////
//
// piscaLedStatus()
// 
// Recebe: status_led = qte de piscadas
//		   porta = registrador responsavel pelo led (recomendado LATx)	
////////////////////////////////////////////////////////////////////////
/* volatile unsigned int8 contaPeriodo = TEMPOCICLOLEDS;
void piscaLedStatus(struct latx *lat){
	struct latx latCopy;
	latCopy = *lat;

	//responsavel por desligar o led no segundo periodo
	//se led esta ligado, desliga
	if(bit_test(*latCopy.porta,latCopy.pino))
	{
		bit_clear(*latCopy.porta,latCopy.pino);
		return;
	}

	//entra no fim de cada pausa, pausa por (latCopy.status) tempo, e liga led
	if (latCopy.statusBuf == 0)
	{
		if(contaPeriodo != TEMPOCICLOLEDS)
		{
			return;
		}
			latCopy.statusBuf = latCopy.status;
			latCopy.pausaBuf = contaPeriodo - (latCopy.status * 2);
	}

	//responsavel por pausar antes de cada ciclo
	if (latCopy.pausaBuf != 0)
	{
		latCopy.pausaBuf--;
	}

	else
	{
		bit_set(*latCopy.porta, latCopy.pino);
		latCopy.statusBuf--;
	}

	*lat = latCopy;
} */

////////////////////////////////////////////////////////////////////////
//
// piscaLedStatus()
// 
// Recebe: status_led = qte de piscadas
//		   porta = registrador responsavel pelo led (recomendado LATx)	
////////////////////////////////////////////////////////////////////////
/* volatile unsigned int8 contaPeriodo = TEMPOCICLOLEDS;
void piscaLedStatus(struct latx *lat){
	struct latx latCopy;
	latCopy = *lat;

	//responsavel por desligar o led no segundo periodo
	//se led esta ligado, desliga
	if(bit_test(*latCopy.porta,latCopy.pino))
	{
		bit_clear(*latCopy.porta,latCopy.pino);
		return;
	}

	//se tempo de pausa acabou
	if (latCopy.statusBuf == 0)
	{
		//esta no meio do ciclo, retorna. (aguarda ciclo acabar)
		if(contaPeriodo != TEMPOCICLOLEDS)
		{
			return;
		}
		//senao, significa que ee inicio de um ciclo. statusBuf pega quantas
			//vezes deve piscar, e pausaBuf recebe quanto tempo deve ficar em pausa.
			//pausaBuf: tempo em pausa = tempo total do ciclo - tempo piscando
		latCopy.statusBuf = latCopy.status;
		latCopy.pausaBuf = contaPeriodo - (latCopy.status * 2);
	}

	//acende led
	else
	{
		bit_set(*latCopy.porta, latCopy.pino);
		latCopy.statusBuf--;
	}

	*lat = latCopy;
} */

/* 
void piscaLedStatus(latx *lat)
{
	unsigned int16 test = 0;

	latx latCopy;
	latCopy = *lat;
	
	test = bit_test(*latCopy.porta,latCopy.pino);
	if(test)
	{
		output_high(LED2);
	}

	bit_set(*latCopy.porta,latCopy.pino);
	*lat = latCopy;
} */

void main()
{
	output_a(0b00000000);
	output_c(0b00000000);

	//controle pino LED1 (RA4)
	struct latx LATA4;
	LATA4.status = 0x05;
	LATA4.statusBuf = 0x00;
	LATA4.pausa = 0x0A;
	LATA4.pausaBuf = 0x00;
	LATA4.porta = 0x10C;
	LATA4.pino = 0x04;

	//controle pino LED2 (RC5)
	struct latx LATC5;
	LATC5.status = 0x01;
	LATC5.statusBuf = 0x00;
	LATC5.pausa = 0x0A;
	LATC5.pausaBuf = 0x00;
	LATC5.porta = 0x10E;
	LATC5.pino = 0x05;


	disable_interrupts(GLOBAL);                 // habilitar interr global
	//enable_interrupts(INT_EXT_H2L);             // interrupcao CAN
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);   // setar timer1 para interno
	enable_interrupts(INT_TIMER1);              // habilita Timer1 
	set_timer1(0);                              // limpar flag TMR1H & TMR1L 
	counter=int_per_sec;
	enable_interrupts(GLOBAL);                  // habilitar interr global

	//LATA.statusBuf = 4;
	output_low(LED1);
	
	while(TRUE)
	{		
		if(flag_interr)
		{          
			flag_interr = 0b0;
		}

		if(um_periodo)
		{
			um_periodo = 0;

			//passagem LATA.4 como parametro
			piscaLedStatus(&LATA4);
			piscaLedStatus(&LATC5);
			
			contaPeriodo--;
			if(contaPeriodo == 0)
			{
				contaPeriodo = TEMPOCICLOLEDS;
			}
		}	

		if(um_segundo)
		{      
			//output_toggle(LED2);
			um_segundo = 0b0;
		}
	}
}
