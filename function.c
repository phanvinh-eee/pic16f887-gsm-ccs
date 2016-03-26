#include "myLCD16x2.c"
#include <string.h>
#define RXBUFSIZE 55
#define smssize 26
#define numbersize 16

//char init[]="GSM initializing";
int RxBuf[RXBUFSIZE];
char numberphone[numbersize];
char smstext[smssize];
int RxHead = 0;
//char khesim;
int i,numsms=0;
int calling=0,startsms=0;
//const char cmti[]="+CMTI: \"SM\",";
const char error1[]="Khong thanh cong";
const char OK[]="OK";
const char cusd[]="+CUSD";
const char setkttk[]="AT+CUSD=1";
const char kttka[]="AT+CUSD=1,\"\*101\#\"";
const char kttkb[]="AT+CUSD=1,\"\*102\#\"";
const char saicuphap[]="Sai cu phap";
const char msg9[]="DS:";
const char kttk1[]="kttk1";
const char kttk2[]="kttk2";
const char dssdt[]="dssdt";
const char cnmi[]="AT+CNMI=2,2,0,0,0";
const char cmgd[]="AT+CMGD=1";
const char cmgdall[]="AT+CMGD=1,2";
//const char cpms[]="AT+CPMS=1";
const char quenmk[]="quenmk";
const char doimk[]="doimk";
const char doiad[]="doiad";
const char msg1[]="Danh sach nguoi dieu khien khong duoc qua 5 nguoi, xoa bot so dien thoai";
const char msg2[]="Khong dung so dien thoai";
const char msg3[]="Da them:";
const char msg4[]="Da xoa: ";
const char msg5[]="Do dai mat khau la 6 ky tu";
const char msg6[]="Sai mat khau";
const char msg7[]="Doi mat khau thanh cong";
const char msg8[]="Doi admin thanh cong";
const char them[]="them0";
const char xoahet[]="xoahet";
const char xoa[]="xoa0";
//const char cpas[]="+CPAS: 3";
//const char atcpas[]="AT+CPAS";
const char atclcc[]="AT+CLCC";
const char clcc[]="+CLCC: 1,1,4,0,0";
//const char new[]="+CMGR: \"REC UNREAD\"";
const char cmee[]="AT+CMEE=0";
const char cmgf[]="AT+CMGF=1";
//const char unread[]="AT+CMGL=\"REC UNREAD\"";
//const char cmgr[]="AT+CMGR=1";
//char *atorder;
char numfull[6],numtem1[numbersize];
char numtem[numbersize];
char mk[7];
int p_char1,p_char2;
int newsms=0;
int admin=0;
int smsmode,smscount;
char *c;
char *ret_val;
//ham tim vi tri 1 ky tu thu i trong chuoi
int gsmchr(char str[],int size,char ch, int num){
int t=0;
for(i=0;i<size;i++){
if(str[i]==ch)t++;
if(t==num)return i;
}
return 0;
}
void eeprom_write_string(unsigned int8 addr,unsigned char*str)
{
  while(*str)
  {
  write_eeprom(addr,*str);
  addr++;
  str++;
  }
}
void eeprom_read_string(unsigned int8 addr, unsigned char* str,unsigned int8 len)
{
  unsigned int8 i;
  for(i=0;i<len;i++)
  {
      str[i]=0;
      str[i]=read_eeprom(addr+i);      
  }
  str[len]=0;
}
//ham xoa buffer va so dien thoai
void RxInit(){
 RxHead=0;
 //for(i=0;i<RXBUFSIZE;i++)
   RxBuf[0]=0;
 for(i=0;i<numbersize;i++)
   numberphone[i]=' '; 
}
//ham xoa buffer sms
void smsInit(){
//for(i=0;i<smssize;i++)
 smstext[0]=0;
for(i=0;i<numbersize;i++)
   numberphone[i]=' '; 
newsms=0;
smsmode=0;
smscount=0;
}
//ham gui 1 lenh toi sim900
void sendCommand(char *AT_cmd){
RxInit();
delay_ms(800);
printf("%s\r\n",AT_cmd);
}
//ham so sanh ket qua tra ve cua sim900 voi 1 chuoi
int IsStringReceived(char *compare_string)
{
 do{
   delay_ms(800);
  ret_val=strstr(RxBuf,compare_string);    
  }while(RxHead==0);
  if(ret_val!=0)return 1;
 return 0;
}
void UART_ISR()//source from MDK8051 project
{     
   c=getc();
   //if(calling){
   if (RxHead < RXBUFSIZE-1){// check for wrap around.
      RxBuf[RxHead] = c;
      RxHead ++;         
      RxBuf[RxHead] = 0;
   }
   if(c=='+')startsms=1;
   if(startsms==1&&c=='C')startsms=2;
   if(startsms==2&&c=='M')startsms=3;
   if(startsms==3&&c=='T')startsms=4;
   if(startsms==4){
      RxHead=0;
      smscount=0;
      smsmode=0;  
      startsms=0;
   }
      if(smsmode==6)
      {
      if(c==';'){
         calling=0;
      }      
      if(smscount<smssize-1){
      smstext[smscount]=c;
      smscount++;
      smstext[smscount]=0;
      }
      }
      if(c=='"')smsmode++;
   //}
}
void turndevice(){
   b0=0;
   delay_ms(800);
   printf("ATH\r\n");   
   delay_ms(5000);
   b0=1;
}
void turnled(){
led1=1;
delay_ms(1000);
led1=0;
}

