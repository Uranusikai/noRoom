/*
 * Mechatronics_Lab3.cpp
 *
 * Created: 2/5/2020 3:52:53 PM
 * Author : surface
 */ 


/* * * blink_using_raw_count_delay.c * * Created: 1/20/2018 10:59:37 AM * Author : WW Clark */
/* *
This program is meant to blink an LED at a set frequency determined by a raw count loop to kill time, but the time is incorrect. Since it is C code, the exact number of clock cycles used per instruction is not exactly known, so the times are approximate.
** Note that the loop takes much longer than expected because of many more instructions needed in assembly than expected.  See below.
Connect LED to PC0.
Connect LED signal to oscilloscope to test frequencies.
*
*/

#include <avr/io.h>
#include <avr/interrupt.h>

//** GLOBAL VARIABLES **/
int count_door1 = 0;
int count_door2 = 0;
volatile int i = 0;
int debounce = 600; // number of msec for debouncing the switches

//** Functions **//
void wait(volatile int multiple, volatile char time_choice); // Note that variables in the delay loops are declared as volatile.  If
// not, the compiler will likely
// see these as not necessary since they are not used for any important tasks or I/O and
// will eliminate the loops.  In fact, if
// the simulator is used without these being declared volatile, then the loops will be
// ignored all together in the simulation,
// and the cycle counter will not grow correctly.
void delay_T_msec_timer0(char choice);

//** GLOBAL VARIABLES **/
long freqclk = 16000000; // clock frequency in Hz


int main(void) {
	// Setup
	DDRD = 0x00; // set bits of PORTD as input (only need PD2 and PD3 as input for the interrupts)
	DDRC = 0xFF; // Sets all pins of PORTC to output
	PORTC = 0xFF; // Turn off all pins on PORTC (they are wired as active low)
	
	// Set up Interrupts
	EICRA = 1<<ISC01 | 1<<ISC00 | 1<<ISC11 | 1<<ISC10;	// Trigger INT0 and INT1 on rising edge
	EIMSK = 1<<INT1 | 1<<INT0;	// Enable INT1 and INT0
	sei();	//Enable Global Interrupt
	
	
	while(1) {
		
		PORTC = 0xFF; // Turn off all pins on PORTC (they are wired as active low)
		// The main loop -- toggle the bit, then delay to set the frequency
		PORTC = PORTC ^ 1<<PC0; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC
		wait(debounce,2); // call delay function to delay a certain number of msec
		
		PORTC = PORTC ^ 1<<PC0; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC		
		
		
		
		PORTC = 0xFF; // Turn off all pins on PORTC (they are wired as active low)
		PORTC = PORTC ^ 1<<PC1; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC
		wait(debounce,2); // call delay function to delay a certain number of msec
		
		PORTC = PORTC ^ 1<<PC1; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC
				
		
		
		PORTC = 0xFF; // Turn off all pins on PORTC (they are wired as active low)
		PORTC = PORTC ^ 1<<PC2; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC
		wait(debounce,2); // call delay function to delay a certain number of msec
		
		PORTC = PORTC ^ 1<<PC2; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC
		
		
		
		PORTC = 0xFF; // Turn off all pins on PORTC (they are wired as active low)
		PORTC = PORTC ^ 1<<PC3; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC
		wait(debounce,2); // call delay function to delay a certain number of msec
		
		PORTC = PORTC ^ 1<<PC3; // Note that this uses the Exclusive OR operator, which toggles
		//an individual bit without changing the other bits in PORTC
	}
} // end main()




//Interrupt Service Routine for INT0
ISR(INT0_vect) // This routine is entered if Switch 1 goes high (door 1 is opened)
{
	count_door1++; // Increment count on door opening
	
		int c = 0b00000000;
		PORTC = PORTC & 0xFF; //Clear
		wait(debounce,2);
		
		// a do-nothing statement to preserve the empty while loop
		for (int a = 1; a <= 16; a = a + 1)
		{
			PORTC = PORTC & ~c; // Turn on LED
			wait(debounce/2,2); // debounce the switch
			
			PORTC = PORTC | c;  // Turn off LED
			
			c = c + 1;
		}

	EIFR = EIFR | 1<<INTF0;  // Clear the interrupt flag in case it was reset while in the ISR (so that it doesn't run again)
	
}

ISR(INT1_vect)
{
	PORTC = PORTC & 0b11110000; // Turn on LED by clearing PC3 (which is active low),  leave other bits unchanged  (AND PORTC with a binary number that has 0 in the only bit that we want to ensure is 0)
	count_door2++; // Increment count on door opening
	wait(debounce*2,2); // debounce the switch
	
	PORTC = PORTC | 0b00001111;  // Turn off LED by setting bit PC3 (which is active low), leave other bits unchanged (OR PORTC with a binary number that has 1 in the only bit that we want to ensure is 1)
	wait(debounce,2); // debounce the switch
	EIFR = EIFR | 1<<INTF1;  // Clear the interrupt flag in case it was reset while in the ISR (so that it doesn't run again)
	
}




/* ============================================ S U B R O U T I N E S ============================================ */
void wait(volatile int multiple, volatile char time_choice) {
	//*** wait ***
	/* This subroutine calls others to create a delay.
		 Total delay = multiple*T, where T is in msec and is the delay created by the called function.
	
		Inputs: multiple = number of multiples to delay, where multiple is the number of times an actual delay loop is called.
		Outputs: None
	*/
	
	while (multiple > 0) {
		delay_T_msec_timer0(time_choice); // we are choosing case 2, which is a 1 msec delay
		multiple--;
	}
} // end wait()

void delay_T_msec_timer0(char choice) {
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