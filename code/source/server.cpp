#include<bits/stdc++.h>
/*#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include<stdlib.h>*/
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include<time.h>
#include <ifaddrs.h>
#include<fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <dirent.h>

#define sa struct sockaddr_in
#define  maxlen 100
#define listen_queue 50
#define u_int unsigned int
#define server_path "./server_files/"

#include "header.h"
using namespace std;


void dataConnectionSend(sa clientAddress,int client_socket, unsigned long int port, char* filename){
	sa clientAddress2;

	// creating data socket for data connection 
	int client_data_socket= socket(AF_INET,SOCK_STREAM,0);
	if(client_data_socket==-1){
		cout<<"Data Socket cannot be opened\n";
		return ;
	}

 	bzero(&clientAddress2, sizeof(clientAddress2));

	clientAddress2.sin_family = AF_INET;
	clientAddress2.sin_addr.s_addr = clientAddress.sin_addr.s_addr;
	clientAddress2.sin_port = port;
	
	// sending connect request to the client from the server
	int status_connect = connect(client_data_socket,(struct sockaddr*) &clientAddress2,sizeof(clientAddress2));
	
	if(status_connect<0){
		cout<<"Connection Error\n";
		return ;
	}
//printf("Connection Established\n");
	int filehandle,size; 
	char file[maxlen];
	struct stat object;
	strcpy(file,server_path);
	strcat(file,filename);
	stat(file,&object);
	filehandle = open(file,O_RDONLY);
	size = object.st_size;
	char data[10];
	memset(data,0,sizeof(data));
	//if file doesnt exist
	if(filehandle == -1){
		size = 0;
		sprintf(data,"%d",size);
		
		send(client_socket,data,10,0);
	}
else{
		int ow = 0;
		sprintf(data,"%d",size);
		//sending the size of the file
		send(client_socket,data,10,0);
		// receiving the overwrite option 
		recv(client_socket,&ow,sizeof(int),0);
		if(ow == 1){
			sendfile(client_data_socket,filehandle,NULL,size);
		}
	}
	close(client_data_socket);
}



int main()
{
int login_check=0;
	int serverSocketHandler = socket(AF_INET , SOCK_STREAM , 0);
	//creating a socket and assigning it to the socket handler
	if(serverSocketHandler < 0)
        {
       // socket methode return -1 if the creation was not successful
	cout<< "Socket creation has failed.\n";
		return 0;
	    } 
           cout<<"Socket Created.\n";
	struct sockaddr_in serverAddr , clientAddr;
	//specifying address and type for the server to operate under
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int bindStatus = bind(serverSocketHandler , (struct sockaddr*) & serverAddr , sizeof(serverAddr));
	if(bindStatus < 0)
    {
		cout<<"Socket binding has failed\n";
		return 0;
	}
      cout<<"Socket Binding Succesfull\n"; 
	//listen to the client while others are waiting in queue of size 5
	int listenStatus = listen(serverSocketHandler , client_listen );
	if(listenStatus < 0)
        {	// when queue is full listen fails
		cout<< "Listner has failed\n";
		return 0;
        }
cout<<"Listening\n";

cout<<"...Waiting for connections... \n";
   
sa clientAddress;
	char buffer[maxlen],command[5], filename[20]; //initiating variables for data buffer,command buffer,file buffer
	unsigned long int PORT = 0;
	int filehandle; 
	char socket_buffer[maxlen];
	memset(buffer,0,maxlen);
	memset(socket_buffer,0,maxlen);
	socklen_t length = sizeof(sa);
	printf("server waiting to accept\n");

	//accepting the request from the client
	int client_socket = accept(serverSocketHandler,(struct sockaddr*) &clientAddress,&length);


	if (client_socket < 0){
		cout<<"Error in accepting socket\n";
   	}	
	

recv(client_socket,&login_check,sizeof(int),0);
 if(login_check==1){
cout<<"accepted\n";
cout<<"logined\n";

char buffer[maxlen],command[5], filename[20]; //initiating variables for data buffer,command buffer,file buffer
	unsigned long int PORT = 0;
	int filehandle; 
	char socket_buffer[maxlen];
	memset(buffer,0,maxlen);
	memset(socket_buffer,0,maxlen);
	socklen_t length = sizeof(sa);
while(1){

		// receiving file name or extension
		recv(client_socket,filename,20,0);
		printf("Received file name : %s\n",filename);
		// receiving control information
		recv(client_socket,buffer,maxlen,0);
                int i=0 ,j;
		int m = 0;
		for(i = 0; buffer[i]!=':';i++){
			command[m++] = buffer[i];
		}
		cout<<"Received command :"<<command<<endl;
		int k = 0;
		for(j = i+1;buffer[j]!='#';j++){
			socket_buffer[k++] = buffer[j];
		}
		//printf("New data connection port : %s\n",socket_buffer);
                if(!strcmp(command,"GET")){

			//recv(client_socket,socket_buffer,sizeof(socket_buffer),0);
			
			unsigned long int PORT = atoi(socket_buffer);
			dataConnectionSend(clientAddress,client_socket,PORT,filename);
	  		memset(buffer,0,maxlen);
	  		memset(socket_buffer,0,maxlen);
		}

		


		



}
}
else cout<<"not logined\n";
 return 0;
}

