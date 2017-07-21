#include "pic_gsm_demo.h"
#include "myLCD16x2.c"
#include <math.h>



#define Led0 PIN_C1
#define Led1 PIN_C2
#define GSM_PW PIN_E2
#define GSM_SENSE PIN_E1
#define TEXTMODE 5;
#define PDUMODE 0;
//clear buffer
#define UART_emptyIn()     {RxHead=0;RxTail=0;}
#define RXBUFSIZE   70
#define TIME_OUT  (int32)5000000
#define PHONE_NUM "0903513011"


int16 delay_100;
char timed_getc(void);
int1 timeout_error;
int1 frame_get;
int1 new_frame;
void GSM_init(void);
void GSM_start(void);
void GSM_sync(void);
void GSM_sel_mess_mode(char mode);
int UART_getc_timeout();
int UART_getc_blocking();
unsigned int check_new_message();
int read_message();

int RxBuf[RXBUFSIZE];
int RxHead = 0;
int RxTail = 0;
char new_mess_idx[10];

char cmd_str[30];

//**********************************************************
//      USART Interrupt service
//**********************************************************
void UART_ISR()//source from MDK8051 project
{
   char c;
   c=getc();
   //fputc(c,debug_com);
   RxBuf[RxHead] = c;
   RxHead++;
   if (RxHead > (RXBUFSIZE - 1))// check for wrap around.
      RxHead = 0;
}


#int_TIMER0
TIMER0_isr() 
{
   if(delay_100 < 40)
      delay_100++;
   else
      {
         delay_100=0;
         output_toggle(Led0);
      }
  
   return 0;
}

#INT_RDA
void rda()
{
   UART_ISR();
}



void GSM_init(void)
{
   output_bit(GSM_PW,1);
   output_bit(Led1,1);
   output_float(GSM_SENSE);
}
void GSM_start(void)
{
//start up GSM module
   delay_ms(2000);
   lcd_init();
    
   //printf("GSM demo \n \r");   
   output_bit(GSM_PW,0);
   output_bit(Led1,0);
   delay_ms(2000);
   output_bit(GSM_PW,1);
   output_bit(Led1,1);
   //fprintf(debug_com,"\n\r\n\r....Please wait for initializing SIM300CZ");
   //fprintf(debug_com,"\n\rBe patient, Maybe it will take you some minutes\n\r");
   lcd_putc("\nGSM initializing");
   delay_ms(5000);
   lcd_putc("\nPlease wait.......");
   delay_ms(5000);  
   lcd_putc("\n..to init SIM300");
   delay_ms(5000);
   lcd_putc("\nPlease wait.......");
   delay_ms(5000);  
   lcd_putc("\n..to init SIM300");
   delay_ms(5000);
   lcd_putc("\nPlease wait.......");
   delay_ms(5000);  
   lcd_putc("\n..to init SIM300");
}
void GSM_sync(void)
{
//try to test connection of GSM module
      //lcd_putc("\nGSM start OK            "); 
      putc(13);//enter CR
      
      delay_ms(1000);
      //lcd_putc("\nConnect to GSM       "); 
      output_bit(Led1,0);  
      new_frame=0;
      frame_get=0;
      //fprintf(debug_com,"\n\rHost>AT");
      printf("AT");
      putc(13);//enter CR
       delay_ms(1000);
       //fprintf(debug_com,"\n\rHost>AT");
       printf("AT");
       putc(13);//enter CR
       delay_ms(1000);
       //fprintf(debug_com,"\n\rHost>AT");
       printf("AT");
       putc(13);//enter CR
       delay_ms(1000);

}
void GSM_sel_mess_mode(char mode)
{
   if(mode>0) //text mode
      {
         output_bit(Led1,0);
         //fprintf(debug_com,"\n\rHost>AT+CMGF=1");
         printf("AT+CMGF=1");
         putc(13);//enter CR
          output_bit(Led1,1);
         delay_ms(1000);
         output_bit(Led1,0);
         //fprintf(debug_com,"\n\rHost>AT+CMGF=1");         
         printf("AT+CMGF=1");
         putc(13);//enter CR
          output_bit(Led1,1);
         delay_ms(1000);
         output_bit(Led1,0);
         }
   else //PDU mode
      {
         output_bit(Led1,0);
         //fprintf(debug_com,"\n\rHost>AT+CMGF=0");         
         printf("AT+CMGF=0");
         putc(13);//enter CR
         output_bit(Led1,1);
         delay_ms(1000);
         output_bit(Led1,0);
         //fprintf(debug_com,"\n\rHost>AT+CMGF=0");         
         printf("AT+CMGF=0");
         putc(13);//enter CR
         output_bit(Led1,1);
         delay_ms(1000);
         output_bit(Led1,0);
      
      }
   

}
void main()
{
   int1 gsm_ok;
   int cnt;
   int1 temp;
   char c;
   char string[20];
   char *temp1;
   int i;
   char rec_str[10];
   
   GSM_init();
   
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   

   // TODO: USER CODE!!
   //fprintf(debug_com,"\n\r         H-LABORATORY - www.hlab.com.vn");
   //fprintf(debug_com,"\n\r                 Pic GSM 1 demo");
   
   GSM_start();
   GSM_sync();
   GSM_sel_mess_mode(5);
    //fprintf(debug_com,"\n\rHost>ATE1");
    printf("ATE1"); //echo on
    //printf("ATE0"); //echo off
    putc(13);//enter CR
    delay_ms(1000);//
    //fprintf(debug_com,"\n\rHost>AT");  
    printf("AT");
    putc(13);//enter CR
    delay_ms(1000);
    
   
    output_bit(Led1,1);
    lcd_putc("\nGSM send message      ");
    printf("AT");
    putc(13);//enter CR
    delay_ms(1000);
    output_bit(Led1,0);
    //fprintf(debug_com,"\n\rHost>AT+CMGS=");
    printf("AT+CMGS=");
    putc(34);//"
    //fprintf(debug_com,"\n\rHost>0989703330");
    printf(PHONE_NUM);
    putc(34);//"
      
    putc(13);//enter CR
    delay_ms(1000);//cho ki tu ">"
    //fprintf(debug_com,"\n\rHost>SIM300 Ready");
    printf("Sim300 San sang hoat dong!");
    delay_ms(1000);//
    putc(0x1A);//CTRL+Z
    output_bit(Led1,1);
    delay_ms(2000);//
  
      
    lcd_putc("\nGSM sent done    ");
  
    while(1);
      

}




