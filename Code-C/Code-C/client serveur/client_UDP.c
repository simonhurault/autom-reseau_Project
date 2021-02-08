#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

struct mesg {

long id;
double position[1];
double control[1];

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
message.position[0]=0.0;
message.control[0]=0;


fcntl(serveur,F_SETFL,fcntl(serveur,F_GETFL) | O_NONBLOCK); 

do{

message.id++;
printf("send : \n  id=%d \n",message.id);
sendto(serveur,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));

while(recvfrom(serveur,&answer,sizeof(answer), 0,(struct sockaddr*)&sockAddr,&addr) == -1);

printf("recieve : \n  id=%d \n",answer.id);

}while(message.id<100.0);

close(serveur);

return 0;

}



