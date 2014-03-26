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

//*********************************************************************************
// Headers
//*********************************************************************************
#include "button_debounce.h"

//*********************************************************************************
// Functions
//*********************************************************************************
Debouncer::
Debouncer()
{
    uint8_t i;
    
    index = 0;
    debouncedState = 0xFF;
    changed = 0x00;
    
    // Initialize the state array
    for(i = 0; i < MAX_BUTTON_CHECKS; i++)
    {
        state[i] = 0xFF;
    }
}

void Debouncer::
ButtonProcess(uint8_t portStatus)
{
    uint8_t i;
    uint8_t lastDebouncedState = debouncedState;
    
    if(USING_BUTTON_PULLUPS == false)
    {
        portStatus = ~portStatus;
    }
    
    // Save the port status info into the state array
    state[index] = portStatus;
    
    // Debounce the buttons
    for(i = 0, debouncedState = 0xFF; i < MAX_BUTTON_CHECKS; i++)
    {
        debouncedState = debouncedState & state[i];
    }
    
    // Check to make sure the index hasn't gone over the limit
    index++;
    if(index >= MAX_BUTTON_CHECKS)
    {
        index = 0;
    }
    
    // Calculate what changed
    // If the switch was high and is now low, 1 and 0 xORed with
    // each other produces a 1. If the switch was low and is now
    // high, 0 and 1 xORed with each other produces a 1. Otherwise,
    // it is 0
    changed = debouncedState ^ lastDebouncedState;
}

bool Debouncer::
ButtonPressed(uint8_t GPIOButtonPin)
{
    // If the button changed and it changed to a 1, then the
    // user just pressed it.
    if((changed & (~debouncedState)) & GPIOButtonPin)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Debouncer:: 
ButtonReleased(uint8_t GPIOButtonPin)
{
    // If the button changed and it changed to a 0, then the
    // user just released the button.
    if((changed & debouncedState) & GPIOButtonPin)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint8_t Debouncer::
ButtonDebouncedStateGet()
{
    if(USING_BUTTON_PULLUPS == true)
    {
        return debouncedState;
    }
    else
    {  
        return ~debouncedState;
    }
}

