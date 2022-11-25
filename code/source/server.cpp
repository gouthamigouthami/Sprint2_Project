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
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include<iostream>
#include "server_header.h"
class server {
	public:	
/** dataConnectionsend is for GET operation i.e receiving file from server to client **/
void dataConnectionSend(sa clientAddress,int client_socket, unsigned long int port, char* filename) {
	sa clientAddress2;
	// creating data socket for data connection 
	int client_data_socket = socket(AF_INET, SOCK_STREAM, PROTOCOL);
	if(client_data_socket == PROTOCOL) {
	  error("Data Socket cannot be opened");
		return;
	}
 	bzero(&clientAddress2, sizeof(clientAddress2));
	clientAddress2.sin_family = AF_INET;
	clientAddress2.sin_addr.s_addr = clientAddress.sin_addr.s_addr;
	clientAddress2.sin_port = port;
	// sending connect request to the client from the server
	int status_connect = connect(client_data_socket, (struct sockaddr*) &clientAddress2, sizeof(clientAddress2));
	if(status_connect < PROTOCOL) {
		error("Connection Error");
		return ;
	}
	int filehandle,size; 
	char file[MAXLEN];
	struct stat object;
	strcpy(file,SERVER_PATH); // copies server path into file
	strcat(file,filename); //concatenating both filename and serverpath
	stat(file,&object);
	filehandle = open(file,O_RDONLY); //opening a file for reading the filename
	size = object.st_size;
	char data[DATA_SIZE];
	memset(data,PROTOCOL,sizeof(data)); // used to clear the data
	// if file doesnt exist
	if(filehandle == FILEHANDLE) {
		size = false;
		sprintf(data,"%d",size);
		send(client_socket,data,DATA_SIZE,PROTOCOL);
	} else {

		int overwrite = false;
		sprintf(data,"%d",size);
		//sending the size of the file
		send(client_socket,data,DATA_SIZE,PROTOCOL);
		// receiving the overwrite option 
		recv(client_socket,&overwrite,sizeof(int),PROTOCOL);
		if(overwrite == true) {
			sendfile(client_data_socket,filehandle,NULL,size);
		}
	}
	//closing socket
	close(client_data_socket);
}
/** dataCnnectiontoSend for receving files from server to client **/
void dataConnectionSend_mget(int client_data_socket,sa clientAddress,int client_socket, unsigned long int port, char* filename) {
	int filehandle,size;
	char file[MAXLEN];
	struct stat object;
	strcpy(file,SERVER_PATH); // copies the serverpath into file
	strcat(file,filename); //concatenates both filename and path
	stat(file,&object);
	filehandle = open(file,O_RDONLY);
	size = object.st_size;
	char data[DATA_SIZE];
	memset(data,PROTOCOL,sizeof(data));
	//if file doesnt exist	
	if(filehandle == FILEHANDLE) {
		size = false;
		sprintf(data,"%d",size);
		send(client_socket,data,DATA_SIZE,PROTOCOL);
	} else {
		int overwrite = false;
		sprintf(data,"%d",size);
		//sending the size of the file
		send(client_socket,data,DATA_SIZE,PROTOCOL);
		// receiving the overwrite option
		recv(client_socket,&overwrite,sizeof(int),PROTOCOL);
		if(overwrite == true){
			sendfile(client_data_socket,filehandle,NULL,size);
		}
	}

}
	/* 
	*dataConnectionReceive is for PUT operation i.e receving files fro client*
	*/
void dataConnectionReceive(sa clientAddress,int client_socket, unsigned long int port, char* filename) {
	sa clientAddress2;
	// creating data socket for data connection 
	int client_data_socket = socket(AF_INET,SOCK_STREAM,PROTOCOL);
	if(client_data_socket == PROTOCOL){
		error("Data Socket cannot be opened");
		return ;
	}
 	bzero(&clientAddress2, sizeof(clientAddress2));
	clientAddress2.sin_family = AF_INET;
	clientAddress2.sin_addr.s_addr = clientAddress.sin_addr.s_addr;
	clientAddress2.sin_port = port;
	// sending connect request to the client from the server
	int status_connect = connect(client_data_socket,(struct sockaddr*) &clientAddress2,sizeof(clientAddress2));
	if(status_connect<PROTOCOL){ 
		error("Connection Error");
		return ;
	}
	int size,status,filehandle;
	char *receiveFile;
	char file[MAXLEN];
	strcpy(file,SERVER_PATH);
	strcat(file,filename);
	int exists, overwrite;
	// checking if the file already exists or not 
	if( access( file, F_OK ) != FILEHANDLE){
        exists = true;
        send(client_socket,&exists,sizeof(int),PROTOCOL);	
    } else { 
        exists = false;
        send(client_socket,&exists,sizeof(int),PROTOCOL);            
    }
    recv(client_socket,&overwrite,sizeof(int),PROTOCOL);  // receving overwrite option 
	char data[DATA_SIZE];
	memset(data,PROTOCOL,sizeof(data));
	// Taking appropriate action according to the overwrite value 
    if(overwrite == true)
    {
    	if(exists == true)
    		filehandle = open(file, O_WRONLY | O_CREAT | O_TRUNC, PERMISSION);     // opening a file for writing with the permission 644
    	else
    		filehandle = open(file, O_CREAT | O_EXCL | O_WRONLY, PERMISSION1);     // opening for not overwritting with permission 666

    	// receving size of the file
		recv(client_socket,data,DATA_SIZE,PROTOCOL);
		size = atoi(data);
		// receving the file and sending status
                receiveFile = (char*)malloc(size*sizeof(char));
                recv(client_data_socket, receiveFile, size, PROTOCOL);			
                status = write(filehandle, receiveFile, size);	
                close(filehandle);
                send(client_socket, &status, sizeof(int), PROTOCOL);		
    }

    close(client_data_socket);
}
};
int main()
{
	int login_check = false;
	server OBJ;
	int server_socket = socket(AF_INET , SOCK_STREAM , PROTOCOL);
	//creating a socket and assigning it to the socket handler
	if(server_socket < PROTOCOL)
        {
       // socket methode return -1 if the creation was not successful
	error("Socket creation has failed.");
		return 0;
	 }
         info("Socket Created.");
	 struct sockaddr_in serverAddress , clientAddress;
	//specifying address and type for the server to operate under
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT_1);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int bindStatus = bind(server_socket , (struct sockaddr*) & serverAddress , sizeof(serverAddress));
	if(bindStatus < PROTOCOL) {
		error("Socket binding has failed");
		return 0;
	}
      info("Socket Binding Succesfull"); 
	//listen to the client while others are waiting in queue of size 5
	int listenStatus = listen(server_socket , CLIENT_LISTEN );
	if(listenStatus < PROTOCOL)
        {	// when queue is full listen fails
		error( "Listner has failed");
		return 0;
        }
	info("Listening");
	info("...Waiting for connections... ");
	int client_socket;
	socklen_t len = sizeof(clientAddress);
         char buffer[MAXLEN],command[COMMAND_SIZE], filename[FILE_SIZE]; //initiating variables for data buffer,command buffer,file buffer
	char socket_buffer[MAXLEN];
	memset(buffer,PROTOCOL,MAXLEN);
	memset(socket_buffer,PROTOCOL,MAXLEN);
	if((client_socket = accept(server_socket , (struct sockaddr*) & clientAddress , &len)) < PROTOCOL) {
		error( "Server didn't accept the request.") ;
		return  0;
	} else {
		info( "Server accepted the request. ") ;
		recv(client_socket,&login_check,sizeof(int),PROTOCOL);
		if(login_check == true) {
			info("logined");
			while(true) {
				// receiving file name or extension
		                recv(client_socket,filename,FILE_SIZE,PROTOCOL);
		                std::cout<<"Received file name : "<<filename<<"\n";
		                // receiving control information
		                recv(client_socket,buffer,MAXLEN,PROTOCOL);
		                int cmd_read ,read_;
		                int store = 0;
				for(cmd_read = 0; buffer[cmd_read]!=':'; cmd_read++) {
					command[store++] = buffer[cmd_read];
				}
		                command[store]='\0';
		                std::cout<<"Received command : "<<command<<std::endl;
				int check_ = 0;
				//reading data port
				for(read_ = cmd_read+1;buffer[read_]!='#';read_++) {
					socket_buffer[check_++] = buffer[read_];
				}
		if(!strcmp(command,"GET")){

			//recv(client_socket,socket_buffer,sizeof(socket_buffer),0);
			
			unsigned long int PORT = atoi(socket_buffer);
			OBJ.dataConnectionSend(clientAddress,client_socket,PORT,filename);
	  		memset(buffer,PROTOCOL,MAXLEN);
	  		memset(socket_buffer,PROTOCOL,MAXLEN);
		}
		else if(!strcmp(command,"PUT")){
			unsigned long int PORT = atoi(socket_buffer);
			OBJ.dataConnectionReceive(clientAddress,client_socket,PORT,filename);
			memset(buffer,PROTOCOL,MAXLEN); // freeup the memory occupied by maxlen
			memset(socket_buffer,PROTOCOL,MAXLEN);
		}
		else if(!strcmp(command,"MGET")){


			unsigned long int PORT = atoi(socket_buffer);

			sa clientAddress2;

			// creating data socket for data connection
			int client_data_socket= socket(AF_INET,SOCK_STREAM,PROTOCOL);
			if(client_data_socket==PROTOCOL){
				error("Data Socket cannot be opened");
			}

		 	bzero(&clientAddress2, sizeof(clientAddress2));


			clientAddress2.sin_family = AF_INET;
			clientAddress2.sin_addr.s_addr = clientAddress.sin_addr.s_addr;
			clientAddress2.sin_port = PORT;
			
			// sending connect request to the client 
			int status_connect = connect(client_data_socket,(struct sockaddr*) &clientAddress2,sizeof(clientAddress2));
			
			if(status_connect < PROTOCOL){
				error("Connection Error");
			}
			// traversing through the directory for the files with given extension 
			char *fname;
			//creating poineter variable for directory
			 DIR *directory;
	                 struct dirent *dir;
	                 directory = opendir(SERVER_PATH);
	                int ready = true;
			char newname[FILE_SIZE];
			while((dir = readdir(directory)) != NULL){
				fname=dir->d_name;
				memset(newname,PROTOCOL,sizeof(newname));
	                        strcpy(newname,fname);
				char* fextension = strrchr(newname, '.'); // getting the extension of the files
				if(fextension == NULL) continue;
				if (strcmp(filename,fextension) == PROTOCOL){   // comparing the externsion with the given extension
					std::cout<<"Sending " << newname<<"file\n";
	                                send(client_socket, &ready, sizeof(int), PROTOCOL);
	                                send(client_socket,newname,FILE_SIZE,PROTOCOL);
					OBJ.dataConnectionSend_mget(client_data_socket,clientAddress,client_socket,PORT,newname);
				}
			}
			ready = false;
			send(client_socket,&ready,sizeof(int),PROTOCOL);
			closedir(directory);
			memset(buffer,PROTOCOL,sizeof(buffer));
			close(client_data_socket);
		}
		 else if(!strcmp(command, "QUIT"))
	        {
	           
		    info("Quitting....");
	            send(client_socket, &cmd_read, sizeof(int), PROTOCOL);           
	            break;
	        }

	}

}
	       
else{
	error("NOt loggined unable to proceed any file downloads");
}

}
 return 0;
}
