#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001


unsigned i; unsigned j;

int main(int argc,char** argv){
  
  msg p; msg t;
  init(HOST,PORT);

  if (recv_message(&p)<0){
    perror("Receive message");
    return -1;
  }

  int cnt = 0;

  for(i = 0 ; i < p.len; ++i) {

    char par = 0;
    char c = p.payload[i];
    int mask = (1<<7);

    for(j = 0 ; j < 8 ; ++j) {

        par = par ^ (mask & c);
        mask >>= 1;
    }

    if(par != p.payload[i + p.len])
        ++cnt;

  }

  printf(" Cate pachete difera: %d \n" , cnt); 
  printf("[%s] Got msg with payload: %s\n",argv[0],p.payload);

  sprintf(t.payload,"ACK(%s)",p.payload);
  t.len = strlen(t.payload+1);
  send_message(&t);
  return 0;
}
