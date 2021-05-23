all: Primzahlen

Primzahlen: control.o calculator.o
	gcc -o Primzahlen control.o calculator.o -lpthread -lm

control.o: control.c control.h
	gcc -c -O3 -march="native" control.c -Wall

calculator.o: calculator.c control.h
	gcc -c -O3 -march="native" calculator.c -Wall

clean:
		rm -f Primzahlen *.o

new: clean Primzahlen
