#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

unsigned i; unsigned j;

void setBit(int val, int pos, int offset, msg* pkt) {

	int mask = (1 << offset);
	if(val)
		pkt[pos] = pkt[pos] | mask
	else 
		pkt[pos] = pkt[pos] & (~mask);
}



void insert(int pos, msg* pkt) {

	pos = pos / 8;
	int offset = pos % 8;

	//shiftez toti bitii


}

int main(int argc,char** argv){

  init(HOST,PORT);
  msg pkt;

  sprintf(pkt.payload,"Hello World of PC");
  pkt.len = strlen(pkt.payload)+1;

  int k = 0;

  for( ; (1 << k) < (pkt.len * 8 + k + 1); ++k);

  printf("Dimensiune k: %d\n", k); 

  for(i = 0 ; i <= k ; ++i) {

  	 insert((1 << i) - 1, &msg);
  }

  for(i = 0 ; i <=k ; ++i) {

  	int mask = (1 << i);
  	char p = 0;

  	for(j = 0 ; j < pkt.len; ++j) {

  		if( j & mask )
  			p = p & pkt.payload[j];
  	}

  	pkt.payload[(1 << i) - 1] = p;
  }

  send_message(&pkt);

  if (recv_message(&pkt)<0){
    perror("receive error");
  }
  else {
    printf("[%s] Got reply with payload: %s\n",argv[0],pkt.payload);
  }

  return 0;
}
