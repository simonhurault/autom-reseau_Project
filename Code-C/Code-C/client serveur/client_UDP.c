#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>


/// message send to VREP
struct mesg {
  long id;
  double position[1];
  double control[1];
  clock_t date;
};


#define ERROR (-1)

int main (int nba, char *arg[]) {
int result;
int nsend;
int nconnect;
struct mesg message, answer;
int addr;
long int Te;

struct sockaddr_in sockAddr, sock;
int serveur, client, err, nConnect, longaddr , results, resultr;


    serveur=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    sockAddr.sin_family=PF_INET;
    sockAddr.sin_port=htons(2000); 
    sockAddr.sin_addr.s_addr=inet_addr(arg[1]);
    addr=sizeof(sockAddr);

message.id=0.0;
message.position[0]=0;
message.control[0]=50;


fcntl(serveur,F_SETFL,fcntl(serveur,F_GETFL) | O_NONBLOCK); 

do{

  // Updating message data
  message.id++;
  message.date = clock();
  message.control[0] = -message.control[0];
  printf("sended : \n  id=%d \n",message.id);
  sendto(serveur,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));

  unsigned long timout = clock(); 
  while(recvfrom(serveur,&answer,sizeof(answer), 0,(struct sockaddr*)&sockAddr,&addr) == -1 && (clock() -  timout) * 1000 / CLOCKS_PER_SEC <= 10000);
  if((clock() -  timout) * 1000 / CLOCKS_PER_SEC <= 10000)
  {
    unsigned long time_spent = (clock() -  answer.date) * 1000 / CLOCKS_PER_SEC;
    printf("recieve : \n  id=%d and take a time of %ld\n",answer.id, time_spent);
  }
  else printf("packet loss\n");

}while(message.id<100.0);

close(serveur);

return 0;

}