//ham gui nt toi mot sdt
void sendsms(char *number_str, char *message_str){
   printf("AT+CMGS=\"");
   printf(number_str);
   printf("\"\r");
   delay_ms(800);
   printf(message_str);
   delay_ms(800);
   putc(26);
}
void themsdt(){
       p_char1=gsmchr(smstext,smssize,'m',1);//lay so dien thoai trong tin nhan 
       p_char2=gsmchr(smstext,smssize,';',1);   
      for(i=0;i<15;i++){
      numtem1[i]=' ';
      }
       if(p_char1<p_char2){
       for(i=p_char1+1;i<p_char2;i++){
       numtem1[i-p_char1-1]=smstext[i];
       }      
      admin=0;
      for(i=1;i<5;i++)
         if(numfull[i]=='0'){
         admin=i;
         i=5;
         }
      if(admin>0){
       switch(admin){    //ghi so dien thoai vao vi tri trong
       case 1:eeprom_write_string(0x20,numtem1);
       numfull[1]='1';
       break;
       case 2:eeprom_write_string(0x30,numtem1);              
       numfull[2]='1';
       break;
       case 3:eeprom_write_string(0x40,numtem1);
       numfull[3]='1';
       break;
       case 4:eeprom_write_string(0x50,numtem1);
       numfull[4]='1';
       break;
       }     
       eeprom_write_string(0x00,numfull);//ghi lai trang thai danh sach
       strcpy(RxBuf,msg3);
       for(i=8;i<23;i++){
          RxBuf[i]=numtem1[i-8];
       if(RxBuf[i]==' ')RxBuf[i]=0;
       }
       sendsms(numberphone,RxBuf);//da them       
       //turnled();
       }else sendsms(numberphone,msg1);//neu danh sach full thi gui nt bao nguoi dung xoa bot           
     } 
}
void xoasdt(){
int t=0;
         //eeprom_read_string(0x00,numfull,5);
         p_char1=gsmchr(smstext,smssize,'a',1);//lay so dien thoai trong tin nhan 
         p_char2=gsmchr(smstext,smssize,';',1); 
          for(i=0;i<15;i++){
            numtem1[i]=' ';
          }
         if(p_char1<p_char2){    
         for(i=p_char1+1;i<p_char2;i++){
         numtem1[i-p_char1-1]=smstext[i];
         }     
         
         admin=0; 
         for(i=1;i<5;i++)
         if(numfull[i]=='1'){ //so sanh vs sdt trong danh sach
           eeprom_read_string(16*(i+1),numtem,15);           
            for(t=0;t<15;t++){
            if(numtem1[t]==numtem[t]){          
            if(numtem[t]==' '){
               numfull[i]='0';
               admin=i;               
               break;
            }            
            }
          }
         }
          if(admin==0) sendsms(numberphone,msg2);//khong dung sdt
          else {
          eeprom_write_string(0x00,numfull);//ghi lai trang thai danh sach
          strcpy(RxBuf,msg4);
          for(i=8;i<23;i++){
             RxBuf[i]=numtem1[i-8];
             if(RxBuf[i]==' ')RxBuf[i]=0;
             }
         //strcat(RxBuf,numtem1);
          sendsms(numberphone,RxBuf);//da xoa
          }          
     }          
}
void doimktk(){
   int t=0;
   p_char1=gsmchr(smstext,smssize,'k',1);//lay so dien thoai trong tin nhan 
       p_char2=gsmchr(smstext,smssize,'*',1); 

       if(p_char2>p_char1){
       eeprom_read_string(0x60,mk,6);//doc mk tu bo nho
       for(t=0;t<6;t++)      
         if(smstext[p_char1+t+1]!=mk[t])break;
       if(t==6){
       p_char1=gsmchr(smstext,smssize,';',1);
       t=p_char1-p_char2;       
       if(t==7){
          for(t=0;t<6;t++)
            mk[t]=smstext[p_char2+t+1];
          eeprom_write_string(0x60,mk);
          sendsms(numberphone,msg7);//doi mat khau thanh cong
       }else sendsms(numberphone,msg5);//mat khau 6 ky tu
       }else sendsms(numberphone,msg6);//sai mat khau
       }
}
void kttkchinh(){
int t=0;
   sendCommand(setkttk);
       if( IsStringReceived(OK)){
         sendCommand(kttka);
         delay_ms(800);
         t=0;
         while(IsStringReceived(cusd)!=1){
         RxHead=0; 
         t++;
         if(t==10)break;
         }     
         if(t<10){
            for(i=9;i<15;i++)
            if(numtem1[i]==' ')numtem1[i]=0;
            RxBuf[0]='K';
            RxBuf[1]='T';
            RxBuf[2]='T';
            RxBuf[3]='K';
            RxBuf[4]='1';
         sendsms( numtem1, RxBuf);
         }else sendsms(numtem1,error1);
       }
}
void kttkphu(){
   int t=0;
       sendCommand(setkttk);
       if( IsStringReceived(OK)){
         sendCommand(kttkb);
         delay_ms(800);
         t=0;
         while(IsStringReceived(cusd)!=1){
         RxHead=0; 
         t++;
         if(t==10)break;
         }     
         if(t<10){
            for(i=9;i<15;i++)
            if(numtem1[i]==' ')numtem1[i]=0;
            RxBuf[0]='K';
            RxBuf[1]='T';
            RxBuf[2]='T';
            RxBuf[3]='K';
            RxBuf[4]='2';
         sendsms( numtem1, RxBuf);
         }else sendsms(numtem1,error1);
       }
}
int admin1(){
   int t=0;
      if(strstr(smstext,them)!=0){  //them 1 so dien thoai      
         return 1;
      }
     //xoa sdt trong danh sach
      if(strstr(smstext,xoa)!=0){
         return 2;       
      }
      if(strstr(smstext,doimk)!=0){
       return 3;
     }
     if(strstr(smstext, kttk1)!=0){                 
       return 4;       
       }
      if(strstr(smstext, kttk2)!=0){                 
        return 5;       
      }  
      if(strstr(smstext,quenmk)!=0){//gui lai mat khau
       eeprom_read_string(0x60,mk,6);//doc mk tu bo nho
       sendsms(numberphone,mk);       
      }     
     else if(strstr(smstext, dssdt)!=0){//lay danh sach sdt
     strcpy(RxBuf,msg9);
     RxHead=3;
     for(i=1;i<5;i++)
     if(numfull[i]=='1'){
       eeprom_read_string((i+1)*16,numtem,15); 
       for(t=0;t<15;t++){
       RxBuf[RxHead++]= numtem[t];
       if(numtem[t]==' '){
       RxBuf[RxHead-1]=';';
       RxBuf[RxHead++]=' ';
       t=15;        
       }
       }
     }else RxBuf[RxHead]=0;
       sendsms(numberphone,RxBuf);//gui danh sach sdt
     }       
      else if(strstr(smstext,xoahet)!=0){
         for(i=1;i<5;i++)
         numfull[i]='0';
         eeprom_write_string(0x00,numfull); 
         strcpy(RxBuf,msg4);
         RxBuf[6]='!';
         sendsms(numberphone,msg4);//da xoa
         //turnled();
         }
       else{
       sendsms(numberphone,saicuphap);
      } 
      return 0;
}

