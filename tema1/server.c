#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

#define simplu  0
#define ack 	1
#define parity  2
#define hamming 3

#define ACK 	1
#define NACK    2

int receive_message(msg*, int);
int receive_mess_simplu(msg* , int);
int receive_mess_parity(msg* , int);
int receive_mess_ack(msg*, int);
int receive_mess_hamming(msg*, int);
int send_mess(msg, int);
int send_mess_simplu(msg, int);
int send_mess_ack(msg, int);
int send_mess_parity(msg, int);
int send_mess_hamming(msg, int);
int rec_ack(int);
int send_ack(int, int);
void setBite(char*, int, int);
int getBite(int, char, char);
void changeBite(int, char*, char*);
void construct_hamming(msg*);
char construct_nr(char, char);
void corectare(msg*);
char compute_parity_byte(msg*);
int binary_search(msg*, int);
int set_mode(int, char**, int*);

int rec_ack(int mode) {

	msg r;

	int res = recv_message(&r);

	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}

	if(mode == parity) {

		if(r.len == 4)
			return ACK;

		if(r.len == 5)
			return NACK;
	}

	if(mode == hamming) {

		if(r.len == 3)
			return ACK;
		if(r.len == 4)
			return NACK;
	}
	
	return 1;
}


int send_ack(int x, int mode) {

	msg r; int res;

	if(mode == parity) {

		if(x == ACK) 
			r.len = sprintf(r.payload, "xACK");
		 else 
			r.len = sprintf(r.payload, "xNACK");
		
		res = send_message(&r);

		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}

		return 1;
	}

	//petru toate celelalte trimit doar ack

	r.len = sprintf(r.payload, "ACK");

	if(r.len < 0) {
		perror("[RECEIVER] Error at creating ACK message. Exiting.\n");
		return -1;
	}

    res = send_message(&r);

	if (res < 0) {
		perror("[RECEIVER] Send ACK error. Exiting.\n");
		return -1;
	}

	return 1;
}

void setBite(char* x, int pos, int val) {

	//seteaza al pos-lea bit (de la stanga la dreapta) al lui x cu valoarea val
	if(val)
		*x = (*x) | (1 << (8 - pos));
	else 
		*x = (*x) & ( 0XFF - (1 << (8 - pos) ) );
}

int getBite(int pos, char frst, char scd) {

	//al pos-lea bit din concaternarea frst scd
	if(pos > 8) {
		pos -= 8;
		return ((scd & (1 << (8 - pos))) != 0);
	} else 
		return ((frst & (1 << (8 - pos) ) ) != 0);
}

void changeBite(int pos, char* frst, char* scd) {
	/* schimba al pos-lea bit din cuplul frst,scd */
	if(pos > 8) {
		pos -= 8;
		*scd = (*scd) ^ ( 1 << (8 - pos) );
	} else 
		*frst = (*frst) ^ (1 << (8 - pos) );
}


void construct_hamming(msg* r) {

	/* construiesc hamming pentru un intreg mesaj */

	int i;int j;

	int dim = 2 * r->len;

	//construire format, mai intai pui creez un vector 
	//cu dimensiunile specificate, adica practic inserez un caracter din 2 in 2
	//si pozitionez bitii D la locul lor

	for( i = r->len - 1; i >= 0 ; --i) {

		char x = r->payload[i];
		char frst = 0;
		char scd = 0;

		setBite(&frst, 7, x & (1 << 7));

		int nth = 2;

		for( j = 1; j <= 8 ; ++j) {
			
			if(j == 4) continue;
			
			setBite(&scd, j, x & ( 1 << (8 - nth) ) );

			nth++;
		}

		r->payload[dim - 1] = scd;
		r->payload[dim - 2] = frst;

		dim -= 2;
	}

	int k;
	
	//construire mesaj
	//calculez aici si sindromul (bitii de paritate) si ii pun la locul lor

	for(i = 0; i < 2 * r->len; i += 2) {

		for(j = 1; j <= 4; ++j) {

			char p = 0;
			int mask = (1 << (j - 1));

			for(k = 1; k <= 12 ; ++k) {
				
				if( (k & mask) && (k != mask) )
					p = p ^ getBite(k + 4, r->payload[i], r->payload[i + 1]);
			}

			if(j == 4)
				setBite(&(r->payload[i + 1]), 4 , p);
			else 
				setBite(&(r->payload[i]), 4 + mask, p);
		}
	}

	r->len = 2 * r->len;
}


