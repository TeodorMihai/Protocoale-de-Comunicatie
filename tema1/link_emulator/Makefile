all: link lib.o

link: link.o queue.o
	gcc -g -m32 link.o queue.o -o link -pthread

.c.o: 
	gcc -Wall -g -c -m32 $? -pthread

clean:
	rm -f *.o link