int admin2(){
int t=0;
   if(strstr(smstext,doiad)!=0){
       p_char1=gsmchr(smstext,smssize,'d',2);//lay so dien thoai trong tin nhan 
       p_char2=gsmchr(smstext,smssize,';',1);          
       if(p_char2>p_char1){
       for(t=0;t<6;t++)      
         if(smstext[p_char1+t+1]!=mk[t])break;
       if(t==6){
         eeprom_read_string(0x10,numtem,15);
         eeprom_write_string(0x10,numtem1);
         eeprom_write_string(admin*16,numtem); 
         sendsms(numberphone,msg8);//doi admin thanh cong 
        }else {
         sendsms(numberphone,msg6);//sai mat khau      
        }
       }          
     }
     else if(strstr(smstext, kttk1)!=0){
       return 1;       
       }
      else if(strstr(smstext, kttk2)!=0){                 
         return 2;       
      }
     else{
       sendsms(numberphone,saicuphap);
       }
    return 0;
}

void admin0(){
      eeprom_read_string(0x10,numtem,15); 
       for(i=0;i<15;i++)
       if(numtem[i]==' ')numtem[i]=0;
       strcpy( RxBuf, numberphone);
       strcat( RxBuf, smstext);
       sendsms(numtem,RxBuf);//gui lai noi dung tin nhan so la
}
//ham doc noi dung tin nhan co lenh bat led hay khong, neu co thi bat led
int readsms(){
int t=0;       
       p_char1=gsmchr(RxBuf,RXBUFSIZE,'"',1);//lay so dt nt den
       p_char2=gsmchr(RxBuf,RXBUFSIZE,'"',2);   
       if(p_char1<p_char2){    
          for(i=p_char1+1;i<p_char2;i++){
            numberphone[i-p_char1-1]=RxBuf[i];
          }   
          //numberphone[p_char2-p_char1-1]=' ';

          for(i=3;i<16;i++){
            numtem1[i-2]=numberphone[i];
            if(numberphone[i]==' '){
            numberphone[i]=0;
            }
          }//printf("%s", numberphone);          
         numtem1[0]='0';                 
        admin=0;       
        eeprom_read_string(0x00,numfull,5);//doc trang thai danh sach sdt
       
        for(i=0;i<5;i++)
         if(numfull[i]=='1'){ //so sanh vs sdt trong danh sach
           eeprom_read_string(16*(i+1),numtem,15);           
            for(t=0;t<15;t++){
            if(numtem1[t]==numtem[t]){          
            if(numtem[t]==' '){
               admin=i+1;               
               return 1;;
            }                        
          }else break;
         } 
       }//printf("%d", admin);              
     }
   return 0;
}

