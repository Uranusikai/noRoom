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





//** FUNCTION DECLARATIONS **//




void wait(volatile int multiple, volatile char time_choice); // Note that variables in the delay loops are declared as volatile.  If not, the compiler will likely



// see these as not necessary since they are not used for any important tasks or I/O and will eliminate the loops.  In fact, if



// the simulator is used without these being declared volatile, then the loops will be ignored all together in the simulation,



// and the cycle counter will not grow correctly.



void delay_T_msec_timer0(volatile char choice) ;







#include <avr/io.h>



int main(void)



{



	 	



	DDRC = 0xFF;  // Sets all pins of PORTC to output

	PORTC = 0xFF; // Turn off all pins on PORTC (assume pins are configured as active low)

		 



    // ============================================



    // P R O G R A M L O O P



    // ============================================

while(1)

	{	

	


	

		PORTC = PORTC ^ 1 << PORTC0;	// an individual bit without changing the other bits in  PC0

		wait(100,2); // call delay function: the wait function will loop 200 time using delay #2 (1 msec each loop) for a total of 200 msec delay

		PORTC = PORTC ^ 1 << PORTC0;	// an individual bit without changing the other bits in  PC0



		while (PIND & 0B00000100 ^ 0B00000100)	// Checking if PD2, the switch, is close.
 
		{

			PORTC = PORTC & 0B11110000;	// turn on all lights, which are from PC0 - PC3 

		}

	

		PORTC = 0xFF;// Turn off all pins on PORTC (assume pins are configured as active low)

	

		PORTC = PORTC ^ 1 << PORTC1;	// an individual bit without changing the other bits in  PC1

		wait(100,2); // call delay function: the wait function will loop 200 time using delay #2 (1 msec each loop) for a total of 200 msec delay

		PORTC = PORTC ^ 1 << PORTC1;	// an individual bit without changing the other bits in  PC1





		while (PIND & 0B00000100 ^ 0B00000100)	// Checking if PD2, the switch, is close.

		{

			PORTC = PORTC & 0B11110000;	// turn on all lights, which are from PC0 - PC3 

		}

		

		PORTC = 0xFF; // Turn off all pins on PORTC (assume pins are configured as active low)

	

		PORTC = PORTC ^ 1 << PORTC2;	// an individual bit without changing the other bits in  PC2

		wait(100,2); // call delay function: the wait function will loop 200 time using delay #2 (1 msec each loop) for a total of 200 msec delay

		PORTC = PORTC ^ 1 << PORTC2;	// an individual bit without changing the other bits in  PC2

				

		while (PIND & 0B00000100 ^0B00000100 )	// Checking if PD2, the switch, is close.

		{

			PORTC = PORTC & 0B11110000;	// turn on all lights, which are from PC0 - PC3 

		}


		PORTC = 0xFF; // Turn off all pins on PORTC (assume pins are configured as active low)


		PORTC = PORTC ^ 1 << PORTC3;	// an individual bit without changing the other bits in  PC3

		wait(100,2); // call delay function: the wait function will loop 200 time using delay #2 (1 msec each loop) for a total of 200 msec delay

		PORTC = PORTC ^ 1 << PORTC3;	// an individual bit without changing the other bits in  PC3

				
	

		while (PIND & 0B00000100 ^0B00000100)	// Checking if PD2, the switch, is close.

		{

			PORTC = PORTC & 0B11110000;	// turn on all lights, which are from PC0 - PC3 

		}		
		
		PORTC = 0xFF; // Turn off all pins on PORTC (assume pins are configured as active low)




	}


}



// ============================================



// S U B R O U T I N E S



// ============================================



void wait(volatile int multiple, volatile char time_choice) {

	/* This subroutine calls others to create a delay.

		 Total delay = multiple*T, where T is in msec and is the delay created by the called function.

	

		Inputs: multiple = number of multiples to delay, where multiple is the number of times an actual delay loop is called.

		Outputs: None

	*/

	

	while (multiple > 0) {

		delay_T_msec_timer0(time_choice); 

		multiple--;

	}

} // end wait()



void delay_T_msec_timer0(volatile char choice) {

    //*** delay T ms **

    /* This subroutine creates a delay of T msec using TIMER0 with prescaler on clock, where, for a 16MHz clock:

    		for Choice = 1: T = 0.125 msec for prescaler set to 8 and count of 250 (preload counter with 5)

    		for Choice = 2: T = 1 msec for prescaler set to 64 and count of 250 (preload counter with 5)

    		for Choice = 3: T = 4 msec for prescaler set to 256 and count of 250 (preload counter with 5)

    		for Choice = 4: T = 16 msec for prescaler set to 1,024 and count of 250 (preload counter with 5)

			for Choice = Default: T = .0156 msec for no prescaler and count of 250 (preload counter with 5)

	

			Inputs: None

			Outputs: None

	*/

	

	TCCR0A = 0x00; // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.

	TCNT0 = 0;  // preload value for testing on count = 250

	// preload value for alternate test on while loop: TCNT0 = 5;  // preload load TIMER0  (count must reach 255-5 = 250)

	

	switch ( choice ) { // choose prescaler

		case 1:

			TCCR0B = 0b00000010; //1<<CS01;	TCCR0B = 0x02; // Start TIMER0, Normal mode, crystal clock, prescaler = 8

		break;

		case 2:

			TCCR0B =  0b00000011; //1<<CS01 | 1<<CS00;	TCCR0B = 0x03;  // Start TIMER0, Normal mode, crystal clock, prescaler = 64

		break;

		case 3:

			TCCR0B = 0b00000100; //1<<CS02;	TCCR0B = 0x04; // Start TIMER0, Normal mode, crystal clock, prescaler = 256

		break; 

		case 4:

			TCCR0B = 0b00000101; //1<<CS02 | 1<<CS00; TCCR0B = 0x05; // Start TIMER0, Normal mode, crystal clock, prescaler = 1024

		break;

		default:

			TCCR0B = 0b00000001; //1<<CS00; TCCR0B = 0x01; Start TIMER0, Normal mode, crystal clock, no prescaler

		break;

	}

	

	while (TCNT0 < 0xFA); // exits when count = 250 (requires preload of 0 to make count = 250)

	// alternate test on while loop: while ((TIFR0 & (0x1<<TOV0)) == 0); // wait for TOV0 to roll over:

	// How does this while loop work?? See notes

	

	TCCR0B = 0x00; // Stop TIMER0

	//TIFR0 = 0x1<<TOV0;  // Alternate while loop: Clear TOV0 (note that this is a nonintuitive bit in that it is cleared by writing a 1 to it)

	

} // end delay_T_msec_timer0()
