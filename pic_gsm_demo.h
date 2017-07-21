#include <16F887.h>
#device adc=8
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD
#use delay(clock=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
//#use rs232(baud=9600,parity=N,xmit=PIN_B0,rcv=PIN_B1,bits=8,stream=debug_com)

