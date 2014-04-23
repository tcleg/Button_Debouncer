Platform Independent Button Debouncer
=====================================

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

All of the documentation for how to use the libraries can be found inside the header
files.