//ham kiem tra cuoc goi den, neu co thi bat led trong 5s
int checkcall(){
int t; 
   sendCommand(atclcc);
   if(IsStringReceived(clcc)){
    p_char1=gsmchr(RxBuf,RXBUFSIZE,'"',1); //lay sdt goi den
    p_char2=gsmchr(RxBuf,RXBUFSIZE,'"',2);   
    if(p_char1<p_char2){    
    for(i=p_char1+1;i<p_char2;i++){
    numberphone[i-p_char1-1]=RxBuf[i];
    }
    //numberphone(p_char2-p_char1-1)=' ';
      eeprom_read_string(0x00,numfull,5);//doc trang thai danh sach sdt
      for(i=0;i<5;i++)
      if(numfull[i]=='1'){ //doc trang thai danh sach
      eeprom_read_string(16*(i+1),numtem,15);//doc sdt trong danh sach  
      for(t=0;t<15;t++){
      if(numberphone[t]==numtem[t]){
      if(numtem[t]==' '){  
       
       return 1;
      }
      }else break;
      }     
      }
      delay_ms(800);
      printf("ATH\r\n");   
      delay_ms(800);       
      
    }
   }
  return 0;
}

void echo(){
   printf("ATE0\r\n");
   delay_ms(800);//
}

//ham set che do text mode cho sim900
void GSM_sel_mess_mode(int mode)
{
   if(mode>0) //text mode
      {
//!        delay_ms(800);         
//!        printf("AT+CMEE=0\r\n");
//!        delay_ms(800);         
//!         printf("AT+CMGF=1\r\n");
//!         delay_ms(800);   
//!         printf("AT+CNMI=2,2,0,0,0\r\n");
//!         delay_ms(800);
         sendCommand(cmgf);
         sendCommand(cmee);
         sendCommand(cnmi);
         //sendCommand(cpms);
      }
   else //PDU mode
      {
         delay_ms(800);
         //output_bit(Led1,1);    
         printf("AT&F\r\n");
         //putc(13);//enter CR
         //Led1=!Led1;
         delay_ms(1000);   
      } 
}
//ham khoi dong lcd
//!void GSM_start()
//!{
//!   delay_ms(800);
//!   lcd_init();     
//!   lcd_putc(init);
//!}
void GSM_sync()
{
      delay_ms(2000);      
      //output_bit(Led1,1);           
      printf("AT\r\n");
      delay_ms(1000);
       //output_bit(Led1,1); 
      printf("AT\r\n");
      delay_ms(1000);  
      GSM_sel_mess_mode(0);
}


