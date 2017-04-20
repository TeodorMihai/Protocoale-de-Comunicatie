

client : client.cpp
	g++ -std=c++11 -fmax-errors=1 client.cpp -o client
server : server.cpp
	g++ -std=c++11 -fmax-errors=1 server.cpp -o server

run_client :
	./client 127.0.0.1 1222
run_server :
	./server 1222 users.txt shares.txt