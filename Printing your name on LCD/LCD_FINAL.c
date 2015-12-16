#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define clear 0b00000001                //replacing characters with space       
#define cursor_Return 0b00000010        //setting the cursor back to its original position  
#define entry 0b00000110                //shifting the cursor left to right
#define off 0b00001000                  //turning display off
#define on 0b00001100                   //turning display on
#define Function_reset 0b00110000       //resetting the LCD   
#define Function_set 0b00101000         //Setting the LCD for 4-bit data,2 line display 
#define cursor 0b10000000               //setting cursor position

/*RS on pin2 of arduino
Enable on pin 3 of arduino
D4 on pin4
D5 on pin5
D6 on pin6
D7 on pin7*/

uint8_t name1[]   = "MOHAMMAD NAYEEM";

/*
Assuming the data is 1
making data = 1 if necessary
repeating for each data bit
*/
void lcd_write(uint8_t theByte)
{
    PORTD &= ~(1<<PD7);                        
    if (theByte & 1<<7) 
	{
		PORTD |= (1<<PD7);     
	}
    PORTD &= ~(1<<PD6);                        
    if (theByte & 1<<6) 
	{
		PORTD |= (1<<PD6);
	}
    PORTD &= ~(1<<PD5);
    if (theByte & 1<<5) 
	{
		PORTD |= (1<<PD5);
	}
    PORTD &= ~(1<<PD4);
    if (theByte & 1<<4) 
	{
		PORTD |= (1<<PD4);
	}
	
    PORTD |= (1<<PD3);    //enable is high to write the data  
    _delay_us(1);         //data set-up time
    PORTD &= ~(1<<PD3);   //again setting the enable pin to low
    _delay_us(1);         //data hold time
}

void characterWrite(uint8_t theData)
{
    PORTD |= (1<<PD2);         //RS is set low
    PORTD &= ~(1<<PD3);        //enable is set low
    lcd_write(theData);        //First writing the upper four bits of data 
    lcd_write(theData << 4);   //then sending the lower 4 bits of data as we are using 4 bit mode
}

void stringWrite(uint8_t theString[])
{
    int i = 0;            
    while (theString[i] != 0)
    {
        characterWrite(theString[i]);
        i++;                       //counting the number of charactersS
        _delay_us(80);                              
    }
}

void order(uint8_t theInstruction)
{
    PORTD &= ~(1<<PD2);                //RS is low      
    PORTD &= ~(1<<PD3);                //enable is low
    lcd_write(theInstruction);         //Same as in characterwrite function
    lcd_write(theInstruction << 4);
}

//We have set the lcd mode to 4-bit mode instead of 8-bit mode
void initialize()
{
    _delay_ms(20);                      //power up delay
    PORTD &= ~(1<<PD2);                 //RS is set low
    PORTD |= (0b00110000);              //First part of reset sequence
    _delay_ms(5);                                  
    PORTD |= (0b00110000);              //Second part of reset sequence
    _delay_us(200);                                
    PORTD |= (0b00110000);              //Third part of reset sequence  
    _delay_us(200);                                
    PORTD |= (0b00100000);               
    _delay_ms(5);                                  
    lcd_write(Function_set);            //setting 4-bit mode of lcd
    _delay_us(80);                             
    order(off);                         //turning display off
    _delay_us(80);                             
    order(clear);                       //clearing display
    _delay_ms(4);                              
    order(entry);                       //shifting by desired characters
    _delay_us(80);                             
    order(on);                          //turning the display on
    _delay_us(80);                             
}

/*RS on pin2 of arduino
Enable on pin 3 of arduino
D4 on pin4
D5 on pin5
D6 on pin6
D7 on pin7*/

int main(void)
{
    DDRD |= 0b11111100;    //initialization of pins for lcd
    initialize();          //initializing lcd as 4-bit mode
    while(1)
    {
       order(cursor_Return);    //curcor position
       _delay_ms(5);
       order(clear);            //clearing the lcd
       _delay_ms(5);
       stringWrite(name1);      //writing first name to lcd
       _delay_us(240); 
       _delay_ms(1000);
    }
    return 0;
}
