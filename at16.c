//#define debug 1
//#include <mega16.h>
//#include <delay.h>

#include <util/delay.h>
#include <avr/io.h> 

#ifdef debug
#include <stdio.h>
#endif
#define FWD 0xAA
#define REV 0x55
#define R 0x22
#define L 0x88
#define CW 0x99
#define CCW 0x66
#define STOP 0x00
#define B 0xFF
#define RSPEED OCR1AL
#define LSPEED OCR1BL
#define SPEED0 255
#define SPEED1 0
#define SPEED2 0
#define SPEED3 0
#define MAX 3
#define HMAX 1
void move (unsigned char dir,unsigned char delay,unsigned char power);

unsigned char i,rdev,ldev, cdev, ip,delay,dir,power,dirl,history[MAX],hcount=0,rotpow;
#ifdef debug
unsigned char rep=0,prev=0;
#endif

void main(void)
{
// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In

// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
PORTA=0x00;
DDRA=0x00;
// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
PORTB=0x00;
DDRB=0x00;
// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
PORTC=0x00;
DDRC=0xFF;
// Port D initialization
// Func7=In Func6=In Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=0 State4=0 State3=T State2=T State1=T State0=T
PORTD=0x00;
DDRD=0x30;
// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;
// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 921.600 kHz
// Mode: Fast PWM top=00FFh
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// Noise Canceler: Off
// Input Capture on Falling Edge
TCCR1A=0xA1;
TCCR1B=0x0A;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0xFF;
OCR1BH=0x00;
OCR1BL=0xFF;
// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;
// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;
#ifdef debug
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud rate: 57600
UCSRA=0x00;
UCSRB=0x18;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x07;
#endif
// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;
// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;



while (1)
{
      
#ifdef debug
if(rep<255)
rep++;
if(prev!=PINA) 
               {
               prev=PINA;
               printf("%u\r",rep);
               for(i=0;i<8;i++)
                               printf("%u\t",(prev>>i)&0x01);
               rep=0;
               }
#endif

if((PORTA)!=255)
             {
             rotpow=255;
             ldev=rdev=0;
    //         if(PINA.0==0)
    //                      rdev=4;
    //         if(PINA.1==0)
    //                      rdev=3;
    
    // expected ')' before numeric constant

       //below is method 1..      if(PINA.2==0)
             if( (PORTA && (1<<2)) )
                          rdev=2;
      // below is method 2..      if(PINA.3==0)
             if( ( (PORTA<<4)>>7 )==1 )             
                          rdev=1;


      //       if(PINA.4==0)         // centre pin
             if( ( (PINA<<3)>>7 )==1 )
                          cdev=1;

                          
      //       if(PINA.5==0)
             if( ( (PORTA<<2)>>7 )==1 )
                          ldev=1; // used to be 2
      //       if(PINA.6==0)
             if( ( (PORTA<<1)>>7 )==1 )
                          ldev=2; // used to be 3
 
      //       if(PINA.7==0)
      //                    ldev=4;


             if(rdev>ldev)
                          move(R,0,195+12*rdev);   // (dir delay power)
             if(rdev<ldev)
                          move(L,0,195+12*ldev);
             if(rdev==ldev)
     //          if((rdev==ldev)&&cdev==1)  use if necess later
                           move(FWD,0,200);
             }
             
else 
             {
             for(i=0,dirl=0;i<MAX;i++) 
                                       {
                                       if(history[i]==L)
                                                        {dirl++;}
                                       }
             if(rotpow<160) 
                            {rotpow=160;}
             if(rotpow<255) 
                            {rotpow++;}
             if(dirl>HMAX)
                            {move(CW,0,rotpow);}
             else
                            {move(CCW,0,rotpow);}
             }
};
}


void move (unsigned char dir,unsigned char delay,unsigned char power) 
     {
     PORTC=dir;
     if(dir==L || dir==R) 
               {
               hcount=(hcount+1)%MAX;
               history[hcount]=dir;
               }
LSPEED=RSPEED=255;//power;
//delay_ms(delay);
}
