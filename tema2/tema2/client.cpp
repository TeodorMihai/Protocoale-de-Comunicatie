#include <bits/stdc++.h>

using namespace std;

void parseConfig(int, char**, string& , int*);

void parseConfig(int argc, char* argv[], string& ip, int* port) {

	if(argc != 3) {
		char error[100];
		sprintf(error, "Apelare gresita a clientului. ./client ip_server port_server");
		perror(error);
		exit(EXIT_FAILURE);	
	}

	ip = argv[1];
	*port = atoi(argv[2]);
}
int main(int argc, char *argv[]) {

	string ip;
	int port;

	parseConfig(argc, argv, ip, &port);

	cout << ip << ' ' << port << '\n'; 
	return 0;
}