char construct_nr(char frst, char scd) {
	//un pic harcodat, era mai rapid sa hardocdez aici decat sa 
	//incerc sa modularizez ceva :(
	return  (getBite(7, frst, scd) << 7)
		  + (getBite(9, frst, scd) << 6)
	      + (getBite(10, frst, scd) << 5)
	      + (getBite(11, frst, scd) << 4)
	      + (getBite(13, frst, scd) << 3)
	      + (getBite(14, frst, scd) << 2)
	      + (getBite(15, frst, scd) << 1)
	      + (getBite(16, frst, scd) << 0);
}


void corectare(msg* r) {

	//schimbam mai intai bitii necesari
	int i; int j; int k;

	for(i = 0 ; i < r->len; i += 2) {

		int pos = 0;//pozitia bitului modificat in urma transmiterii

		for(j = 1; j <= 4; ++j) {

			int mask = 1 << (j - 1);
			char p = 0;

			for(k = 1; k <= 12; ++k) 
				if(k & mask)
					p = p ^ getBite(k + 4, r->payload[i], r->payload[i + 1]);
			if(p) 
				pos += mask;
		}

		if(pos)
			changeBite(pos + 4, &(r->payload[i]), &(r->payload[i + 1]));
	}

	//reconstruim numarul

	for(i = 0 ; i < r->len; i += 2) 
		r->payload[i/2] = construct_nr(r->payload[i], r->payload[i + 1]);

	r->len = r->len / 2;
}

int receive_mess_simplu(msg* r, int mode) {

	int res = recv_message(r);

	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}

	return 1;
}

int receive_mess_ack(msg* r, int mode) {

	int res = recv_message(r);

	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}

	if(send_ack(ACK, mode) < 0)
		return -1;
	else 
		return 1;
}

char compute_parity_byte(msg* r) {

	//calculez paritatea unui mesaj

	int i; int j;
	char par = 0;

  	for(i = 1 ; i < r->len; ++i) {

   	 	char c = r->payload[i];
    	int mask = (1<<7);

   		for(j = 0 ; j < 8 ; ++j) {

		    par = par ^ ((mask & c) != 0);
		    mask >>= 1;
	    }
	}

	return par;
}

int receive_mess_parity(msg* r, int mode) {

	int res = recv_message(r);

	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}

	char par = compute_parity_byte(r);
	
	if( par != (r->payload[0] & 1) ) {
		if(send_ack(NACK, mode) < 0)
			return -1;

		receive_mess_parity(r, mode);
		return 1;
	}

	strcpy(r->payload , r->payload + 1);
	r->len--;

	if(send_ack(ACK, mode) < 0)
		return -1;

	return 1;
}

int receive_mess_hamming(msg* r, int mode) {

	int res = recv_message(r);

	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}

	corectare(r);

	printf("%s\n", r->payload);

	if(send_ack(ACK, mode) < 0)
		return -1;

	return 1;
}

int receive_message(msg* r, int mode) {

	switch(mode) {

		case simplu: return receive_mess_simplu(r, mode); break;
		case ack: return receive_mess_ack(r, mode); break;
		case parity: return receive_mess_parity(r, mode); break;
		case hamming: return receive_mess_hamming(r, mode); break;
	}

	return -1;

}

int send_mess_simplu(msg r, int mode) {

	int res = send_message(&r);

	if (res < 0) {
		perror("[RECEIVER] Send error. Exiting.\n");
		return -1;
	}

	return 1;
}

int send_mess_ack(msg r, int mode) {

	int res = send_message(&r);

	if (res < 0) {
		perror("[RECEIVER] Send error. Exiting.\n");
		return -1;
	}
	
	if(rec_ack(mode) < 0) 
		return -1;
	return 1;
}

