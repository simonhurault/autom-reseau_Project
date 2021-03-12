#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>

struct mesg {
long id;
double position[1];
double control[1];
clock_t date;
};

#define ERROR (-1)

int main (int nba, char *arg[]) {

struct mesg rtt_message, vrep_message;
long rtt_frame_id = 0;
long vrep_frame_id = 0;
int result, nsend;
struct sockaddr_in sockAddr, sock;
int server, client, err, nConnect, longaddr, longaddr2;
int n , i, results, resultr ;
long int  Te;
double  Un,  Sn,  Snp , K;
double tau, dt, A, B;
int Rt;  


// Create server
  server=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    sockAddr.sin_family=PF_INET;
    sockAddr.sin_port=htons(2000); 
    sockAddr.sin_addr.s_addr=0;
 	longaddr=sizeof(sockAddr);

	err=bind(server,(struct sockaddr*)&sockAddr,longaddr);
 	if(err==ERROR) {
        printf("\n erreur de bind du server UDP!! \n");
	}


// Create client
client=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    sock.sin_family=PF_INET;
    sock.sin_port=htons(2500); 
    //sock.sin_addr.s_addr=inet_addr(arg[1]);
    longaddr2=sizeof(sock);





rtt_message.id=0;
rtt_message.position[0]=0.0;
rtt_message.control[0]=0.0;


vrep_message.id=0;
vrep_message.position[0]=0.0;
vrep_message.control[0]=0.0;


Te=3000000; // Te=2s



fcntl(server,F_SETFL,fcntl(server,F_GETFL) | O_NONBLOCK); 
fcntl(client,F_SETFL,fcntl(client,F_GETFL) | O_NONBLOCK);
 
/* Intializes random number generator */
srand((unsigned) time(NULL));
 
do{

recvfrom(server, &rtt_message,sizeof(rtt_message), 0,(struct sockaddr*)&sockAddr,&longaddr);
// make sure we don't send the packet twice
 if(rtt_frame_id != rtt_message.id)
 {
   int eject_packet = rand() % 100;
   rtt_frame_id = rtt_message.id;
   printf("rtt frame : \n id=%d \n",rtt_message.id);
   usleep(Te); // fake delay
   if(eject_packet <= 50)
   {
     sendto(client, &rtt_message, sizeof(rtt_message), 0, (struct sockaddr*)&sock, sizeof(sock)); // send frame to VREP server
   }
   else printf("ejected packet \n");
 }


 
 recvfrom(client, &vrep_message,sizeof(vrep_message), 0,(struct sockaddr*)&sock, &longaddr2);

 if(vrep_frame_id != vrep_message.id)
 {
     vrep_frame_id = vrep_message.id;
     printf("vrep frame : \n id=%d \n",vrep_message.id);
     sendto(server, &vrep_message, sizeof(vrep_message), 0, (struct sockaddr*)&sockAddr,sizeof(sockAddr));
	
 }

}while(rtt_message.id<100.0);


close(server);

return 0;

}


