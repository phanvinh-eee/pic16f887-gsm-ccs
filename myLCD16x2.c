///////////////////////////////////////////////////////////////////////////
////                             LCDD.C                                ////
////                 Driver for common LCD modules                     ////
////                                                                   ////
////  lcd_init()   Must be called before any other function.           ////
////                                                                   ////
////  lcd_putc(c)  Will display c on the next position of the LCD.     ////
////                     The following have special meaning:           ////
////                      \f  Clear display                            ////
////                      \n  Go to start of second line               ////
////                      \b  Move back one position                   ////
////                                                                   ////
////  lcd_gotoxy(x,y) Set write position on LCD (upper left is 1,1)    ////
////                                                                   ////
////  lcd_getc(x,y)   Returns character at position x,y on LCD         ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

/*
   LCD 16x2 Driver
   Using I2C interface
   Coding : Hoang Nguyen
   Date : 25/3/2010
   Rev : 1.0
   Platform : PIC C Compiler
*/

// As defined in the following structure the pin connection is as follows:
//     D0  enable
//     D1  rs
//     D2  rw
//     D4  D4
//     D5  D5
//     D6  D6
//     D7  D7
//
//   LCD pins D0-D3 are not used and PIC D3 is not used.

#define LCD_EN_PIN PIN_D2
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1

#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 lines
#define lcd_line_two 0x40    // LCD RAM address for the second line
//#define LCD_TRIS_LOCATION PORTD

BYTE const LCD_INIT_STRING[4] = {0x20 | (lcd_type << 2), 0xc, 1, 6};
                             // These bytes need to be sent to the LCD
                             // to start it up.



//EPIC 2 dung 4 bit thap
//EPIC1 dung 4 bit cao
//#define input_data() input_data_port()&0xf0
//#define input_data() ((input_data_port()&0x0f)<<4)
//#define output_data(n) output_data_port((n<<4)|(input_data_port()&0x0f))
//#define output_data(n) output_data_port(n|(input_data_port()&0xf0))

void output_data( BYTE n ) 
{
 
      
      output_bit(PIN_D4,n&0x01);
      output_bit(PIN_D5,(n>>1)&0x01);
      output_bit(PIN_D6,(n>>2)&0x01);
      output_bit(PIN_D7,(n>>3)&0x01) ;
      //output_d(n<<4);
      
}

void lcd_strobe() {
      output_high(LCD_EN_PIN);
      delay_us(2);
      output_low(LCD_EN_PIN);
}


void lcd_send_nibble( BYTE n ) {
      output_data(n);
      delay_cycles(1);
      output_high(LCD_EN_PIN);
      delay_us(2);
      output_low(LCD_EN_PIN);
}


void lcd_send_byte( BYTE address, BYTE n ) {

      output_low(LCD_RS_PIN);
      delay_ms(5);//while ( bit_test(lcd_read_byte(),7) ) ;//hay bi treo
      
      output_bit(LCD_RS_PIN,address);
      //delay_cycles(1);
      output_low(LCD_RW_PIN);
      //delay_cycles(1);
      output_low(LCD_EN_PIN);
      lcd_send_nibble(n >> 4);
      lcd_send_nibble(n & 0xf);
}


void lcd_init() {
    BYTE i;
    //set_tris_lcd(LCD_WRITE);
    delay_ms(10);
    output_low(LCD_RS_PIN);
    delay_ms(10);
    output_low(LCD_RW_PIN);
    delay_ms(10);
    output_low(LCD_EN_PIN);
    delay_ms(10);
    for(i=1;i<=3;++i) {
       lcd_send_nibble(3);
       delay_ms(5);
    }
    lcd_send_nibble(2);
    for(i=0;i<=3;++i)
       lcd_send_byte(0,LCD_INIT_STRING[i]);
}



void lcd_gotoxy( BYTE x, BYTE y) {
   BYTE address;

   if(y!=1)
     address=lcd_line_two;
   else
     address=0;
   address+=x-1;
   lcd_send_byte(0,0x80|address);
}

void lcd_putc( char c) {
   switch (c) {
     case '\f'   : lcd_send_byte(0,1);
                   delay_ms(2);
                                           break;
     case '\n'   : lcd_gotoxy(1,2);        break;
     case '\b'   : lcd_send_byte(0,0x10);  break;
     default     : lcd_send_byte(1,c);     break;
   }
}

void lcd_putstring( char *c,char x,char y) {
int i;
   lcd_gotoxy(x,y);
   i=0;
   while(c[i] != 0)
   {
      lcd_putc(c[i]);
      i++;
   }
}