int send_mess_parity(msg r, int mode) {

	if(r.len >= MSGSIZE) {

		perror("[RECEIVER] Message too long. Exiting.\n");
		return -1;
	}

	int i;
	//fac loc pentru primul caracter
	for(i = r.len ; i > 0; --i)
		r.payload[i] = r.payload[i - 1];

	r.len++;
	r.payload[0] = compute_parity_byte(&r);
	//trimit mesajul
	int res = send_message(&r);		

	if (res < 0) {
		perror("[RECEIVER] Send error. Exiting.\n");
		return -1;
	}
	
	//primesc ack
	if( (res = rec_ack(mode)) < 0) {
		perror("[RECEIVER] Eroare primire ack. Exiting. \n");
		return -1;
	}

	//daca primesc nack mai trimit odata acelasi mesaj, pentru ca inseman ca a fost corupt
	while(res == NACK) {

		res = send_message(&r);

		if (res < 0) {
			perror("[RECEIVER] Send error. Exiting.\n");
			return -1;
		}
	
		if( (res = rec_ack(mode)) < 0) {
			perror("[RECEIVER] Eroare primire ack. Exiting. \n");
			return -1;
		}

	} 

	return 1;
}

int send_mess_hamming(msg r, int mode) {

	if(r.len > MSGSIZE / 2) {
		perror("[RECEIVER] Mesagge too long. Exiting.\n");
		return -1;
	}

	construct_hamming(&r);
	corectare(&r);
	construct_hamming(&r);

	int res = send_message(&r);

	if (res < 0) {
		perror("[RECEIVER] Send error. Exiting.\n");
		return -1;
	}
	
	if(rec_ack(mode) < 0) 
		return -1;

	return 1;
}

int send_mess(msg r,int mode) {

	switch(mode) {

		case simplu: return send_mess_simplu(r, mode); break;
		case ack: return send_mess_ack(r, mode); break;
		case parity: return send_mess_parity(r, mode); break;
		case hamming: return send_mess_hamming(r, mode); break;
	}

	return 1;
}


int binary_search(msg* p, int mode) {

	//cautare binara pe biti 
	msg r;

	int pos; int step;

	for(step = 1 ; step <= 500; step <<= 1);

	for(pos = 0; step; step >>= 1) {

		int nr_to_send = pos + step;

		if(nr_to_send >= 1000) 
			nr_to_send = 999;

		p->len = sprintf(p->payload, "%d", nr_to_send) + 1;

		if( send_mess(*p, mode) < 0 ) 			return -1;

		if(receive_message(&r, mode) < 0)		return -1;

		char first_char = r.payload[0];
		char second_char = r.payload[1];

		if(first_char == 'b' )  
			pos = pos + step;
		else if(second_char == 'u')
				return 1;
	}

	return 0;
}


int set_mode(int argc, char* argv[], int *mode) {

	//setez modul de interactioanre, parity ack etc
	if(argc == 1) {
		*mode = simplu;
		return 1;
	} else {

		if(strcmp(argv[1], "ack") == 0)		 {
			*mode = ack;
			return 1;
		}

		if(strcmp(argv[1], "parity") == 0)  {
			 *mode = parity;
			 return 1;
		}

		if(strcmp(argv[1], "hamming") == 0) {
			*mode = hamming;
			return 1;
		}

		*mode = 0;

		return 1;
	}

	*mode = 0;

	perror("[RECEIVER] Invalid mode. Exit.\n");

	return -1;
}


int main(int argc, char *argv[])  {

	int mode;

	if( set_mode(argc, argv, &mode) < 0) 	return -1;

	msg r;	
	
	printf("[RECEIVER] Starting.\n");

	init(HOST, PORT);

	if(receive_message(&r, mode) < 0) 		return -1;

	r.len = sprintf(r.payload, "Hello");
	if(send_mess(r, mode) < 0)			 	return -1;

	if(receive_message(&r, mode) < 0)		return -1;
	
	if(receive_message(&r, mode) < 0)		return -1;

	if(receive_message(&r, mode) < 0)		return -1;

	r.len = sprintf(r.payload, "YEY");
	if(send_mess(r, mode) < 0)				return -1;

	r.len = sprintf(r.payload, "OK");
	if(send_mess(r, mode) < 0)				return -1;

	if(receive_message(&r, mode) < 0)		return -1;

	msg p;

	if(binary_search(&r, mode) == 0)		return -1;

	p.len = sprintf(p.payload, "exit");
	
	if(receive_message(&r, mode) < 0)		return -1;

	if( send_mess(p, mode) < 0 ) 		    return -1;


	printf("[RECEIVER] Finished receiving..\n");

	return 0;
}
