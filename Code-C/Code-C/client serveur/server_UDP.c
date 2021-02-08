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

struct mesg message;
int result, nsend;
struct sockaddr_in sockAddr, sock;
int serveur, client, err, nConnect, longaddr;
int n , i, results, resultr ;
long int  Te;
double  Un,  Sn,  Snp , K;
double tau, dt, A, B;
int Rt;  

  serveur=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    sockAddr.sin_family=PF_INET;
    sockAddr.sin_port=htons(2500); 
    sockAddr.sin_addr.s_addr=0;
 	longaddr=sizeof(sockAddr);

	err=bind(serveur,(struct sockaddr*)&sockAddr,longaddr);
 	if(err==ERROR) {
        printf("\n erreur de bind du serveur UDP!! \n");
	}

message.id=0.0;
message.position[0]=0.0;
message.control[0]=0.0;


fcntl(serveur,F_SETFL,fcntl(serveur,F_GETFL) | O_NONBLOCK); 
 
while(1)
{


if(recvfrom(serveur,&message,sizeof(message), 0,(struct sockaddr*)&sockAddr,&longaddr) != -1)
{	
	printf("receive : \n id=%d \n",message.id);
	sendto(serveur,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
}

}


close(serveur);

return 0;

}

