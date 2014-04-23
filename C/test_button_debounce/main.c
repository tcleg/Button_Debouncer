//*********************************************************************************
// Button Debouncer Example Using the MSP430G2 Launchpad Evaluation Board with
// the MSP430G2553 Microcontroller, Written in C Using Code Composer Studio
// 
// Description: The button located on port 1 pin 3 has an interrupt placed
// on it and after being pressed, loads the debouncer and performs the debouncing
// operation until the button has been released and then returns to a low power
// state until the button is pressed again. As long as the button is held down,
// two LEDs will be on. Otherwise, they are off.
// 
// Copyright (C) 2014 Trent Cleghorn
// 
// Email: trentoncleghorn@gmail.com
// 
//                                  MIT License
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//*********************************************************************************

//*********************************************************************************
// Headers
//*********************************************************************************
#include "common_msp.h" 
#include "button_debounce.h"

//*********************************************************************************
// Macros and Globals
//*********************************************************************************
#define BUTTON      P1_3
#define RED_LED     P1_0
#define GREEN_LED   P1_6

tDebouncer port1;

//*********************************************************************************
// Function Prototypes
//*********************************************************************************
void setup();
void loop();

//*********************************************************************************
// Main
//*********************************************************************************
int 
main() {

    // Stop the watchdog timer (WDT)
    WDTCTL = WDTPW | WDTHOLD;
	
    setup();
    
    while(1)
    {
        loop();
    }
}

//*********************************************************************************
// Interrupts
//*********************************************************************************

// Button Interrupt
#pragma vector=PORT1_VECTOR
__interrupt void
Port1(void)
{
    // If the button was pressed, exit the low power mode
    // and disable button interrupts. Else, reset the flag.
    if (P1IFG & BUTTON)
    {
        LPM_EXIT();
        P1IE &= ~BUTTON;
        P1IFG &= ~BUTTON;
    } 
    else
    {
        P1IFG = 0;
    }
}

// The Watchdog Timer interrupts the system every half second and forces it out
// of Low Power Mode 0
#pragma vector=WDT_VECTOR
__interrupt void 
Counter_and_Debouncer(void)
{   
    LPM_EXIT();
    
    // WDT interrupt flag automatically clears
}

//*********************************************************************************
// Functions
//*********************************************************************************
void
setup()
{
    // Preliminary check to see if the clocking calibration constants stored
    // in flash memory segment A have been erased. If that flash segment has
    // been erased, the calibration constants will read all 1's
    // Hex 0xFF = Binary 11111111
    // If they have been erased, the microcontroller will need them reprogrammed
    // back into segment A.
    if (CALBC1_1MHZ == 0xFF)
    {                                                   
        while(1);
    }
    
    // DCO = MCLK = SMCLK, set DCO to 1MHZ
    DCOCTL  = 0;                                                            
    BCSCTL1 = CALBC1_1MHZ;                                                
    DCOCTL  = CALDCO_1MHZ;
    
    // Set Watchdog timer to interval mode and about a 0.5ms interrupt interval
    // This will also be the debouncing interval.
    WDTCTL = WDT_MDLY_0_5;            
    IE1   |= WDTIE;
    
    // Setup GPIO
    // P1.3 set to input with pullup and generates an interrupt on a falling
    // edge. All other port pins are set to output and are set LOW.
    // Port 2 is set to output and LOW as well.
    P1OUT  = BUTTON;                                                            
    P1DIR |= ~BUTTON;
    P2OUT  = 0x00;
    P2DIR  = 0xFF;
    P1REN |= BUTTON;                                                        
    P1IES |= BUTTON;
    P1IFG &= ~BUTTON;
    P1IE  |= BUTTON;
    
    // Setup the debouncer. Tell the debouncer which buttons are being 
    // pulled up.
    ButtonDebounceInit(&port1, BUTTON);
    
    // Wait in Low Power Mode 4 until the button is pressed.
    LPM_4_ENTER();
}

void
loop()
{
    static uint8_t buttonPressed = false;
    
    // If the button interrupt is disabled... 
    if(!(P1IE & BUTTON) && (buttonPressed == false))
    {
        buttonPressed = true;
        
        // Load the debouncer
        ButtonProcess(&port1, ~BUTTON);
    }
    else
    {
        // P1IN -> Port 1 Inputs
        ButtonProcess(&port1, P1IN);
    }
    
    // Check for button press and release.
    if(ButtonPressed(&port1, BUTTON))
    {
        // Toggle the LEDs on and wait until the WDT wakes up the processor
        // to continue.
        P1OUT ^= RED_LED | GREEN_LED;
        LPM_0_ENTER();
    }
    else if(ButtonReleased(&port1, BUTTON))
    {
        // Toggle the LEDs off and temporarily disable interrupts before
        // re-enabling the button interrupt since if the interrupt
        // occurs right before LPM4 is entered the processor will eternally
        // sleep
        P1OUT ^= RED_LED | GREEN_LED;
        buttonPressed = false;
        INTERRUPT_DISABLE();
        P1IE |= BUTTON;
        LPM_4_ENTER();
    }
    else
    {
        // Wait until the processor is woken up again to continue debouncing the
        // buttons
        LPM_0_ENTER();
    }
}
