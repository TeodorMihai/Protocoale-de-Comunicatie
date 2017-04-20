all: client server

client: client.o link_emulator/lib.o
	gcc -g -m32 client.o link_emulator/lib.o -o client

server: server.o link_emulator/lib.o
	gcc -g -m32 server.o link_emulator/lib.o -o server

.c.o: 
	gcc -Wall -g -c -m32 $? 

clean:
	rm -f server.o server
