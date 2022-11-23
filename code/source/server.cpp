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
#include "server_header.h"
using namespace std;
class server{
	public:
	

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
	char file[MAXLEN];
	struct stat object;
	strcpy(file,SERVER_PATH);
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
		
		send(client_socket,data,DATA_SIZE,0);
	}
	else{
		int ow = 0;
		sprintf(data,"%d",size);
		//sending the size of the file
		send(client_socket,data,DATA_SIZE,0);
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
	char file[MAXLEN];
	struct stat object;
	strcpy(file,SERVER_PATH);
	strcat(file,filename);
	stat(file,&object);
	filehandle = open(file,O_RDONLY);
	size = object.st_size;
	
	char data[DATA_SIZE];
	memset(data,0,sizeof(data));
	//if file doesnt exist	
	if(filehandle == -1){
		size = 0;
		sprintf(data,"%d",size);
		
		send(client_socket,data,DATA_SIZE,0);
	}
	else{
		int ow = 0;
		sprintf(data,"%d",size);
		//sending the size of the file
		send(client_socket,data,DATA_SIZE,0);
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
	char file[MAXLEN];
	strcpy(file,SERVER_PATH);
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
	char data[DATA_SIZE];
	memset(data,0,sizeof(data));
	// Taking appropriate action according to the overwrite value 
    if(overwrite == 1)
    {
    	if(exists == 1)
    		filehandle = open(file, O_WRONLY | O_CREAT | O_TRUNC, 644);     
    	else
    		filehandle = open(file, O_CREAT | O_EXCL | O_WRONLY, 666);     

    	// receving size of the file
		recv(client_socket,data,DATA_SIZE,0);
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
};

int main()
{
int login_check=0;
server OBJ;
	int server_socket = socket(AF_INET , SOCK_STREAM , 0);
	//creating a socket and assigning it to the socket handler
	if(server_socket < 0)
        {
       // socket methode return -1 if the creation was not successful
	cout<< "Socket creation has failed.";
		return 0;
	    } 
          cout<<"Socket Created.\n";
	struct sockaddr_in serverAddress , clientAddress;
	//specifying address and type for the server to operate under
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int bindStatus = bind(server_socket , (struct sockaddr*) & serverAddress , sizeof(serverAddress));
	if(bindStatus < 0)
    {
	cout<<"Socket binding has failed\n";
		return 0;
	}
      cout<<"Socket Binding Succesfull\n"; 
	//listen to the client while others are waiting in queue of size 5
	int listenStatus = listen(server_socket , CLIENT_LISTEN );
	if(listenStatus < 0)
        {	// when queue is full listen fails
		cout<< "Listner has failed\n";
		return 0;
        }
cout<<"Listening\n";
cout<<"...Waiting for connections... \n";
	
	int client_socket;
	socklen_t len = sizeof(clientAddress);
         char buffer[MAXLEN],command[COMMAND_SIZE], filename[FILE_SIZE]; //initiating variables for data buffer,command buffer,file buffer
	unsigned long int PORT = 0;
	int filehandle; 
	char socket_buffer[MAXLEN];
	memset(buffer,0,MAXLEN);
	memset(socket_buffer,0,MAXLEN);
	
	if((client_socket = accept(server_socket , (struct sockaddr*) & clientAddress , &len)) < 0)
    {
		cout<< "Server didn't accept the request.\n" ;
		return  0;
	}
	else
    {
		cout<< "Server accepted the request.\n " ;
	
	

recv(client_socket,&login_check,sizeof(int),0);
 if(login_check==1){
cout<<"logined\n";

while(1){

		// receiving file name or extension
		recv(client_socket,filename,FILE_SIZE,0);
		cout<<"Received file name : "<<filename<<"\n";
		// receiving control information
		recv(client_socket,buffer,MAXLEN,0);
		
		int cmd_read=0 ,read_;
		int store = 0;
		for(cmd_read = 0; buffer[cmd_read]!=':';cmd_read++){
			command[store++] = buffer[cmd_read];
		}
		command[store]='\0';
		cout<<"Received command : "<<command<<endl;
		int check_ = 0;
		for(read_ = cmd_read+1;buffer[read_]!='#';read_++){
			socket_buffer[check_++] = buffer[read_];
		}
		//printf("New data connection port : %s\n",socket_buffer);
		
		

		if(!strcmp(command,"GET")){

			//recv(client_socket,socket_buffer,sizeof(socket_buffer),0);
			
			unsigned long int PORT = atoi(socket_buffer);
			OBJ.dataConnectionSend(clientAddress,client_socket,PORT,filename);
	  		memset(buffer,0,MAXLEN);
	  		memset(socket_buffer,0,MAXLEN);
		}
		else if(!strcmp(command,"PUT")){
			unsigned long int PORT = atoi(socket_buffer);
			OBJ.dataConnectionReceive(clientAddress,client_socket,PORT,filename);
			memset(buffer,0,MAXLEN);
			memset(socket_buffer,0,MAXLEN);
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
	        d = opendir(SERVER_PATH);
	        int ready=1;
			char newname[FILE_SIZE];
			while ((dir = readdir(d)) != NULL){
				fname=dir->d_name;
				memset(newname,0,sizeof(newname));
	            strcpy(newname,fname);
				char* fextension = strrchr(newname, '.'); // getting the extension of the files
				if(fextension == NULL) continue;
				if (strcmp(filename,fextension)==0){   // comparing the externsion with the given extension
					cout<<"Sending" << newname<<"file\n";
	                send(client_socket, &ready, sizeof(int), 0);
	                send(client_socket,newname,FILE_SIZE,0);
					OBJ.dataConnectionSend_mget(client_data_socket,clientAddress,client_socket,PORT,newname);
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
		    cout<<"Quitting...."<<endl;
	            send(client_socket, &cmd_read, sizeof(int), 0);           
	            break;
	        }

	}

}
else{
cout<<"NOt loggined unable to proceed any file downloads\n";
}

}
 return 0;
}
