#include<iostream>
#include<fstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <netdb.h>
#include <time.h>
#include <iomanip>
#include <fcntl.h>
#include <ifaddrs.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include "client_header.h"
using namespace std;
int LOGIN_CHECK=false;
char NAME[MAXIMUM_NAME],PASSWORD[MAXIMUM_PSW],EMAIL[MAXIMUM_EMAIL],PHONE_NUMBER[MAXIMUM_PH_NO];
class Customer{
	protected:
		string name, email, phone_number;
	public:
		/* setting name **/
		void setName(){
			LOG_INFO("Enter your name : ");
			cin.ignore();
			getline(std::cin, name);
			std::cout << std::endl;
		}
		/* setting email */
		void setEmail(){
			int valid_email = false;
			char temp;
			LOG_INFO("Enter your email address: ");
			cin >> email;
			std::cout << std::endl;
			int email_length = email.length();
			for (int check=0; check<email_length; ++check){
				temp = email.at(check);
				if (temp == '@' ){
					valid_email = true;
				}
			}
			while (!valid_email ){
				LOG_ERROR("Enter a valid email address: ");
				std::cin.ignore();
				std::cin >> email;
				std::cout << std::endl;
				for (int email_check=0; email_check<email_length; ++email_check){
					temp = email.at(email_check);
					if (temp == '@' ){
						valid_email = true;
					}
				}
			}
		}
		/*  setting phonenumber  */
		void setPhonenumber(){
			LOG_INFO("Enter your phone number : ");
			std::cin >> phone_number;
			int phone_length = phone_number.length();
			while (phone_length != PHNUM_LIMIT){
				std::cout << std::endl;
				LOG_ERROR("Enter a valid phone number: ");
				std::cin >> phone_number;
				phone_length = phone_number.length();
			}
			std::cout << std::endl;
		}
		/* returning the  name */
		string getName(){
			return name;
		}
		/* returning the  phone number */
		string getPhonenumber(){
			return phone_number;
		}
		/* returning the email */
		string getEmail(){
			return email;
		}
		~Customer(){}
};
class Member : public Customer {
	public:
		char password[MAXIMUM_PSW];
		/*setting password*/
		void setPassword(){
			char password_1[MAXIMUM_PSW];
passwordset:
			LOG_INFO("ENTER YOUR PASSWORD : ");
			/* taking password input from user */
			std::cin >> password;
			string pw = password;
			LOG_INFO("REENTER YOUR PASSWORD :");
			std::cin >> password_1;
			string pw1 = password_1;
			if(pw1==pw){
				LOG_INFO (" Your password set. you are a member now !!");
			} else {
				LOG_ERROR( " Your password does not match. Try again !!");
				goto passwordset;
			}
		}
		/* returning the password */
		string getPassword(){
			return password;
		}
};
/*creating client class*/
class client {
	protected:
		/* membership function for a  newmembe */
		void newmembership(){
			fstream member;
			member.open("Membership.txt",ios::app);
			string username,password , username1,password1;
			Member * m1 = new Member;
			m1 -> setName();
			m1 -> setEmail();
			m1 -> setPhonenumber();
			m1 ->setPassword();
			std::cout << "\n\t----------------------------------------------------------------------------------------------\n"                               <<setw(MAXIMUM_PSW)<< "\tHello Mr/MS , " << std::endl << std::endl <<
				setw(MAXIMUM_PSW)<< "Your Name is : " << m1 -> getName() << std::endl << std::endl <<
				setw(MAXIMUM_PSW)<< "Your Password is : " << m1 -> getPassword() << std::endl << std::endl <<
				setw(MAXIMUM_PSW)<< "Your Phone Number is : " << m1 -> getPhonenumber() << std::endl << std::endl <<
				setw(MAXIMUM_PSW)<< "Your Email is : " << m1 -> getEmail() <<std::endl << std::endl
				<< "\t --------------------------------------------------------------------------------------------------\n\n";
			member << m1 ->getName() <<  '|' << m1->getEmail() << '|' << m1->getPhonenumber() <<  '|'
				<< m1 ->getPassword() <<'\n';
			//closing file
			member.close();
				/* *after finishing newmembership registration calling to login function to login after registration */
			login();
		}
		/*  login for existing and new user afetr registration */
		void login(){
			fstream member1;
			char pass_input[MAXIMUM_PSW];
			string line,password_input,name ,name_input, email1, password1;
			int login_attempt = false, count = 0 ,  success=false,read = 0;
name_check:
			member1.open("Membership.txt",ios::in);
			LOG_INFO(" Enter your name for login : ");
			std::cin >> name_input;
			ifstream file("Membership.txt");
			while (getline(file, line)) {
				count++;
			}
			while( read <= count ){
				member1.getline(NAME, MAXIMUM_NAME, '|');
				member1.getline(EMAIL, MAXIMUM_EMAIL, '|');
				member1.getline(PHONE_NUMBER, MAXIMUM_PH_NO, '|');
				member1.getline(PASSWORD, MAXIMUM_PSW);
				name = NAME;
				if ( NAME== name_input){
					success  = true;
					break;
				}
				read ++;
			}
			member1.close();
			if ( !success  ){
				std::cout <<std:: endl;
				LOG_ERROR("Your Name not found !!!");
				goto name_check;
			}
			password1=PASSWORD;
			std::cout << std::endl;
			LOG_INFO("Enter  password : ");
			std::cin>>pass_input;
			for (login_attempt = 1 ; login_attempt <= MAXIMUM_ATTEMPT ; login_attempt ++){
				if (pass_input ==password1 ){
					LOGIN_CHECK=true;
					LOG_INFO("Login Successful !!!");
					break;
				}
				std::cout <<std:: endl;
				std::cout << "Login Failed. Attempt " << login_attempt  << " of 3" << std::endl;
				LOG_ERROR ("Please re-enter Password: ") ;
				std::cin>>pass_input;
				if (pass_input == password1){
					LOGIN_CHECK=true;
					LOG_INFO( "Login Successful !!!");
					break;
				}
			}
			if (login_attempt == MAXIMUM_ATTEMPT){
				std::cout << std::endl;
				LOG_ERROR("Login Failed. Attempt 3 of 3");
			}
		}
	public:
/* memberhip function for  reading user input foe login and new membership */
void membership()
		{
			char resp;
			std::cout << "\n\t\t========================\t==========================\t\n"
				<< "\t\t* (A) NEW MEMBERSHIP  \t\t     *(B)   LOGIN *\t\t\n"
				<< "\t\t========================\t==========================\t\n\n"
				<< "\t\t\t\t\t\t\t Input :" ;
			std::cin >> resp;
			resp = tolower(resp);
			while ((resp != 'a') && (resp != 'b') )
			{
				LOG_ERROR( "Enter a valid response: ");
				std::cin >> resp;
			}
			if (resp == 'a'){
				newmembership();
			}else{
				login();
			}
		}
/* data_connection_send is for sending files from client to server */
void data_connection_send(unsigned long int dataPort,int sockfd,char* filename){
	struct sockaddr_in clientAddr;
	int clientfd,already_exist = false,overwrite = true,filehandle;
	char filename_path[FILEPATH_SIZE];
	// creating data socket for data connection 
        clientfd = socket(AF_INET,SOCK_STREAM,PROTOCOL);
	if(clientfd < PROTOCOL){
		LOG_ERROR("Error in creating socket for data connection");
		return;
	}
	memset(&clientAddr,PROTOCOL,sizeof(clientAddr));
	//objects of clientAddr 
        clientAddr.sin_family = AF_INET;//ipv4
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = dataPort;
	//binding the socket with the corresponding data port
        int binding = bind(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
        if(binding < PROTOCOL){
		LOG_ERROR("Error in binding in PUT");
		return;
	}
	//listening for the server request 
        int lis = listen(clientfd,MAX_LISTEN);
        if(lis < PROTOCOL){
		LOG_ERROR("Error in listening in PUT");
		return;
	}
	struct sockaddr_in serverAddr;
	socklen_t length = sizeof(serverAddr);
	// accepting the request from the server
        int acc = accept(clientfd,(struct sockaddr*) &serverAddr,&length);
	LOG_INFO("Accepted");
        strcpy(filename_path,PATH);
        strcat(filename_path,filename);
        filehandle = open(filename_path,O_RDONLY);
        // receving whether the file already exists in the server or not 
        recv(sockfd,&already_exist,sizeof(int),PROTOCOL);
	if(already_exist){
		std::cout<<filename<<" file already exists in server \n press 1 for OVERWRITE \n press 0 for NO OVERWRITE\n";
repeat:
		std::cin>>overwrite;
		if(overwrite != 0 && overwrite != 1){
			LOG_ERROR("Invalid type 0 or 1");
			goto repeat;
		}
	}
	//sending the overwrite option over control connection 
        send(sockfd,&overwrite,sizeof(int),PROTOCOL);
	char data[DATA_SIZE];
	memset(data,PROTOCOL,sizeof(data));
        if(overwrite == true){
		struct stat obj;
                int status,size;
                stat(filename_path,&obj);
                size = obj.st_size;
		sprintf(data,"%d",size);
		send(sockfd,data,DATA_SIZE,PROTOCOL);
                sendfile(acc,filehandle,NULL,size);
                recv(sockfd,&status,sizeof(int),PROTOCOL);
                if(status){
			LOG_INFO("File successful PUT in the server");
		}else LOG_ERROR("File not PUT successfully in the server");
	}
	close(clientfd);
	close(acc);
}
/*
* data_connection_receive_mget is for receiving all user choice extension files from server to client
*/
void data_connection_receive_mget(int acc,unsigned long int dataPort,int sockfd,char* filename){
	int size,filehandle,status;
        int already_exist = false,overwrite = true;
        char filename_path[FILEPATH_SIZE];
	char data[DATA_SIZE];
        strcpy(filename_path,PATH);
        strcat(filename_path,filename);
	memset(data,PROTOCOL,sizeof(data));
        //recv(sockfd,&size,sizeof(int),0);
        recv(sockfd,data,DATA_SIZE,PROTOCOL);
        size = atoi(data);
	if(size){
		// checking whether the file already exists or not 
		   if(access(filename_path, F_OK) != FILEHANDLE){
			   already_exist = true;
			   std::cout<<filename<<" file already exists in server \n press 1 for OVERWRITE \n press 0 for NO OVERWRITE\n";
loop:
			   std::cin>>overwrite;
			   if(overwrite != 0 && overwrite != 1){
				   LOG_ERROR("Invalid type 0 or 1");
				   goto loop;
			   }
		   }
		   // sending the overwrite option 
		   send(sockfd,&overwrite,sizeof(int),0);
		   if(overwrite == true){
			   if(already_exist==true)
				   filehandle = open(filename_path, O_WRONLY | O_CREAT | O_TRUNC, PERMISSION);
			   else
				   filehandle = open(filename_path, O_CREAT | O_EXCL | O_WRONLY, PERMISSION1);
			   char *receive_file = (char*)malloc(size*sizeof(char));
			   // receving the size of the file
			   recv(acc, receive_file, size, PROTOCOL);
			   status=write(filehandle, receive_file, size);
			   close(filehandle);
			   if(status){
				   LOG_INFO("File successful GET in the server");
			   }
			   else LOG_ERROR("File not GET successfully in the server");
		   }
	}else{
		LOG_ERROR("No such file exists in server");
	}
	return;
}
/*
* data_connection_recive for receiving files from server to client
*/
void data_connection_receive(unsigned long int dataPort,int sockfd,char* filename){
	struct sockaddr_in clientAddr;
	int clientfd;
	// creating data socket for data connection 
	clientfd = socket(AF_INET,SOCK_STREAM,PROTOCOL);
	if(clientfd < PROTOCOL){
		LOG_ERROR("Error in creating socket for data connection");
		return;
	}
	memset(&clientAddr,PROTOCOL,sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = dataPort;
	//binding the socket with the corresponding data port
	int binding = bind(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
	if(binding < PROTOCOL){
		LOG_ERROR("Error in binding in PUT");
		return;
	}
	//listening for the server request
	int lis = listen(clientfd,LISTEN_SIZE);
        if(lis < PROTOCOL){
		LOG_ERROR("Error in listening in PUT");
		return;
	}
	struct sockaddr_in serverAddr;
        socklen_t length = sizeof(serverAddr);
        // accepting the request from the server
        int acc = accept(clientfd,(struct sockaddr*) &serverAddr,&length);
	int size,filehandle,status;
        int already_exist = false,overwrite = true;
        char filename_path[FILEPATH_SIZE];
        strcpy(filename_path,PATH);
        strcat(filename_path,filename);
        char data[DATA_SIZE];
	memset(data,PROTOCOL,sizeof(data));
        //receiving file 
	recv(sockfd,data,DATA_SIZE,0);
	size = atoi(data);
	if(size){
        //checking the file already exists or not 
	if(access(filename_path, F_OK) != FILEHANDLE){
		already_exist = true;
		std::cout<<filename<<" file already exists in server \n press 1 for OVERWRITE \n press 0 for NO OVERWRITE\n";
repeat:
	    
		std::cin>>overwrite;
		if(overwrite != 0 && overwrite != 1){
			LOG_ERROR("Invalid type 0 or 1");
                        goto repeat;
		}
	}
	// sending the overwrite option to the server over control connection 
	send(sockfd,&overwrite,sizeof(int),PROTOCOL);
	if(overwrite == true){
		if(already_exist==true)
			filehandle = open(filename_path, O_WRONLY | O_CREAT | O_TRUNC, 644);
        	else
        		filehandle = open(filename_path, O_CREAT | O_EXCL | O_WRONLY, 0666);
		char *receive_file = (char*)malloc(size*sizeof(char)); 
                recv(acc, receive_file, size, PROTOCOL);  //receving the file
                status=write(filehandle, receive_file, size);
                close(filehandle);
                if(status){
			LOG_INFO("File successful GET in the server");
		}
		else LOG_ERROR("File not GET successfully in the server");
	}
	}else{
		LOG_INFO("No such file exists in server");
	}
	close(clientfd);
        close(acc);
        return;
}
};
int main()
{
	client OBJ;
	int sockfd;
        struct sockaddr_in serverAddr ;

	sockfd = socket(AF_INET , SOCK_STREAM , PROTOCOL);
	if(sockfd < PROTOCOL){

		LOG_INFO("Creation of client socket failed") ;
		return 0;
	}
	LOG_INFO("socked creted");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);
	if(connect(sockfd ,  (struct sockaddr*) & serverAddr , sizeof(serverAddr)) < PROTOCOL)
	{
		LOG_ERROR("Connection Error...");
		return 0;
	}else{
		LOG_INFO("Connection Established...");
		OBJ.membership();
		if(LOGIN_CHECK==true){
                LOG_INFO("successfull");
                LOG_INFO("GOOD TO GO ");
                send(sockfd,&LOGIN_CHECK,sizeof(int),PROTOCOL);
		while(1){
			char filename[FILENAME_SIZE],comm[BUFFER_SIZE],filename_path[FILEPATH_SIZE];
			LOG_INFO("Try These commands:-");
                        LOG_INFO("Upload files to server using- PUT");
                        LOG_INFO("Upload all files with a specific extention using- MPUT");
                        LOG_INFO("Download files from server using- GET");
                        LOG_INFO("Download all files with a specific extention using- MGET");
                        LOG_INFO("Exit Using- QUIT");
                        LOG_INFO("Enter the choice: command <filename>");
			// Taking input for command and filename 
       
			std::cin>>comm>>filename;
			char buffer_comm[CMD_SIZE],buffer_soc[BUFFER_SIZE];
                        memset(buffer_comm,PROTOCOL,sizeof(buffer_comm));
		        memset(buffer_soc,PROTOCOL,sizeof(buffer_soc));
                        strcpy(buffer_comm,comm);
                        strcpy(filename_path,PATH);
                        strcat(filename_path,filename);
		        char newname[BUFFER_SIZE];
		        memset(newname,PROTOCOL,sizeof(newname));
                        if(strcmp(comm,"PUT") == PROTOCOL){
				// checking whether the file exists or not in the client side
				if(access(filename_path,F_OK) == FILEHANDLE){
					std::cout<<filename<<" does not exist in client side\n";
				}else{
					// sending filename to the server
			                send(sockfd,filename,BUFFER_SIZE,PROTOCOL);
                                        // generating an arbitrary port number
					unsigned long int dataPort = rand()%1000;//3 digit
					strcat(buffer_comm,":");
					dataPort = dataPort + (long int)10000;//6digits
					sprintf(buffer_soc,"%ld",dataPort);
					strcat(buffer_comm,buffer_soc);
					strcat(buffer_comm,"#");
					//sending the command and data port over the control connection 
					send(sockfd,buffer_comm,sizeof(buffer_comm),PROTOCOL);
                                        OBJ.data_connection_send(dataPort,sockfd,filename);
                                        LOG_INFO("File PUT successful");
				}
			}
			else if(strcmp(comm,"GET") == 0){
				//sending filename to the server 
			        send(sockfd,filename,BUFFER_SIZE,PROTOCOL);
				//generating an arbitrary port number 
                                unsigned long int dataPort = rand()%1000;
			        strcat(buffer_comm,":");
				dataPort = dataPort + (long int)10000;
				sprintf(buffer_soc,"%ld",dataPort);
				strcat(buffer_comm,buffer_soc);
			        strcat(buffer_comm,"#");
				//sending the command and data port over the control connection
				send(sockfd,buffer_comm,sizeof(buffer_comm),PROTOCOL);
			        //send(sockfd,&dataPort,sizeof(dataPort),PROTOCOL);
				OBJ.data_connection_receive(dataPort,sockfd,filename);
			}
			else if(strcmp(comm,"MPUT") == 0){
				//going through all the files in the directory 
				DIR *directory;
				struct dirent *dir;
				directory = opendir(PATH);
				while((dir = readdir(directory)) != NULL){
				char *fname = dir -> d_name;
				strcpy(newname,fname);
				char* fextension = strrchr(newname,'.');  //gettting the file extension
				if(fextension == NULL) continue;
				else if(!strcmp(filename,fextension)){  //comparing the extension with the given extension
                                //sending filename to the server 
				send(sockfd,newname,BUFFER_SIZE,PROTOCOL);
                                //generating an arbitrary port number 
		                unsigned long int dataPort = rand()%1000;
				memset(buffer_comm,PROTOCOL,sizeof(buffer_comm));//reuse the same buffer after memset 
				memset(buffer_soc,PROTOCOL,sizeof(buffer_soc));
				strcpy(buffer_comm,"PUT");
				strcat(buffer_comm,":");
                                dataPort = dataPort + (long int)10000;
				sprintf(buffer_soc,"%ld",dataPort);
				strcat(buffer_comm,buffer_soc);
				strcat(buffer_comm,"#");
				//sending commad	
				send(sockfd,buffer_comm,sizeof(buffer_comm),PROTOCOL);
		                OBJ.data_connection_send(dataPort,sockfd,newname);
				}
				}
				closedir(directory);
			}
			else if(strcmp(comm,"MGET") == 0){
				int clientfd;
				int ready ;
				//sending file name
				send(sockfd,filename,BUFFER_SIZE,PROTOCOL);
                                //generating arbitrary port number
                                unsigned long int dataPort = rand()%1000;
                                strcat(buffer_comm,":");
				dataPort = dataPort + (long int)10000;
				sprintf(buffer_soc,"%ld",dataPort);
				strcat(buffer_comm,buffer_soc);
                                strcat(buffer_comm,"#");
                                //sending command and port number 
                                send(sockfd,buffer_comm,sizeof(buffer_comm),PROTOCOL);
		                char newname[BUFFER_SIZE];
				memset(buffer_comm,PROTOCOL,sizeof(BUFFER_SIZE));
				struct sockaddr_in clientAddr;
                                //creating socket for the control connection
                                clientfd = socket(AF_INET,SOCK_STREAM,PROTOCOL);
                                if(clientfd < PROTOCOL){
					LOG_ERROR("Error in creating socket for data connection");
				}
				memset(&clientAddr,PROTOCOL,sizeof(clientAddr));
                                clientAddr.sin_family = AF_INET;
                                clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
                                clientAddr.sin_port = dataPort;
                                //binding the socket with the corresponding data port
                                int binding = bind(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
                                if(binding < PROTOCOL){
					LOG_ERROR("Error in binding in MGET");
				}
				//listening for the server request
				int lis = listen(clientfd,LISTEN_SIZE);
				if(lis < PROTOCOL){
					LOG_ERROR("Error in listening in MGET");
				}
				struct sockaddr_in serverAddr;
                                socklen_t length = sizeof(serverAddr);
                                // accepting the request from the server
                                int acc = accept(clientfd,(struct sockaddr*) &serverAddr,&length);
				while(true){
                                //ready = false implies all files are sent 
				recv(sockfd,&ready,sizeof(int),PROTOCOL);
				if(ready == false) break;
				memset(newname,PROTOCOL,sizeof(newname));
				recv(sockfd,newname,BUFFER_SIZE,PROTOCOL);
				OBJ.data_connection_receive_mget(acc,dataPort,sockfd,newname);
				}
				close(clientfd);
				close(acc);
			        LOG_INFO("File MGET successful");
			}
			else if(strcmp(comm,"QUIT")==0)
			{
				// strcpy(buffer, "quit");
                                send(sockfd,filename,BUFFER_SIZE,PROTOCOL);
                                unsigned long int dataPort = rand()%1000;
                                strcat(buffer_comm,":");
                                dataPort = dataPort + (long int)10000;
				sprintf(buffer_soc,"%ld",dataPort);
				strcat(buffer_comm,buffer_soc);
				strcat(buffer_comm,"#");
				send(sockfd,buffer_comm,sizeof(buffer_comm),PROTOCOL);
				int status = false;                
				recv(sockfd, &status, CMD_SIZE, PROTOCOL);
				if(status)
				{
					LOG_INFO("Quitting..");
					exit(0);
				}
				LOG_ERROR("Server failed to close connection");      
			}else{
				LOG_ERROR("command should be in form of GET,PUT,MGET,MPUT");
			}
		}
		}else{
			send(sockfd,&LOGIN_CHECK,sizeof(int),PROTOCOL);
			LOG_ERROR("client failed to login");
		}
	}
	return 0;
}
