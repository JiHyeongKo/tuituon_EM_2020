objects  = one.o

all: five.so one.o
	gcc -o four.elf five.so one.o

five.so: two.c three.h
	gcc -o five.so -fPIC two.c -c
	
$(objects): one.c three.h
	$(CC) -o $@ -c $<

clean:
	rm *.elf *.o *.so
