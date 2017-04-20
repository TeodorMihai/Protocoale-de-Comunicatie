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

int main(int argc,char** argv){

  init(HOST,PORT);
  msg pkt;

  sprintf(pkt.payload,"Hello World of PC");
  pkt.len = strlen(pkt.payload)+1;

  for(i = 0 ; i < pkt.len; ++i) {

  	char c = pkt.payload[i];
  	int mask = (1 << 7);
  	char par = 0;

  	for(j = 0 ; j < 8 ; ++j) {
  		par = par ^ (c & mask);
  		mask >>= 1;
  	}		

  	pkt.payload[pkt.len + i] = par;
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
