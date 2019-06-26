all: Primzahlen

Primzahlen: control.o calculator.o
	gcc -o Primzahlen control.o calculator.o -lpthread -lm

control.o: control.c control.h
	gcc -c -O3 -march="native" control.c

calculator.o: calculator.c control.h
	gcc -c -O3 -march="native" calculator.c

clean:
		rm -f Primzahlen *.o

new: clean Primzahlen
