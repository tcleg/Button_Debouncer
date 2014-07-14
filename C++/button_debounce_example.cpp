//*********************************************************************************
// Button Debouncer Example
// 
// Description: 
// Waits for a button press on port 1, pin 2 or port 1, pin 3 to toggle an LED. 
// Port 1, pin 2 has a pullup resistor while port 1, pin 3 does not.
// 
// This example cannot be compiled and used unless additional work
// is done to to so (coding wise). This example exists to show the general
// operation of the library it happens to utilize. It only displays one
// general use case although more use cases may exist.
// 
// Copyright (C) 2014 Trent Cleghorn <trentoncleghorn@gmail.com>
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

#include "button_debounce.h"

...

// Initialize the button debouncer. Tell the debouncer that
// pin 2 has a pullup.
Debouncer port1(BUTTON_PIN_2);

int
main()
{
    uint32_t currentTime;
    uint32_t initialTime;

    // Setup timer to generate an interrupt on a regular interval.
    // In this case, it will be every 1 millisecond.

    ...
    
    while(1)
    {
        ...
        
        // Check when 1 millisecond has passed
        if(currentTime - initialTime >= 1)
        {
            // Save the current time for the next go around
            initialTime = currentTime;
            
            // Port1ReadBits is a fictitious function that reads
            // the bits off the port 1 register.
            port1.ButtonProcess(Port1ReadBits());
            
            if(port1.ButtonPressed(BUTTON_PIN_2 | BUTTON_PIN_3))
            {
                // Toggle the LED on or off
                ...
            }
        }
    }
}



