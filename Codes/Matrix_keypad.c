/*********************************************************************************
LCD   <->   AVR Connection

       VSS ->GND
       VDD ->+5V
       VEE -> CENTER PIN OF 10K POT (OTHER TWO PIN OF POT TO +5V AND GND)
             ADJ. THE POT UNTIL YOU HAVE A CLEAR TEXT DISPLAY.

       RS -> PA3
       RW -> PA6
       E  -> PA4

       DB0 -> N/C
       DB1 -> N/C
       DB2 -> N/C
       DB3 -> N/C

       DB4 -> PB0
       DB5 -> PB1
       DB6 -> PB2
       DB7 -> PB3

       LED+ ->+5V (VIA 100 OHM RES)
       LED- ->GND

    KEYPAD

       COL1 ->  PD7
       COL2 ->  PD6
       COL3 ->  PD5
       COL4 ->  PD4

       ROW1 ->  PD3
       ROW2 ->  PA2
       ROW3 ->  PA1
       ROW4 ->  PA0


    ******************************************************************************/

    #include <avr/io.h>
    #include <util/delay.h>

    #include "lcd.h"
    #include "myutils.h"

    #define KEYPAD D  //KEYPAD IS ATTACHED ON PORTD

    //Defining the keypad port
    //*******************************
    #define KEYPAD_PORT PORT(KEYPAD)
    #define KEYPAD_DDR   DDR(KEYPAD)
    #define KEYPAD_PIN   PIN(KEYPAD)
    //*******************************


    /*******************************************

    Function return the keycode of keypressed
    on the Keypad. Keys are numbered as follows

    [01] [02] [03] [04]
    [05] [06] [07] [08] 
    [09] [10] [11] [12]
    [13] [14] [15] [16]

   Arguments:
      None

   Return:
      Any number between 1-16 depending on
      keypressed.

      255 (hex 0xFF) if NO keypressed.

   Precondition:
      None. Can be called without any setup.

   *******************************************/


   uint8_t GetKeyPress()
   {
      uint8_t r,c;

      KEYPAD_PORT|= 0X0F;	//we have not made it output but writing HIGH to it. So it will put the pins D7->D4 in TRISTATE

      for(c=0;c<4;c++)	//since we have four columns
      {
         KEYPAD_DDR&=~(0XFF);	//make them all inputs

         KEYPAD_DDR|=(0X80>>c);
         for(r=0;r<4;r++)
         {
            if(!(KEYPAD_PIN & (0X08>>r)))	//if the pin is LOW
            {
               return ((r*4+c)+1);	//Plus 1 because here the counts of rows and columns are starting from zero but our designation of //numbers to each button is from 1
            }
         }
      }

      return 0XFF;//Indicate No key pressed
   }


   void main()
   {
      //Wait for LCD To Start
      _delay_loop_2(0);

      //Now initialize the module
      LCDInit(LS_NONE);

      uint8_t key;

      while(1)
      {
         key=GetKeyPress(); //Get the keycode of pressed key

         LCDWriteIntXY(0,0,key,3);  //Print it at location 0,0 on LCD.
      }
   }
