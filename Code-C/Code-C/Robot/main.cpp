//
//   C++ - VREP API for manipulator Robotis H
//
//
//
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

// serveur

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

struct mesg {
  long id;
  double position[1];
  double control[1];
  clock_t date;

};

#define ERROR (-1)

// fin serveur

using namespace std;

extern "C" {
    #include "extApi.h"
}

int handles[6],all_ok=1;
simxInt handle, error;


void GetHandles(int clientID){
	simxChar objectName[100];
	char str[10];
    for (int i=0; i < 6; i++) {
        strcpy(objectName, "joint");
        sprintf(str, "%d", i+1);
        strcat(objectName,str);
        error=simxGetObjectHandle(clientID, objectName, &handle, simx_opmode_oneshot_wait);
        if (error == simx_return_ok)
            handles[i]=handle;
        else {
            printf("Error in Object Handle - joint number %d\n", i);
            all_ok=0;
        }
    }
}
/////////////////////////////////////////////////////////
// Set the join position
//
// Inputs:
//  clientID
//  q : array of the joint values
// Return: 0 if an error occurs in object handling, 1 otherwise
/////////////////////////////////////////////////////////
int SetJointVel(int clientID,  float *q)
{
    //simxChar objectName[100];
    //char str[10];
    //simxInt handle, error;
    //int all_ok=1;

    // Get the table of handles
    /*
    for (int i=0; i < 6; i++) {
        strcpy(objectName, "joint");
        sprintf(str, "%d", i+1);
        strcat(objectName,str);
        error=simxGetObjectHandle(clientID, objectName, &handle, simx_opmode_oneshot_wait);
        if (error == simx_return_ok)
            handles[i]=handle;
        else {
            printf("Error in Object Handle - joint number %d\n", i);
            all_ok=0;
        }
    }
    */
    if (all_ok) {
        //Pause the communication thread
        //simxPauseCommunication(clientID, 1);
        // Send the joint target positions
        for (int i=0; i < 6; i++)
            simxSetJointTargetVelocity(clientID, handles[i], q[i], simx_opmode_oneshot);
        // Resume the communication thread to update all values at the same time
        //simxPauseCommunication(clientID, 0);
        return 1;
    }
    else
        return 0;
}
int GetJointPos(int clientID,  float *q)
{
    //simxChar objectName[100];
    //char str[10];
    //simxInt handle, error;
    //int all_ok=1;
	int get[5];

    // Get the table of handles
    /*
    for (int i=0; i < 6; i++) {
        strcpy(objectName, "joint");
        sprintf(str, "%d", i+1);
        strcat(objectName,str);
        error=simxGetObjectHandle(clientID, objectName, &handle, simx_opmode_oneshot_wait);
        if (error == simx_return_ok)
            handles[i]=handle;
        else {
            printf("Error in Object Handle - joint number %d\n", i);
            all_ok=0;
        }
    }
    */
    if (all_ok) {
        //Pause the communication thread
        //simxPauseCommunication(clientID, 1);
        // Send the joint target positions
        for (int i=0; i < 6; i++)
            get[i]=simxGetJointPosition(clientID, handles[i], &q[i], simx_opmode_oneshot);
        // Resume the communication thread to update all values at the same time
        //simxPauseCommunication(clientID, 0);
        return get[0];
    }
    else
        return 0;
}

int main(int argc,char* argv[])
{
    int portNb=5555;            // the port number where to connect
    int timeOutInMs=5000;       // connection time-out in milliseconds (for the first connection)
    int commThreadCycleInMs=5;  // indicate how often data packets are sent back and forth - a default value of 5 is recommended
    int err;

    // Connection to the server
    int clientID=simxStart((simxChar*)"127.0.0.1",portNb,true,true,timeOutInMs,commThreadCycleInMs);

//    float q[7];
//    for (int i=0; i < 6; i++) q[i]=0.0;

    float q[6];
    float qr[6];
    GetHandles(clientID);
    for (int i=0; i < 6;i++)q[i]=0.0;



    //return(0);

	struct mesg message;
	int result, nsend;
	struct sockaddr_in sockAddr, sock;
	int serveur, client, nConnect, longaddr;
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

	message.id=0;
	message.position[0]=0.0;
	message.control[0]=0.0;


	Te=200000; // Te=100ms

	results=ERROR;
	resultr=ERROR;

	fcntl(serveur,F_SETFL,fcntl(serveur,F_GETFL) | O_NONBLOCK); 
	 
	do{

	usleep(Te); 




    if (clientID!=-1)
    {
       int nbloop=100;
       simxSynchronous(clientID,true);       // Enable the synchronous mode (Blocking function call)
       simxStartSimulation(clientID, simx_opmode_oneshot);

       float t=0.0;
       float tfinal=5;
       float dt=0.01;
       float q0m=0.5;
       float q1m=0.5;
       float q2m=0.6;
       float w=2*M_PI/2.5;
       
       int offsetTime=simxGetLastCmdTime(clientID)/1000;

       while (t < tfinal) {

			if(resultr=recvfrom(serveur,&message,sizeof(message), 0,(struct sockaddr*)&sockAddr,(socklen_t*)&longaddr) != -1)
			{


			printf("server : \n id=%d rt=%d rr=%d control=%lf\n",message.id, results, resultr, message.control[0]);
           printf("Current time: %6.4f\n", t);
           //GetJointPos(clientId,qr);
           q[0]=q0m*message.control[0];
           //if (t>2) q[0]=0.4;
           //else
           //q[0]=0;
           
           //q[1]=-q1m*sin(2*w*t);
           //q[2]=q2m*sin(4*w*t);
           SetJointVel(clientID, q);
           //simxSynchronousTrigger(clientID);
           t+=dt;
           usleep(dt*1000*1000);

			message.position[0] = GetJointPos(clientID, q);// On envoi pas la bonne chose
			printf("\n position=  %lf", q[0]);
			printf("\n test envoi =  %lf \n", message.position[0]);



			results=sendto(serveur,&message,sizeof(message),0,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
			}
        }

        simxStopSimulation(clientID, simx_opmode_oneshot);


        // Close the connection to the server
        simxFinish(clientID);
    }
    else
        printf("Connection to the server not possible\n");




	}while(message.id<100.0);


	close(serveur);

	return 0;


}
