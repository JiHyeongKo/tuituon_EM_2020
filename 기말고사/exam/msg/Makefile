all: button.c button.h
	arm-linux-gnueabi-gcc -o button.o button.c -c
	arm-linux-gnueabi-ar rc libexam2.a button.o
	arm-linux-gnueabi-gcc -o test main.c -pthread -l exam2 -L.

clean:
	rm *.o *.a test