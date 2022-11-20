#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include<stdlib.h>
#include "header.h"
using namespace std;
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
	
	int clientSocketHandler;
	socklen_t len = sizeof(clientAddr);
	int connection;
	if((connection = accept(serverSocketHandler , (struct sockaddr*) & clientAddr , &len)) < 0)
    {
		cout<< "Server didn't accept the request.\n" ;
		return  0;
	}
	else
    {
		cout<< "Server accepted the request.\n " ;
	
	

recv(connection,&login_check,sizeof(int),0);
 if(login_check==1){
cout<<"logined\n";
}

}
 return 0;
}

