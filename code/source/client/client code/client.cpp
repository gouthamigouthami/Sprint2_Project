#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include "header.h"
using namespace std;
int client_connection()
{
int clientSocket,serverSocket , receiveMsgSize;

	clientSocket = socket(AF_INET , SOCK_STREAM , 0);
	if(clientSocket < 0)
        {
		cout<< "Creation of client socket failed\n" ;
		return 0;
        }
	 cout<<"socked creted";

	struct sockaddr_in serverAddr , clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(port);
	if(connect(clientSocket ,  (struct sockaddr*) & serverAddr , sizeof(serverAddr)) < 0)
	{
		cout<< "Connection Error...\n";
		return 0;
	}
	else
	{
		 cout<<"Connection Established...\n";
		 return 1;
	}
}

int main()
{

if(client_connection())
{
	cout<<"succesfill";
}
	 

}
