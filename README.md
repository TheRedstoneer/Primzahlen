# Primzahlen
## hardcore efficient prime number counter

This project was a challenge against my professor. The goal is to create the fastest/most efficient prime number counter. You just have to input an upper limit (sth big, like 999,999,999) and the program will count all prime numbers below.

### Usage
compile it for your specific system by typing `make`.

now run it: `Primzahlen <MAX NUMBER> <THREAD COUNT>`.\
(The thread count is currently limited to 32 threads, as most systems have at most 16 (real) cores.)

### STILL WIP
The program is still work in progress (if I want to waste my time with this...) and it's not 100% accurate D: (I _guess_ it has something to do with the inverse popcount, but I wasn't able to fix it yet.) The Error is +-1 for every thread, so at most 32 off.

### Timings
The current version takes about **0.7sec for 999,999,999 Numbers** to count. You will find the binary solution from my professor (called fast), which takes over 3secs for the same task.
