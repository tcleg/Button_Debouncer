//*********************************************************************************
// Button Debouncer
// 
// Description: Debounces buttons on a single port being used by the application.
// This module takes what the signal on a GPIO port is doing and removes
// the oscillations caused by a bouncing button and tells the application if
// the button(s) are debounced.
// 
// The debouncing algorithm used in this library is based partly on Jack
// Ganssle's state button debouncer example shown in,
// "A Guide to Debouncing" Rev. 3.
//
// An explanation of the concepts expressed in this library can be found here:
// http://www.ganssle.com/debouncing.htm
// 
// Copyright (C) 2014 Trent Cleghorn
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, andór sell
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
#include <stdbool.h>

//*********************************************************************************
// Macros and Globals
//*********************************************************************************

// MAX_BUTTON_CHECKS should be greater than 0 and less than or equal to 255.
// 10 is a roundabout good number of checks to perform before considering a button 
// debounced.
// If this number is large, the Debouncer instantiation will consume more RAM and
// take more time to debounce but will reduce the chance of having an incorrectly
// debounced button.
// If this is small, the Debouncer instantiation will consume less RAM but will 
// be more prone to incorrectly determining button presses and releases.
#ifndef MAX_BUTTON_CHECKS
#define MAX_BUTTON_CHECKS       10
#endif

// Specifies whether pullups or pulldowns are being used for the buttons.
// If using pulldowns, define USING_BUTTON_PULLUPS to be false.
#ifndef USING_BUTTON_PULLUPS
#define USING_BUTTON_PULLUPS    true
#endif  

//*********************************************************************************
// Class
//*********************************************************************************

class 
Debouncer
{
    public:
        // 
        // Constructor
        // 
        Debouncer();
        
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
        // Check Button Press
        // Description:
        //      Checks to see if a button was pressed. The application should wait
        //      until a button is released before re-enabling any disabled button
        //      interrupts (if the button pins have interrupts attached to them).
        // Parameters:
        //      GPIOButtonPin - The particular bit on the port that needs to be 
        //                      read.
        // Returns:
        //      True if the button was pressed. False if it was not.
        // 
        bool ButtonPressed(uint8_t GPIOButtonPin);
        
        // 
        // Check Button Release
        // Description:
        //      Checks to see if a button was released. The application should wait
        //      until the button is released before re-enabling any disabled button
        //      interrupts (if the button pins have interrupts attached to them).
        // Parameters:
        //      GPIOButtonPin - The particular bit on the port that needs to be 
        //                      read.
        // Returns:
        //      True if the button was released. False if it was not.
        // 
        bool ButtonReleased(uint8_t GPIOButtonPin);
        
        // 
        // Button Debounced State Get
        // Description:
        //      Gets the currently debounced state of the port pins.
        // Parameters:
        //      None.
        // Returns:
        //      The port pins the are currently being debounced.
        // 
        uint8_t ButtonDebouncedStateGet();
        
    private:
        // 
        // Holds the states that the particular port is transitioning through
        // 
        uint8_t state[MAX_BUTTON_CHECKS];
        
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
};

#endif  // BUTTON_DEBOUNCER_H
