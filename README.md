Button Debouncer
=====================================

Platform Independent

I implemented Jack Ganssle's State Button Debouncer Algorithm as C/C++ platform independent 
libraries. This is a fairly robust algorithm that can play nicely with interrupts (if the 
library is set up to be used that way) and can also work with setups where the button pins 
are polled on a regular interval instead.
 
The library can debounce buttons on an 8 bit port in parallel and is efficient in that it 
only requires 14 bytes of RAM per instantiation, is malleable in that the amount of RAM 
consumed by each instantiation can be reduced if desired, and uses no computationally 
expensive operations such as multiplication and division to perform the debouncing.  If 
you would like a detailed explanation of the theory behind the algorithm, feel free to 
follow the link provided below: 
 
http://www.ganssle.com/debouncing.htm

This C/C++ coded library can be used in any application requiring a Software Button Debouncer. 
No matter the platform (Arduino/AVR, PIC, MSP430, Kinetis, STM32, etc...), this library can 
be applied with absolutely no changing of the underlying implementation of the code. Include 
this library in any C/C++ project as needed.

All of the documentation for how to use the libraries can be found inside the header
files.
