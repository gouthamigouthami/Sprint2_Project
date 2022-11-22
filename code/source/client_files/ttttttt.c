#include <pthread.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

sem_t sem;

int server()
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket
		= accept(server_fd, (struct sockaddr*)&address,
				(socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
int n;
    FILE *file;
    char *filename = "add.txt";
    file = fopen(filename, "w");
    while (1){
	    sem_wait(&sem);
	                n=recv(new_socket, buffer, 1024,0);
        if(n<=0){
                break;
               return 0;
       	}
        fprintf(file,"%s", buffer);
printf("%s\n", buffer);
printf("server received\n");
        bzero(buffer, 1024);
   }
 printf("[-]data written in the file successfully.\n");


	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}

int client()
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd
		= connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
FILE *f;
char *filename = "send.txt";
f = fopen(filename, "r");
if(f==NULL){
	perror("[-] Error in reading file.");
	exit(1);
}

    char data[1024] = {0};
while (fgets(data, 1024, f) != NULL){
	    if(send(sock, data, sizeof(data), 0) == -1){
		    perror("[-]Error in sending file,");
			    exit(1);
		    }
		bzero(data, 1024);
     		sem_post(&sem);
                usleep(100*1000);
}

printf("the file was sent successfuly");
	// closing the connected socket
	close(client_fd);
	return 0;
}

int main(){
	pthread_t thread1;
	pthread_t thread2;
	sem_init(&sem,0,0);

	pthread_create(&thread1,NULL,(void *)client,NULL);
	pthread_create(&thread2,NULL,(void *)server,NULL);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	return 0;
}
