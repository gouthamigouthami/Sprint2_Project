#include<iostream>
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
using namespace std;
#define sa struct sockaddr_in
#define  maxlen 100
#define listen_queue 50
#define u_int unsigned int
#define server_path "./server_files/"


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
void dataConnectionSend_mget(int client_data_socket,sa clientAddress,int client_socket, unsigned long int port, char* filename){
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

}
void dataConnectionReceive(sa clientAddress,int client_socket, unsigned long int port, char* filename){
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
	int size,status,filehandle;
	char *receiveFile;
	char file[maxlen];
	strcpy(file,server_path);
	strcat(file,filename);
	int exists, overwrite;
	//checking if the file already exists or not 
	if( access( file, F_OK ) != -1){
        exists = 1;
        send(client_socket,&exists,sizeof(int),0);              
    }else{
        exists = 0;
        send(client_socket,&exists,sizeof(int),0);            
    }
    recv(client_socket,&overwrite,sizeof(int),0);  //receving overwrite option 
	char data[10];
	memset(data,0,sizeof(data));
	// Taking appropriate action according to the overwrite value 
    if(overwrite == 1)
    {
    	if(exists == 1)
    		filehandle = open(file, O_WRONLY | O_CREAT | O_TRUNC, 644);     
    	else
    		filehandle = open(file, O_CREAT | O_EXCL | O_WRONLY, 666);     

    	// receving size of the file
		recv(client_socket,data,10,0);
		size = atoi(data);
		// receving the file and sending status
        receiveFile = (char*)malloc(size*sizeof(char));
        recv(client_data_socket, receiveFile, size, 0);			
        status = write(filehandle, receiveFile, size);	
        close(filehandle);
        send(client_socket, &status, sizeof(int), 0);		
    }

    close(client_data_socket);
}

void getMyIP(char ip[]){  // Function to get the IP.
	/* Variables to Store the IP address of the Machine on which SERVER is running. */
	struct ifaddrs * ipAddrstr=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ipAddrstr);

    for (ifa = ipAddrstr; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if(strncmp(ifa->ifa_name,"eth0",3)==0){
            	strcpy(ip,addressBuffer);
            }
        }
    }
}

int main(int argc, char *argv[])
{
	int login_check=0;
	sa serverAddress;
	// creating a socket for the server (Control socket for control connection)
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	if(server_socket == -1){ 
		cout<<"Error in Opening the Listening Port.\n";
		return 0;
	}
	//initiating IP address for server 
      

    //unsigned long int IP = inet_addr(ip);   
    memset(&serverAddress,0, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port =htons(8000);

	// binding the Port and control socket

	int status_bind = bind(server_socket,(struct sockaddr*) &serverAddress,sizeof(struct sockaddr_in));

	if(status_bind == -1){
		cout<<"Socket Bind Error\n";
		return 0;
	}

//	
	//printf("Server started on IP: < %s > at Port: < %s >\n",ip,argv[1]);

	//server is listening(i.e waiting for the client to send request)

	int status_listen = listen(server_socket,listen_queue);
	if(status_listen == -1){
		cout<<"Listening error at the PORT\n";
		return 0;
	}



	sa clientAddress;
	char buffer[maxlen],command[5], filename[20]; //initiating variables for data buffer,command buffer,file buffer
	unsigned long int PORT = 0;
	int filehandle; 
	char socket_buffer[maxlen];
	memset(buffer,0,maxlen);
	memset(socket_buffer,0,maxlen);
	socklen_t length = sizeof(sa);
	cout<<"server waiting to accept\n";

	//accepting the request from the client
	int client_socket = accept(server_socket,(struct sockaddr*) &clientAddress,&length);


	if (client_socket < 0){
		cout<<"Error in accepting socket\n";
   	}
	cout<<"accepted\n";
	 recv(client_socket, &login_check, sizeof(int), 0);

	if(login_check==1){
	while(1){

		// receiving file name or extension
		recv(client_socket,filename,20,0);
		cout<<"Received file name : "<<filename<<"\n";
		// receiving control information
		recv(client_socket,buffer,maxlen,0);
		
		int i=0 ,j;
		int m = 0;
		for(i = 0; buffer[i]!=':';i++){
			command[m++] = buffer[i];
		}
		command[m]='\0';
		cout<<"Received command : "<<command<<endl;
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
		else if(!strcmp(command,"PUT")){
			unsigned long int PORT = atoi(socket_buffer);
			dataConnectionReceive(clientAddress,client_socket,PORT,filename);
			memset(buffer,0,maxlen);
			memset(socket_buffer,0,maxlen);
		}
		else if(!strcmp(command,"MGET")){


			unsigned long int PORT = atoi(socket_buffer);

			sa clientAddress2;

			// creating data socket for data connection
			int client_data_socket= socket(AF_INET,SOCK_STREAM,0);
			if(client_data_socket==-1){
				cout<<"Data Socket cannot be opened\n";
			}

		 	bzero(&clientAddress2, sizeof(clientAddress2));


			clientAddress2.sin_family = AF_INET;
			clientAddress2.sin_addr.s_addr = clientAddress.sin_addr.s_addr;
			clientAddress2.sin_port = PORT;
			
			// sending connect request to the client 
			int status_connect = connect(client_data_socket,(struct sockaddr*) &clientAddress2,sizeof(clientAddress2));
			
			if(status_connect<0){
				cout<<"Connection Error\n";
			}
			// traversing through the directory for the files with given extension 
			char *fname;
	        DIR *d;
	        struct dirent *dir;
	        d = opendir(server_path);
	        int ready=1;
			char newname[20];
			while ((dir = readdir(d)) != NULL){
				fname=dir->d_name;
				memset(newname,0,sizeof(newname));
	            strcpy(newname,fname);
				char* fextension = strrchr(newname, '.'); // getting the extension of the files
				if(fextension == NULL) continue;
				if (strcmp(filename,fextension)==0){   // comparing the externsion with the given extension
					cout<<"Sending" << newname<<"file\n";
	                send(client_socket, &ready, sizeof(int), 0);
	                send(client_socket,newname,20,0);
					dataConnectionSend_mget(client_data_socket,clientAddress,client_socket,PORT,newname);
				}
			}
			ready = 0;
			send(client_socket,&ready,sizeof(int),0);
			closedir(d);
			memset(buffer,0,sizeof(buffer));
			close(client_data_socket);
		}
		 else if(!strcmp(command, "QUIT"))
	        {
	            int status = 1;
	            send(client_socket, &i, sizeof(int), 0);           
	            break;
	        }

	}
	}
	else
		cout<<"\nLogin Failed\n";
	return 0;
}
