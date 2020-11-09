
#ifdef PISCALEDSTATUS_F
    //====================================================================
    // piscaLedStatus()
    // 
    // Recebe: status_led = qte de piscadas
    //         porta = registrador responsavel pelo led (recomendado LATx)   
    //====================================================================
    struct latx{
    unsigned int8 status;
    unsigned int8 statusBuf;
    unsigned int8 pausa;
    unsigned int8 pausaBuf;
    unsigned int16 porta;
    unsigned int8 pino;

    };
    volatile unsigned int8 contaPeriodo = TEMPOCICLOLEDS;
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
    }
#endif

#ifdef EEPROM_F
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
#endif



#ifdef TIMER0_F
    #define int_per_sec 2
    
    volatile unsigned int   counter = 0,
                            per = 0,
                            sec = 0,
                            min = 0;
                                              
    volatile unsigned   int1 um_periodo = 0b0,
                        um_segundo = 0b0, 
                        um_minuto = 0b0,
                        uma_hora = 0b0;

    #pragma INT_TIMER1
    void timer1_isr(){  // interrupt routine    
    counter--;  // decrements counter which is set to it_per_sec 

    //PERIODO
    if(counter==0){         
        per++;                
        counter=int_per_sec; //resets counter
        um_periodo = 0b01;
    }

    //SEGUNDOS
    if(per==5){
        per = 0;
        sec++;
        um_segundo = 0b1;
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
    set_timer1(53036);
    }
#endif
