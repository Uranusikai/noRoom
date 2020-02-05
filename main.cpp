/*

 * lab2_2019.cpp

 *

 * Created: 1/29/2019 15:37:02 PM

 * Author : Sikai

 */ 



/*

This program is a solution to Lab 2 -- 

	Pushbutton switches are connected to Pins PD2(pulled high when switch is open).

	LEDs are connected to Pins PC0 PC1 PC2 PC3 (active low). 

	

	The program causes  rotate a single lighted LED through the row of four LEDs and then repeat the pattern indefinitely.
	If the switch is pressed, pause the cycling pattern and turn on all LEDs until the switch is released. 


*/

//** GLOBAL VARIABLES **/

long freqclk = 16000000; // clock frequency in Hz



//** FUNCTION DECLARATIONS **//

void wait(volatile int N);  // Note that variables in the delay loops are declared as volatile.  If not, the compiler will likely

// see these as not necessary since they are not used for any important tasks or I/O and will eliminate the loops.  In fact, if

// the simulator is used without these being declared volatile, then the loops will be ignored all together in the simulation,

// and the cycle counter will not grow correctly.

void delay_1_msec_raw_count();



#include <avr/io.h>

int main(void)

{

	 	

	DDRC = 0xFF;  // set bits PD7-PD5 as output, PD4-PD0 as input (this covers Pins 3 and 4 as needed for inputs, and 6 and 7 as needed for outputs)

	PORTC = 0xFF; // set all bits on PORTC- so that all output are off (assumes circuit is wired as active low)

		 

    // ============================================

    // P R O G R A M L O O P

    // ============================================
while(1){	

		PORTC = PORTC ^ 1 << PORTC0;
		wait(10); // debounce delay
		PORTC = PORTC ^ 1 << PORTC0;	

	PORTC = 0xFF; // set all bits on PORTC- so that all output are off (assumes circuit is wired as active low)
	
		while (PIND & 0B0000100)
		{
			PORTC = PORTC & 0B11110000;
		}
	
		PORTC = PORTC ^ 1 << PORTC1;	// an individual bit without changing the other bits in  PORTC
		wait(10); // debounce delay
		PORTC = PORTC ^ 1 << PORTC1;	// an individual bit without changing the other bits in  PORTC

		PORTC = 0xFF; // set all bits on PORTC- so that all output are off (assumes circuit is wired as active low)
	
		while (PIND & 0B00000100)
		{
			 PORTC = PORTC & 0B11110000;
		}
		

		PORTC = PORTC ^ 1 << PORTC2;	// an individual bit without changing the other bits in  PORTC
		wait(10); // debounce delay
		PORTC = PORTC ^ 1 << PORTC2;	// an individual bit without changing the other bits in  PORTC
				
		PORTC = 0xFF; // set all bits on PORTC- so that all output are off (assumes circuit is wired as active low)
	
		while (PIND & 0B00000100)
		{ 
			PORTC = PORTC & 0B11110000;
		}

		PORTC = PORTC ^ 1 << PORTC3;	// an individual bit without changing the other bits in  PORTC
		wait(10); // debounce delay
		PORTC = PORTC ^ 1 << PORTC3;	// an individual bit without changing the other bits in  PORTC
				
		PORTC = 0xFF; // set all bits on PORTC- so that all output are off (assumes circuit is wired as active low)
	
		while (PIND & 0B00000100)
		{ 
			PORTC = PORTC & 0B11110000;
		}		

	}

}

// ============================================

// S U B R O U T I N E S

// ============================================

void wait(volatile int N) {

	//*** wait ***

		

	/* This subroutine creates a delay of N msec by calling another function.

			Inputs: N = number of msec for the delay (N is int, so can be up to 65,536 msec)

			Outputs: None

			

		Why is N a volatile variable? Some C compilers try to optimize the code and may eliminate code that appears to do nothing.  A time-killing 

		loop is is one of those code blocks that could be removed during compile.  Using a nonvolatile variable in the function tells the compiler not 

		remove it.

	*/

	while (N>1) {  // This loop creates an N msec delay

		N--;

		delay_1_msec_raw_count();

	}

} // end wait()



void delay_1_msec_raw_count() {

    //*** delay N ms **

    /* This subroutine creates a delay of N msec by using a raw time-killing count.

			Inputs: None

			Outputs: None

	*/

	volatile int count = (freqclk/1000-28)/4; // each cycle takes 4 clock cycles, so count is the number of cycles needed to create a 1 msec delay.

									// 28 is the approximate number of other clock cycles used in the subroutine

	while (count > 1) {  // This loop is expected to create a 1 msec delay, but it is in fact much longer.  Because of the size of the variables,

			// there is a lot more that needs to be done each cycle so the loop takes much longer than 1 msec to execute. To know exact time for each 

			// instruction, use assembly language instead of C. (Or look at the disassembly code file and count instructions.)

			count--; // decrement count

	}

	

		

} // end delay_1_msec_raw_count()
