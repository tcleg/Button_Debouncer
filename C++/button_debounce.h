//*********************************************************************************
// State Button Debouncer - Platform Independent
// 
// Revision: 1.4
// 
// Description: Debounces buttons on a single port being used by the application.
// This module takes what the signal on a GPIO port is doing and removes
// the oscillations caused by a bouncing button and tells the application if
// the button(s) are debounced. A benefit of this algorithm is that it can play
// nicely with button interrupts. Below is an example of how the button debouncer
// would work in practice in relation to a single button:
// 
// Real Signal:     00110000000000000001110000000000000000001111111000000000000000
// Bouncy Signal:   00101110000000000001101010000000000000001010101101000000000000
// Debounced Sig:   00111111111111111001111111111111111000001111111111111111111100
// 
// The debouncing algorithm used in this library is based partly on Jack
// Ganssle's state button debouncer example shown in, "A Guide to Debouncing" 
// Rev 4. http://www.ganssle.com/debouncing.htm
// 
// Revisions can be found here:
// https://github.com/tcleg
// 
// Copyright (C) 2014 Trent Cleghorn , <trentoncleghorn@gmail.com>
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

// 
// Header Guard
// 
#ifndef BUTTON_DEBOUNCER_H
#define BUTTON_DEBOUNCER_H

//*********************************************************************************
// Headers
//*********************************************************************************
#include <stdint.h>

//*********************************************************************************
// Macros and Globals
//*********************************************************************************

// NUM_BUTTON_STATES should be greater than 0 and less than or equal to 255.
// 10 is a roundabout good number of states to have. At a practical minimum, the
// the number of button states should be at least 3. Each button state consumes
// 1 byte of RAM.
// If this number is large, the Debouncer instantiation will consume 
// more RAM and take more time to debounce but will reduce the chance of having an 
// incorrectly debounced button. If this is small, the Debouncer instantiation 
// will consume less RAM and take less time to debounce but will be more prone 
// to incorrectly determining button presses and releases.
#ifndef NUM_BUTTON_STATES
#define NUM_BUTTON_STATES       10
#endif
                                			// Binary Equivalent
#define BUTTON_PIN_0            (0x0001)	// 0b00000001		
#define BUTTON_PIN_1            (0x0002)	// 0b00000010
#define BUTTON_PIN_2            (0x0004)	// 0b00000100
#define BUTTON_PIN_3            (0x0008)	// 0b00001000
#define BUTTON_PIN_4            (0x0010)	// 0b00010000
#define BUTTON_PIN_5            (0x0020)	// 0b00100000
#define BUTTON_PIN_6            (0x0040)	// 0b01000000
#define BUTTON_PIN_7            (0x0080)	// 0b10000000

//*********************************************************************************
// Class
//*********************************************************************************

class 
Debouncer
{
    public:
        // 
        // Constructor
        // Description:
        //      Initializes the Debouncer instantiation. 
        // Parameters:
        //      pulledUpButtons - 
        //              Specifies whether pullups or pulldowns are being used on the
        //              port pins. This is the ORed BUTTON_PIN_* 's that are being
        //              pulled up. Otherwise, the debouncer assumes that the other
        //              buttons are being pulled down. A 0 bit means pulldown.
        //              A 1 bit means pullup. For example, 00010001 means that
        //              button 0 and button 4 are both being pulled up. All other
        //              buttons have pulldowns if they represent buttons.
        // Returns:
        //      None
        // 
        Debouncer(uint8_t pulledUpButtons);
        
        // 
        // Button Debouncer Process
        // Description:
        //      Does the calculations on debouncing the buttons on a particular
        //      port. This function should be called on a regular interval by the
        //      application such as every 1 to 10 milliseconds. 
        // Parameters:
        //      portStatus - The particular port's status expressed as one 8 bit 
        //                   byte.
        // Returns:
        //      None
        // Note:
        //      A good alternative way to use this function is to set up interrupts 
        //      for the buttons on a particular port and have this function called 
        //      after a button is pressed (and interrupts are temporarily disabled 
        //      for that button) then pass the initial state of the buttons and set 
        //      this function to be called on a regular interval thereafter until 
        //      the button(s) are released. The particular button's interrupt is 
        //      then re-enabled. With this method, computation time spent in 
        //      constantly checking the buttons when none have been pressed goes 
        //      unwasted.
        // 
        void ButtonProcess(uint8_t portStatus);
        
        // 
        // Button Pressed
        // Description:
        //      Checks to see if a button(s) were pressed. 
        // Parameters:
        //      GPIOButtonPins - The particular bits corresponding to the button pins.
        //                       The ORed combination of BUTTON_PIN_*.
        // Returns:
        //      The port pin buttons that have just been pressed. For example, if
        //      (BUTTON_PIN_5 | BUTTON_PIN_0) is passed as a parameter for 
        //      GPIOButtonPins and if the byte that is returned expressed in binary is 
        //      00000001, it means that button 0 (bit 0) has just been pressed while
        //      button 5 (bit 5) has not been at the moment though it may have been
        //      previously.
        // Note:
        //      The application should wait until a button is released before 
        //      re-enabling any disabled button interrupts (if the button pins have 
        //      interrupts attached to them).
        // 
        uint8_t ButtonPressed(uint8_t GPIOButtonPins);
        
        // 
        // Button Released
        // Description:
        //      Checks to see if a button(s) were released. 
        // Parameters:
        //      GPIOButtonPins - The particular bits corresponding to the button pins.
        //                       The ORed combination of BUTTON_PIN_*.
        // Returns:
        //      The port pin buttons that have just been released. For example, if
        //      (BUTTON_PIN_5 | BUTTON_PIN_0) is passed as a parameter for 
        //      GPIOButtonPins and if the byte that is returned expressed in binary is 
        //      00000001, it means that button 0 (bit 0) has just been released while
        //      button 5 (bit 5) has not been at the moment though it may have been
        //      previously.
        // Note:
        //      The application should wait until a button is released before 
        //      re-enabling any disabled button interrupts (if the button pins have 
        //      interrupts attached to them).
        // 
        uint8_t ButtonReleased(uint8_t GPIOButtonPins);
        
        // 
        // Button Debounce State Get
        // Description:
        //      Gets the currently debounced state of the port pins.
        // Parameters:
        //      GPIOButtonPins - The particular bits corresponding to the button pins.
        //                       The ORed combination of BUTTON_PIN_*.
        // Returns:
        //      The port pins the are currently being debounced. For example, if
        //      (BUTTON_PIN_5 | BUTTON_PIN_1) is passed as a parameter for 
        //      GPIOButtonPins and if this function returns 00100000 in binary then 
        //      button 1 (bit 1) is not currently being pressed and button 5 (bit 5) 
        //      is currently being pressed while the other buttons (if they are 
        //      buttons) are being masked out.
        // 
        uint8_t ButtonDebounceStateGet(uint8_t GPIOButtonPins);
        
    private:
        // 
        // Holds the states that the particular port is transitioning through
        // 
        uint8_t state[NUM_BUTTON_STATES];
        
        // 
        // Keeps up with where to store the next port info in the state array
        // 
        uint8_t index;
        
        // 
        // The currently debounced state of the pins
        // 
        uint8_t debouncedState;
        
        // 
        // The pins that just changed debounced state
        // 
        uint8_t changed;
        
        // 
        // Pullups or pulldowns are being used 
        // 
        uint8_t pullType;
};

#endif  // BUTTON_DEBOUNCER_H
