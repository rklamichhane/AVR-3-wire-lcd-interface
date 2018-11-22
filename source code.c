#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#define data PORTB
#define ddr DDRB
#define pos_data PB0
#define pos_shift PB1
#define pos_latch PB2
#define data_high() (data|=1<<pos_data)
#define data_low() (data&=~(1<<pos_data))
//#define data PORTA
#define ctrl PORTA
#define en 0
#define rs 1








void init_hc()
{
   ddr|=(1<<pos_data)|(1<<pos_shift)|(1<<pos_latch);
}
void shift()
{
   data|=(1<<pos_shift);
   _delay_us(1);
   data&=~(1<<pos_shift);
   _delay_us(1);
}
void latch()
{
   data|=(1<<pos_latch);
   _delay_us(1);
   data&=~(1<<pos_latch);
    _delay_us(1);
}
void send_byte(uint8_t ch)
{
   unsigned char i;
   for(i=0;i<8;i++)
   {
   if(ch&0b10000000)
   {
      data_high();
   }
     
   else
   { 
      data_low();
   }
       
   shift();
   ch=ch<<1;
   }
   latch();
}







/*void lcd_cmd1(unsigned char cmd)
{
   ctrl=(0<<rs)|(1<<en);
   //_delay_ms(1000);
   data=cmd&0xf0;
   //ctrl|=(1<<en);
   //_delay_ms(1000);
   ctrl&=~(1<<en);
   //_delay_ms(10000);
   data=cmd<<4;
   ctrl|=(1<<en);
   //_delay_us(1000);
   ctrl&=~(1<<en);
   _delay_us(1000);
}
void lcd_data1(unsigned char dta)
{
   ctrl|=(1<<rs);
   data=dta&0xf0;
   ctrl|=(1<<en);
   _delay_us(1000);
   ctrl&=~(1<<en);
   _delay_us(1000);
   data=dta<<4;
   ctrl|=(1<<en);
   _delay_us(1000);
   ctrl&=~(1<<en);
   _delay_us(1000);
}





*/


void lcd_data(unsigned char cmd)
{
   PORTC=cmd;
   char dat=0;
   char val=0b11110111;
   dat|=(1<<PB3)|(1<<PB2);
   send_byte(dat);
   _delay_us(10);
   dat|=(cmd&0xf0);
   send_byte(dat);
   _delay_us(10);
   //dat=(0<<PB3);
   send_byte(dat&val);
   
   _delay_us(10);
   dat=0;
   dat|=(1<<PB3)|(1<<PB2);
   cmd<<=4;
   dat=dat|cmd;
   _delay_us(10);
   send_byte(dat);
  _delay_us(10);
  dat&=~(1<<PB3);
   send_byte(dat);
  _delay_us(10);
}

void lcd_cmd(unsigned char cmd)
{
   PORTC=cmd;
   char dat=0;
   char val=0b11110111;
   dat|=(1<<PB3)|(0<<PB2);
   send_byte(dat);
   _delay_us(10);
   dat|=(cmd&0xf0);
   send_byte(dat);
   _delay_us(10);
   //dat=(0<<PB3);
   send_byte(dat&val);
   
   _delay_us(10);
   dat=0;
   dat|=(1<<PB3)|(0<<PB2);
   cmd<<=4;
   dat=dat|cmd;
   _delay_ms(1);
   send_byte(dat);
  _delay_us(10);
  dat&=~(1<<PB3);
   send_byte(dat);
  _delay_us(10);
}


void lcd_init()
{
   DDRA=0xff;
//   ctrl&=~(1<<en);
   _delay_us(1000);
   lcd_cmd(0x33);
   //lcd_cmd1(0x33);
    _delay_us(1000);
   lcd_cmd(0x32);
  // lcd_cmd1(0x32);
    _delay_us(1000);
   lcd_cmd(0x28);
   //lcd_cmd1(0x28);
    _delay_us(1000);
   
   lcd_cmd(0x0e);
  // lcd_cmd1(0x0e);
   _delay_us(2000);
   lcd_cmd(0x06);
  // lcd_cmd(0x06);
   _delay_us(1000);
   lcd_cmd(0x01);
   _delay_ms(500);
}
void lcd_gotoxy(unsigned char x, unsigned char y)
{
   unsigned char add[]={0x80,0xC0};
   lcd_cmd(add[x-1]+y-1);
   _delay_us(1000);
}
void lcd_strng( char *str)
{
   unsigned char i=0;
   while(str[i]!=0)
   {
      lcd_data(str[i]);
     // lcd_data1(str[i]);
      i++;
   }
}
void lcd_intg(int a,unsigned int b)
{
 
   int d1[5],i=0;
   for(i=0;i<b;i++)
   {
   d1[i]=a%10;
   a/=10;
   
   }
   for(i=b;i>=0;i--)
   lcd_data('0'+d1[i]);
}
 


int main()
 { 
    
   int i,j;
    DDRC=0xff;
    init_hc();
    lcd_init();
    lcd_strng("hello");
     lcd_intg(PB3,2);
    while(1);
    char ch=0x01;
    init_hc();
    while(1)
    {
    for(i=0;i<8;i++)
    {
       send_byte(ch);
       //_delay_ms(2000);
       ch=ch<<1;
       for(j=0;j<2;j++)
       {
	  send_byte(0);
	  //_delay_ms(2000);
       }
       
       
    }
    ch=0x01;
 }
 return 0;
 }