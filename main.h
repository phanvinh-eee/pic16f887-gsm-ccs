#include <16F887.h>

#device PASS_STRINGS=IN_RAM,adc=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOPUT                    //No Power Up Timer
#FUSES MCLR                     //Master Clear pin enabled
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOCPD                    //No EE protection
#FUSES NOBROWNOUT               //No brownout reset
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOWRT                    //Program memory not write protected
#FUSES BORV40                   //Brownout reset at 4.0V
#FUSES RESERVED                 //Used to set the reserved FUSE bits

#use delay(clock=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

#use fast_io(b)
#use fast_io(c)
#use fast_io(e)

#byte portb=0x06
#byte portc=0x07
#byte porte=0x09
#byte trisc=0x87
#byte trise=0x89
#byte pie1=0x8C
#bit C1=portc.1
#bit C2=portc.2
#bit C3=portc.3
#bit B0=portb.0
#bit B1=portb.1
#bit B2=portb.2
#bit B3=portb.3
#bit B4=portb.4
#bit B5=portb.5
#bit B6=portb.6
#bit B7=portb.7
#bit trisc1=trisc.1
#bit trisc2=trisc.2
#bit trisc3=trisc.3
#bit Led1=portb.2
#bit Led2=portb.3
#bit trise1=trise.1
#bit rcie=pie1.5
//!p_char1=sizeof(clcc)/sizeof(char);
//!p_char2=sizeof(on1)/sizeof(char);
//!    lcd_putc('\f');
//!    for(i=1;i<=p_char1;i++){     
//!     lcd_gotoxy(i,1);
//!     lcd_putc("i");}
//!     for(i=1;i<=p_char2;i++){  
//!     lcd_gotoxy(i,2);
//!     lcd_putc("i");
//!    }
//!    RxInit();
//!    smsinit();
//!    if(IsStringReceived(on1)){
//!    lcd_putc('\f');
//!    for(i=1;i<=16;i++){     
//!     lcd_gotoxy(i,1);
//!     lcd_putc(smstext[i-1]);
//!     lcd_gotoxy(i,2);
//!     lcd_putc(RxBuf[i-1]);
//!    }
//!    }
//!    delay_ms(2000);
//!    lcd_putc("\f");
//!    lcd_gotoxy(1,1);
//!    if(gsmstr(smstext,on1)==1){
//!    lcd_putc("True");
//!    }
//!    IsStringReceived(on1);
//!    p_char1=sizeof(clcc)/sizeof(char);
//!p_char2=sizeof(on1)/sizeof(char);
//!if(gsmstr(RxBuf,on1)==1){
//!lcd_putc('\f');
//!    for(i=1;i<=p_char1;i++){     
//!     lcd_gotoxy(i,1);
//!     lcd_putc("i");}
//!     for(i=1;i<=p_char2;i++){  
//!     lcd_gotoxy(i,2);
//!     lcd_putc("i");
//!    }
//!}
//!int gsmstr(char *str,char *str1){
//!int t=0;
//!for(i=0;i<p_char1-p_char2;i++){
//!   t=0;
//!   for(j=0;j<p_char2;j++){
//!      if(str[i+j]==str1[j])
//!      {
//!         t++;
//!      }
//!   }
//!   if(t==p_char2)return 1;   
//!}
//!return 0;
//!}
