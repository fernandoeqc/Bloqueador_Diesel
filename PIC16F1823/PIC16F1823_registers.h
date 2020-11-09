#byte MCU_INDF0 = 0x000
#byte MCU_INDF1 = 0x001
#byte MCU_PCL = 0x002
#byte MCU_STATUS = 0x003
#bit    MCU_C = MCU_STATUS.0
#bit    MCU_DC = MCU_STATUS.1
#bit    MCU_Z = MCU_STATUS.2
#bit    MCU_PD = MCU_STATUS.3
#bit    MCU_TO = MCU_STATUS.4
#word MCU_FSR0 = 0x004
#word MCU_FSR1 = 0x006
#byte MCU_BSR = 0x008
#byte MCU_WREG = 0x009
#byte MCU_PCLATH = 0x00A
#byte MCU_INTCON = 0x00B
#bit    MCU_IOCIF = MCU_INTCON.0
#bit    MCU_INTF = MCU_INTCON.1
#bit    MCU_TMR0IF = MCU_INTCON.2
#bit    MCU_IOCIE = MCU_INTCON.3
#bit    MCU_INTE = MCU_INTCON.4
#bit    MCU_TMR0IE = MCU_INTCON.5
#bit    MCU_PEIE = MCU_INTCON.6
#bit    MCU_GIE = MCU_INTCON.7
#byte MCU_INTCON = 0x00B
#bit    MCU_T0IF = MCU_INTCON.2
#bit    MCU_T0IE = MCU_INTCON.5
#byte MCU_PORTA = 0x00C
#byte MCU_PORTC = 0x00E
#byte MCU_PIR1 = 0x011
#bit    MCU_TMR1IF = MCU_PIR1.0
#bit    MCU_TMR2IF = MCU_PIR1.1
#bit    MCU_CCP1IF = MCU_PIR1.2
#bit    MCU_SSP1IF = MCU_PIR1.3
#bit    MCU_TXIF = MCU_PIR1.4
#bit    MCU_RCIF = MCU_PIR1.5
#bit    MCU_ADIF = MCU_PIR1.6
#bit    MCU_TMR1GIF = MCU_PIR1.7
#byte MCU_PIR2 = 0x012
#bit    MCU_BCL1IF = MCU_PIR2.3
#bit    MCU_EEIF = MCU_PIR2.4
#bit    MCU_C1IF = MCU_PIR2.5
#bit    MCU_C2IF = MCU_PIR2.6
#bit    MCU_OSFIF = MCU_PIR2.7
#byte MCU_TMR0 = 0x015
#word MCU_TMR1 = 0x016
#byte MCU_T1CON = 0x018
#bit    MCU_TMR1ON = MCU_T1CON.0
#bit    MCU_T1SYNC = MCU_T1CON.2
#bit    MCU_T1OSCEN = MCU_T1CON.3
#bit    MCU_T1CKPS0 = MCU_T1CON.4
#bit    MCU_T1CKPS1 = MCU_T1CON.5
#bit    MCU_TMR1CS0 = MCU_T1CON.6
#bit    MCU_TMR1CS1 = MCU_T1CON.7
#byte MCU_T1GCON = 0x019
#bit    MCU_T1GSS0 = MCU_T1GCON.0
#bit    MCU_T1GSS1 = MCU_T1GCON.1
#bit    MCU_T1GVAL = MCU_T1GCON.2
#bit    MCU_T1GGO = MCU_T1GCON.3
#bit    MCU_T1GSPM = MCU_T1GCON.4
#bit    MCU_T1GTM = MCU_T1GCON.5
#bit    MCU_T1GPOL = MCU_T1GCON.6
#bit    MCU_TMR1GE = MCU_T1GCON.7
#byte MCU_TMR2 = 0x01A
#byte MCU_PR2 = 0x01B
#byte MCU_T2CON = 0x01C
#bit    MCU_T2CKPS0 = MCU_T2CON.0
#bit    MCU_T2CKPS1 = MCU_T2CON.1
#bit    MCU_TMR2ON = MCU_T2CON.2
#bit    MCU_T2OUTPS0 = MCU_T2CON.3
#bit    MCU_T2OUTPS1 = MCU_T2CON.4
#bit    MCU_T2OUTPS2 = MCU_T2CON.5
#bit    MCU_T2OUTPS3 = MCU_T2CON.6
#byte MCU_CPSCON0 = 0x01E
#bit    MCU_T0XCS = MCU_CPSCON0.0
#bit    MCU_CPSOUT = MCU_CPSCON0.1
#bit    MCU_CPSRNG0 = MCU_CPSCON0.2
#bit    MCU_CPSRNG1 = MCU_CPSCON0.3
#bit    MCU_CPSRM = MCU_CPSCON0.6
#bit    MCU_CPSON = MCU_CPSCON0.7
#byte MCU_CPSCON1 = 0x01F
#byte MCU_TRISA = 0x08C
#byte MCU_TRISC = 0x08E
#byte MCU_PIE1 = 0x091
#bit    MCU_TMR1IE = MCU_PIE1.0
#bit    MCU_TMR2IE = MCU_PIE1.1
#bit    MCU_CCP1IE = MCU_PIE1.2
#bit    MCU_SSP1IE = MCU_PIE1.3
#bit    MCU_TXIE = MCU_PIE1.4
#bit    MCU_RCIE = MCU_PIE1.5
#bit    MCU_ADIE = MCU_PIE1.6
#bit    MCU_TMR1GIE = MCU_PIE1.7
#byte MCU_PIE2 = 0x092
#bit    MCU_BCL1IE = MCU_PIE2.3
#bit    MCU_EEIE = MCU_PIE2.4
#bit    MCU_C1IE = MCU_PIE2.5
#bit    MCU_C2IE = MCU_PIE2.6
#bit    MCU_OSFIE = MCU_PIE2.7
#byte MCU_OPTION_REG = 0x095
#bit    MCU_PS0 = MCU_OPTION_REG.0
#bit    MCU_PS1 = MCU_OPTION_REG.1
#bit    MCU_PS2 = MCU_OPTION_REG.2
#bit    MCU_PSA = MCU_OPTION_REG.3
#bit    MCU_TMR0SE = MCU_OPTION_REG.4
#bit    MCU_TMR0CS = MCU_OPTION_REG.5
#bit    MCU_INTEDG = MCU_OPTION_REG.6
#bit    MCU_WPUEN = MCU_OPTION_REG.7
#byte MCU_OPTION_REG = 0x095
#bit    MCU_T0SE = MCU_OPTION_REG.4
#bit    MCU_T0CS = MCU_OPTION_REG.5
#byte MCU_PCON = 0x096
#bit    MCU_BOR = MCU_PCON.0
#bit    MCU_POR = MCU_PCON.1
#bit    MCU_RI = MCU_PCON.2
#bit    MCU_RMCLR = MCU_PCON.3
#bit    MCU_STKUNF = MCU_PCON.6
#bit    MCU_STKOVF = MCU_PCON.7
#byte MCU_WDTCON = 0x097
#bit    MCU_SWDTEN = MCU_WDTCON.0
#bit    MCU_WDTPS0 = MCU_WDTCON.1
#bit    MCU_WDTPS1 = MCU_WDTCON.2
#bit    MCU_WDTPS2 = MCU_WDTCON.3
#bit    MCU_WDTPS3 = MCU_WDTCON.4
#bit    MCU_WDTPS4 = MCU_WDTCON.5
#byte MCU_OSCTUNE = 0x098
#byte MCU_OSCCON = 0x099
#bit    MCU_SCS0 = MCU_OSCCON.0
#bit    MCU_SCS1 = MCU_OSCCON.1
#bit    MCU_IRCF0 = MCU_OSCCON.3
#bit    MCU_IRCF1 = MCU_OSCCON.4
#bit    MCU_IRCF2 = MCU_OSCCON.5
#bit    MCU_IRCF3 = MCU_OSCCON.6
#bit    MCU_SPLLEN = MCU_OSCCON.7
#byte MCU_OSCSTAT = 0x09A
#bit    MCU_HFIOFS = MCU_OSCSTAT.0
#bit    MCU_LFIOFR = MCU_OSCSTAT.1
#bit    MCU_MFIOFR = MCU_OSCSTAT.2
#bit    MCU_HFIOFL = MCU_OSCSTAT.3
#bit    MCU_HFIOFR = MCU_OSCSTAT.4
#bit    MCU_OSTS = MCU_OSCSTAT.5
#bit    MCU_PLLR = MCU_OSCSTAT.6
#bit    MCU_T1OSCR = MCU_OSCSTAT.7
#word MCU_ADRES = 0x09B
#byte MCU_ADCON0 = 0x09D
#bit    MCU_ADON = MCU_ADCON0.0
#bit    MCU_GO = MCU_ADCON0.1
#bit    MCU_CHS0 = MCU_ADCON0.2
#bit    MCU_CHS1 = MCU_ADCON0.3
#bit    MCU_CHS2 = MCU_ADCON0.4
#bit    MCU_CHS3 = MCU_ADCON0.5
#bit    MCU_CHS4 = MCU_ADCON0.6
#byte MCU_ADCON0 = 0x09D
#bit    MCU_ADGO = MCU_ADCON0.1
#byte MCU_ADCON0 = 0x09D
#bit    MCU_DONE = MCU_ADCON0.1
#byte MCU_ADCON1 = 0x09E
#bit    MCU_ADPREF0 = MCU_ADCON1.0
#bit    MCU_ADPREF1 = MCU_ADCON1.1
#bit    MCU_ADCS0 = MCU_ADCON1.4
#bit    MCU_ADCS1 = MCU_ADCON1.5
#bit    MCU_ADCS2 = MCU_ADCON1.6
#bit    MCU_ADFM = MCU_ADCON1.7
#byte MCU_LATA = 0x10C
#bit    MCU_LATA0 = MCU_LATA.0
#bit    MCU_LATA1 = MCU_LATA.1
#bit    MCU_LATA2 = MCU_LATA.2
#bit    MCU_LATA4 = MCU_LATA.4
#bit    MCU_LATA5 = MCU_LATA.5
#byte MCU_LATC = 0x10E
#byte MCU_CM1CON0 = 0x111
#bit    MCU_C1SYNC = MCU_CM1CON0.0
#bit    MCU_C1HYS = MCU_CM1CON0.1
#bit    MCU_C1SP = MCU_CM1CON0.2
#bit    MCU_C1POL = MCU_CM1CON0.4
#bit    MCU_C1OE = MCU_CM1CON0.5
#bit    MCU_C1OUT = MCU_CM1CON0.6
#bit    MCU_C1ON = MCU_CM1CON0.7
#byte MCU_CM1CON1 = 0x112
#bit    MCU_C1NCH0 = MCU_CM1CON1.0
#bit    MCU_C1NCH1 = MCU_CM1CON1.1
#bit    MCU_C1PCH0 = MCU_CM1CON1.4
#bit    MCU_C1PCH1 = MCU_CM1CON1.5
#bit    MCU_C1INTN = MCU_CM1CON1.6
#bit    MCU_C1INTP = MCU_CM1CON1.7
#byte MCU_CM2CON0 = 0x113
#bit    MCU_C2SYNC = MCU_CM2CON0.0
#bit    MCU_C2HYS = MCU_CM2CON0.1
#bit    MCU_C2SP = MCU_CM2CON0.2
#bit    MCU_C2POL = MCU_CM2CON0.4
#bit    MCU_C2OE = MCU_CM2CON0.5
#bit    MCU_C2OUT = MCU_CM2CON0.6
#bit    MCU_C2ON = MCU_CM2CON0.7
#byte MCU_CM2CON1 = 0x114
#bit    MCU_C2NCH0 = MCU_CM2CON1.0
#bit    MCU_C2NCH1 = MCU_CM2CON1.1
#bit    MCU_C2PCH0 = MCU_CM2CON1.4
#bit    MCU_C2PCH1 = MCU_CM2CON1.5
#bit    MCU_C2INTN = MCU_CM2CON1.6
#bit    MCU_C2INTP = MCU_CM2CON1.7
#byte MCU_CMOUT = 0x115
#bit    MCU_MC1OUT = MCU_CMOUT.0
#bit    MCU_MC2OUT = MCU_CMOUT.1
#byte MCU_BORCON = 0x116
#bit    MCU_BORRDY = MCU_BORCON.0
#bit    MCU_SBOREN = MCU_BORCON.7
#byte MCU_FVRCON = 0x117
#bit    MCU_ADFVR0 = MCU_FVRCON.0
#bit    MCU_ADFVR1 = MCU_FVRCON.1
#bit    MCU_CDAFVR0 = MCU_FVRCON.2
#bit    MCU_CDAFVR1 = MCU_FVRCON.3
#bit    MCU_TSRNG = MCU_FVRCON.4
#bit    MCU_TSEN = MCU_FVRCON.5
#bit    MCU_FVRRDY = MCU_FVRCON.6
#bit    MCU_FVREN = MCU_FVRCON.7
#byte MCU_DACCON0 = 0x118
#bit    MCU_DACPSS0 = MCU_DACCON0.2
#bit    MCU_DACPSS1 = MCU_DACCON0.3
#bit    MCU_DACOE = MCU_DACCON0.5
#bit    MCU_DACLPS = MCU_DACCON0.6
#bit    MCU_DACEN = MCU_DACCON0.7
#byte MCU_DACCON1 = 0x119
#byte MCU_SRCON0 = 0x11A
#bit    MCU_SRPR = MCU_SRCON0.0
#bit    MCU_SRPS = MCU_SRCON0.1
#bit    MCU_SRNQEN = MCU_SRCON0.2
#bit    MCU_SRQEN = MCU_SRCON0.3
#bit    MCU_SRCLK0 = MCU_SRCON0.4
#bit    MCU_SRCLK1 = MCU_SRCON0.5
#bit    MCU_SRCLK2 = MCU_SRCON0.6
#bit    MCU_SRLEN = MCU_SRCON0.7
#byte MCU_SRCON1 = 0x11B
#bit    MCU_SRRC1E = MCU_SRCON1.0
#bit    MCU_SRRC2E = MCU_SRCON1.1
#bit    MCU_SRRCKE = MCU_SRCON1.2
#bit    MCU_SRRPE = MCU_SRCON1.3
#bit    MCU_SRSC1E = MCU_SRCON1.4
#bit    MCU_SRSC2E = MCU_SRCON1.5
#bit    MCU_SRSCKE = MCU_SRCON1.6
#bit    MCU_SRSPE = MCU_SRCON1.7
#byte MCU_APFCON = 0x11D
#bit    MCU_CCP1SEL = MCU_APFCON.0
#bit    MCU_P1BSEL = MCU_APFCON.1
#bit    MCU_TXCKSEL = MCU_APFCON.2
#bit    MCU_T1GSEL = MCU_APFCON.3
#bit    MCU_SSSEL = MCU_APFCON.5
#bit    MCU_SDOSEL = MCU_APFCON.6
#bit    MCU_RXDTSEL = MCU_APFCON.7
#byte MCU_APFCON = 0x11D
#bit    MCU_SS1SEL = MCU_APFCON.5
#bit    MCU_SDO1SEL = MCU_APFCON.6
#byte MCU_ANSELA = 0x18C
#bit    MCU_ANSA0 = MCU_ANSELA.0
#bit    MCU_ANSA1 = MCU_ANSELA.1
#bit    MCU_ANSA2 = MCU_ANSELA.2
#bit    MCU_ANSA4 = MCU_ANSELA.4
#byte MCU_ANSELC = 0x18E
#word MCU_EEADR = 0x191
#word MCU_EEDAT = 0x193
#byte MCU_EECON1 = 0x195
#bit    MCU_RD = MCU_EECON1.0
#bit    MCU_WR = MCU_EECON1.1
#bit    MCU_WREN = MCU_EECON1.2
#bit    MCU_WRERR = MCU_EECON1.3
#bit    MCU_FREE = MCU_EECON1.4
#bit    MCU_LWLO = MCU_EECON1.5
#bit    MCU_CFGS = MCU_EECON1.6
#bit    MCU_EEPGD = MCU_EECON1.7
#byte MCU_EECON2 = 0x196
#byte MCU_RCREG = 0x199
#byte MCU_TXREG = 0x19A
#word MCU_SP1BRG = 0x19B
#byte MCU_RCSTA = 0x19D
#bit    MCU_RX9D = MCU_RCSTA.0
#bit    MCU_OERR = MCU_RCSTA.1
#bit    MCU_FERR = MCU_RCSTA.2
#bit    MCU_ADDEN = MCU_RCSTA.3
#bit    MCU_CREN = MCU_RCSTA.4
#bit    MCU_SREN = MCU_RCSTA.5
#bit    MCU_RX9 = MCU_RCSTA.6
#bit    MCU_SPEN = MCU_RCSTA.7
#byte MCU_TXSTA = 0x19E
#bit    MCU_TX9D = MCU_TXSTA.0
#bit    MCU_TRMT = MCU_TXSTA.1
#bit    MCU_BRGH = MCU_TXSTA.2
#bit    MCU_SENDB = MCU_TXSTA.3
#bit    MCU_SYNC = MCU_TXSTA.4
#bit    MCU_TXEN = MCU_TXSTA.5
#bit    MCU_TX9 = MCU_TXSTA.6
#bit    MCU_CSRC = MCU_TXSTA.7
#byte MCU_BAUDCON = 0x19F
#bit    MCU_ABDEN = MCU_BAUDCON.0
#bit    MCU_WUE = MCU_BAUDCON.1
#bit    MCU_BRG16 = MCU_BAUDCON.3
#bit    MCU_SCKP = MCU_BAUDCON.4
#bit    MCU_RCIDL = MCU_BAUDCON.6
#bit    MCU_ABDOVF = MCU_BAUDCON.7
#byte MCU_WPUA = 0x20C
#byte MCU_WPUC = 0x20E
#byte MCU_SSP1BUF = 0x211
#byte MCU_SSP1ADD = 0x212
#byte MCU_SSP1MSK = 0x213
#byte MCU_SSP1STAT = 0x214
#bit    MCU_BF = MCU_SSP1STAT.0
#bit    MCU_UA = MCU_SSP1STAT.1
#bit    MCU_R = MCU_SSP1STAT.2
#bit    MCU_S = MCU_SSP1STAT.3
#bit    MCU_P = MCU_SSP1STAT.4
#bit    MCU_D = MCU_SSP1STAT.5
#bit    MCU_CKE = MCU_SSP1STAT.6
#bit    MCU_SMP = MCU_SSP1STAT.7
#byte MCU_SSP1CON1 = 0x215
#bit    MCU_SSPM0 = MCU_SSP1CON1.0
#bit    MCU_SSPM1 = MCU_SSP1CON1.1
#bit    MCU_SSPM2 = MCU_SSP1CON1.2
#bit    MCU_SSPM3 = MCU_SSP1CON1.3
#bit    MCU_CKP = MCU_SSP1CON1.4
#bit    MCU_SSPEN = MCU_SSP1CON1.5
#bit    MCU_SSPOV = MCU_SSP1CON1.6
#bit    MCU_WCOL = MCU_SSP1CON1.7
#byte MCU_SSP1CON2 = 0x216
#bit    MCU_SEN = MCU_SSP1CON2.0
#bit    MCU_RSEN = MCU_SSP1CON2.1
#bit    MCU_PEN = MCU_SSP1CON2.2
#bit    MCU_RCEN = MCU_SSP1CON2.3
#bit    MCU_ACKEN = MCU_SSP1CON2.4
#bit    MCU_ACKDT = MCU_SSP1CON2.5
#bit    MCU_ACKSTAT = MCU_SSP1CON2.6
#bit    MCU_GCEN = MCU_SSP1CON2.7
#byte MCU_SSP1CON3 = 0x217
#bit    MCU_DHEN = MCU_SSP1CON3.0
#bit    MCU_AHEN = MCU_SSP1CON3.1
#bit    MCU_SBCDE = MCU_SSP1CON3.2
#bit    MCU_SDAHT = MCU_SSP1CON3.3
#bit    MCU_BOEN = MCU_SSP1CON3.4
#bit    MCU_SCIE = MCU_SSP1CON3.5
#bit    MCU_PCIE = MCU_SSP1CON3.6
#bit    MCU_ACKTIM = MCU_SSP1CON3.7
#word MCU_CCPR1 = 0x291
#byte MCU_CCP1CON = 0x293
#bit    MCU_CCP1M0 = MCU_CCP1CON.0
#bit    MCU_CCP1M1 = MCU_CCP1CON.1
#bit    MCU_CCP1M2 = MCU_CCP1CON.2
#bit    MCU_CCP1M3 = MCU_CCP1CON.3
#bit    MCU_DC1B0 = MCU_CCP1CON.4
#bit    MCU_DC1B1 = MCU_CCP1CON.5
#bit    MCU_P1M0 = MCU_CCP1CON.6
#bit    MCU_P1M1 = MCU_CCP1CON.7
#byte MCU_PWM1CON = 0x294
#bit    MCU_P1DC0 = MCU_PWM1CON.0
#bit    MCU_P1DC1 = MCU_PWM1CON.1
#bit    MCU_P1DC2 = MCU_PWM1CON.2
#bit    MCU_P1DC3 = MCU_PWM1CON.3
#bit    MCU_P1DC4 = MCU_PWM1CON.4
#bit    MCU_P1DC5 = MCU_PWM1CON.5
#bit    MCU_P1DC6 = MCU_PWM1CON.6
#bit    MCU_P1RSEN = MCU_PWM1CON.7
#byte MCU_CCP1AS = 0x295
#bit    MCU_PSS1BD0 = MCU_CCP1AS.0
#bit    MCU_PSS1BD1 = MCU_CCP1AS.1
#bit    MCU_PSS1AC0 = MCU_CCP1AS.2
#bit    MCU_PSS1AC1 = MCU_CCP1AS.3
#bit    MCU_CCP1AS0 = MCU_CCP1AS.4
#bit    MCU_CCP1AS1 = MCU_CCP1AS.5
#bit    MCU_CCP1AS2 = MCU_CCP1AS.6
#bit    MCU_CCP1ASE = MCU_CCP1AS.7
#byte MCU_PSTR1CON = 0x296
#bit    MCU_STR1A = MCU_PSTR1CON.0
#bit    MCU_STR1B = MCU_PSTR1CON.1
#bit    MCU_STR1C = MCU_PSTR1CON.2
#bit    MCU_STR1D = MCU_PSTR1CON.3
#bit    MCU_STR1SYNC = MCU_PSTR1CON.4
#byte MCU_IOCAP = 0x391
#byte MCU_IOCAN = 0x392
#byte MCU_IOCAF = 0x393
#byte MCU_CLKRCON = 0x39A
#bit    MCU_CLKRDIV0 = MCU_CLKRCON.0
#bit    MCU_CLKRDIV1 = MCU_CLKRCON.1
#bit    MCU_CLKRDIV2 = MCU_CLKRCON.2
#bit    MCU_CLKRDC0 = MCU_CLKRCON.3
#bit    MCU_CLKRDC1 = MCU_CLKRCON.4
#bit    MCU_CLKRSLR = MCU_CLKRCON.5
#bit    MCU_CLKROE = MCU_CLKRCON.6
#bit    MCU_CLKREN = MCU_CLKRCON.7
#byte MCU_MDCON = 0x39C
#bit    MCU_MDBIT = MCU_MDCON.0
#bit    MCU_MDOUT = MCU_MDCON.3
#bit    MCU_MDOPOL = MCU_MDCON.4
#bit    MCU_MDSLR = MCU_MDCON.5
#bit    MCU_MDOE = MCU_MDCON.6
#bit    MCU_MDEN = MCU_MDCON.7
#byte MCU_MDSRC = 0x39D
#bit    MCU_MDMS0 = MCU_MDSRC.0
#bit    MCU_MDMS1 = MCU_MDSRC.1
#bit    MCU_MDMS2 = MCU_MDSRC.2
#bit    MCU_MDMS3 = MCU_MDSRC.3
#bit    MCU_MDMSODIS = MCU_MDSRC.7
#word MCU_MDCAR = 0x39E
#byte MCU_STATUS_SHAD = 0xFE4
#bit    MCU_C_SHAD = MCU_STATUS_SHAD.0
#bit    MCU_DC_SHAD = MCU_STATUS_SHAD.1
#bit    MCU_Z_SHAD = MCU_STATUS_SHAD.2
#byte MCU_WREG_SHAD = 0xFE5
#byte MCU_BSR_SHAD = 0xFE6
#byte MCU_PCLATH_SHAD = 0xFE7
#byte MCU_FSR0L_SHAD = 0xFE8
#byte MCU_FSR0H_SHAD = 0xFE9
#byte MCU_FSR1L_SHAD = 0xFEA
#byte MCU_FSR1H_SHAD = 0xFEB
#byte MCU_STKPTR = 0xFED
#word MCU_TOS = 0xFEE
