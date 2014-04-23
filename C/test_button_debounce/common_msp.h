//*********************************************************************************
// common_msp.h - Common MSP430 Includes and Definitions
// 
// Description: Makes some aspects of the MSP430 easier to code.
//
// Copyright (C) 2014 Trent Cleghorn
// 
//                                  MIT License
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
#ifndef COMMON_MSP_H
#define COMMON_MSP_H

//*********************************************************************************
// Headers
//*********************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <msp430.h>


// 
// C Binding for C++ Compilers
// 
#ifdef __cplusplus
extern "C"
{
#endif

//*********************************************************************************
// Compiler Intrinsics
//*********************************************************************************

//
// Enter Low-Power Modes with Interrupts Enabled
//
#define LPM_0_ENTER()           _BIS_SR(LPM0_bits + GIE)
#define LPM_1_ENTER()           _BIS_SR(LPM1_bits + GIE)
#define LPM_2_ENTER()           _BIS_SR(LPM2_bits + GIE)
#define LPM_3_ENTER()           _BIS_SR(LPM3_bits + GIE)
#define LPM_4_ENTER()           _BIS_SR(LPM4_bits + GIE)

//
// Exit Low-Power Mode
// Exits LPM4,LPM3,LPM2,LPM1,and LPM0
//
#define LPM_EXIT()              _BIC_SR_IRQ(LPM4_bits)

//
// Delay
// Delays the processor for the specified number of clock cycles.
// Minimum values is 0 and maximum value is 2^32
//
#define DELAY_CYCLES(x)         __delay_cycles(x)           

// 
// Swap Bytes
// Swaps the high byte and low byte of a 16 bit integer and returns the result.
// Example: 0x1234 becomes 0x3412
// 
#define SWAP_BYTES(x)           __swap_bytes(x)            

// 
// Enable and Disable Interrupts
//
#define INTERRUPT_ENABLE()      __enable_interrupt()
#define INTERRUPT_DISABLE()     __disable_interrupt()
                                                            
// 
// Acquire Interrupt States
// Returns the GIE (General Interrupt Enable) bit, which is bit 3
//                                     
#define INTERRUPT_STATE_GET()   (__get_interrupt_state() & GIE)     
                                                            
//*********************************************************************************
// Generic Macro Functions
//*********************************************************************************

// 
// Unit Circle
// 
#define PI                              3.1415926f
#define HALF_PI                         1.5707963f
#define TWO_PI                          6.2831853f
#define RADIANS(deg)                    ((deg) * 0.0174532f)
#define DEGREES(rad)                    ((rad) * 57.2957795f)

// 
// Simple Math Functions
// 

// Squares or cubes a number
#define SQRE(a) 		                ((a) * (a))
#define CUBE(a)                         ((a) * (a) * (a))

// Finds the minimum or maximum between two numbers
#define MIN(a,b)                        ((a) < (b) ? (a) : (b))
#define MAX(a,b)                        ((a) > (b) ? (a) : (b))

// Constrain a value between an upper and lower limit
#define CONSTRAIN(a,lower,upper)        ((a) < (lower) ? (lower) : \
                                        ((a) > (upper) ? (upper) : \
                                         (a)))

// Round a floating point number                                         
#define ROUND_F(a)                      ((a) >= 0.0f ? (int32_t)((a) + 0.5f) : \
                                                       (int32_t)((a) - 0.5f))

// Absolute value of a number                                                       
#define ABSOLUTE(x)                     ((a) < 0 ? ((a) * (-1)) : (a))

//*********************************************************************************
// Watchdog Timer
//*********************************************************************************
 	
    //  Watchdog Timer Periods
	//  16MHz       Period
	//  /64     ->  0.004ms
	//  /512    ->  0.032ms
	//  /8192   ->  0.512ms
	//  /32768  ->  2.048ms
            
	//  1MHz        Period
	//  /64     ->  0.064ms
	//  /512    ->  0.512ms
	//  /8192   ->  8.192ms
	//  /32768  ->  32.768ms
            
	//  32.768kHz   Period
	//  /64     ->  1.9ms
	//  /512    ->  16ms
	//  /8192   ->  250ms
	//  /32768  ->  1000ms 
            
	//  12kHz       Period
	//  /64     ->  5.333ms
	//  /512    ->  42.666ms
	//  /8192   ->  682.666ms
	//  /32768  ->  2730.666ms 

// For Watchdog Timer Interval Mode
#define WDT_SMCK_32768 		WDT_MDLY_32   
#define WDT_SMCK_8192 		WDT_MDLY_8    
#define WDT_SMCK_512 		WDT_MDLY_0_5  
#define WDT_SMCK_64 		WDT_MDLY_0_064
#define	WDT_ACK_32768 		WDT_ADLY_1000 
#define	WDT_ACK_8192 		WDT_ADLY_250  
#define	WDT_ACK_512 		WDT_ADLY_16   
#define WDT_ACK_64 	    	WDT_ADLY_1_9 

//*********************************************************************************
// Port Pins
//*********************************************************************************

#define P1_0                BIT0
#define P1_1                BIT1
#define P1_2                BIT2
#define P1_3                BIT3
#define P1_4                BIT4
#define P1_5                BIT5
#define P1_6                BIT6
#define P1_7                BIT7
            
#define P2_0                BIT0
#define P2_1                BIT1
#define P2_2                BIT2
#define P2_3                BIT3
#define P2_4                BIT4
#define P2_5                BIT5
#define P2_6                BIT6
#define P2_7                BIT7
            
#define P3_0                BIT0
#define P3_1                BIT1
#define P3_2                BIT2
#define P3_3                BIT3
#define P3_4                BIT4
#define P3_5                BIT5
#define P3_6                BIT6
#define P3_7                BIT7

// 
// End of C Binding
// 
#ifdef __cplusplus
}
#endif

#endif  // COMMON_MSP_H
