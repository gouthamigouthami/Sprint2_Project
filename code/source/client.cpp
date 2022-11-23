#include<bits/stdc++.h>
#include<fstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include "client_header.h"
using namespace std;
int login_check=0;
char NAME[MAXIMUM_NAME],PASSWORD[MAXIMUM_PSW],EMAIL[MAXIMUM_EMAIL],PHONE_NUMBER[MAXIMUM_PH_NO];
class Customer{
    protected:
        string name, email, phone_number;
    public:
        void setName(){
            LOG_INFO("Enter your name : ");
            cin.ignore();
            getline(cin, name);
            cout << endl;
        }

        void setEmail(){
            int valid_email = 0;
            char temp;
            LOG_INFO("Enter your email address: ");
            cin >> email;
            cout << endl;

            int email_length = email.length();

            for (int check=0; check<email.length(); ++check){
                temp = email.at(check);
		  if (temp == '@' ){
                    valid_email = 1;
                }
            }

            while (valid_email != 1){
                LOG_ERROR("Enter a valid email address: ");
                cin.ignore();
                cin >> email;
                cout << endl;

                for (int email_check=0; email_check<email.length(); ++email_check){
                  temp = email.at(email_check);
                  if (temp == '@' ){
                     valid_email = 1;
                  }
                }
            }
        }

        void setPhonenumber(){
            LOG_INFO("Enter your phone number : ");
            cin >> phone_number;

            int phone_length = phone_number.length();

            while (phone_length != PHNUM_LIMIT){
               cout << endl;
               LOG_ERROR("Enter a valid phone number: ");
               cin >> phone_number;
               phone_length = phone_number.length();
            }

            cout << endl;
        }
	 string getName(){
            return name;
        }

        string getPhonenumber(){
            return phone_number;
        }

        string getEmail(){
            return email;
        }

       ~Customer(){};
};
class Member : public Customer {
    public:
    char password[MAXIMUM_PSW];
    void setPassword(){

            char password_1[MAXIMUM_PSW];
             passwordset:
            LOG_INFO("ENTER YOUR PASSWORD : ");
            cin >> password;
            string pw = password;
            LOG_INFO("\n  REENTER YOUR PASSWORD :");
            cin >> password_1;
            string pw1 = password_1;
            if(pw1==pw){
             LOG_INFO (" Your password set. you are a member now !!");}
              else{
               LOG_ERROR( " Your password does not match. Try again !!");
              goto passwordset;}

        }


     string getPassword(){
            return password;
         }


};
class client {
 protected:

void newmembership(){
    fstream member;

     member.open("Membership.txt",ios::app);

     string username,password , username1,password1;
       Member * m1 = new Member;
        m1 -> setName();

        m1 -> setEmail();
        m1 -> setPhonenumber();
        m1 ->setPassword();


 cout << "\n\t-------------------------------------------------------------------------------------------------\n\n" <<
         setw(MAXIMUM_PSW)<< "\tHello Mr/MS , " << endl << endl <<

         setw(MAXIMUM_PSW)<< "Your Name is : " << m1 -> getName() << endl << endl <<
         setw(MAXIMUM_PSW)<< "Your Password is : " << m1 -> getPassword() << endl << endl <<
         setw(MAXIMUM_PSW)<< "Your Phone Number is : " << m1 -> getPhonenumber() << endl << endl <<
         setw(MAXIMUM_PSW)<< "Your Email is : " << m1 -> getEmail() << endl << endl
         << "\t --------------------------------------------------------------------------------------------------\n\n";
         


        member << m1 ->getName() <<  '|' << m1->getEmail() << '|' << m1->getPhonenumber() <<  '|'
             << m1 ->getPassword() <<'\n';

       member.close();
 login();
}
void login(){
    fstream member1;
    char pass_input[MAXIMUM_PSW];
    string line,password_input,name ,name_input, email1, password1;
    int login_attempt = 0, count = 0 ,  success=0;
    char  resp;

    name_check:
    member1.open("Membership.txt",ios::in);
    LOG_INFO(" Enter your name for login : ");
    cin >> name_input;

    ifstream file("Membership.txt");
    while (getline(file, line)) {
        count++;
     }

    int read = 0;
     while(read <= count)
    {
        member1.getline(NAME,MAXIMUM_NAME,'|');
       member1.getline(EMAIL,MAXIMUM_EMAIL,'|');
        member1.getline(PHONE_NUMBER,MAXIMUM_PH_NO,'|');
        member1.getline(PASSWORD,MAXIMUM_PSW);
        name = NAME;
           if ( NAME== name_input){
           success  = 1;
            break;
        }

        read ++;
    }
    member1.close();
    if ( success != 1 ){
        cout << endl;
        LOG_ERROR("Your Name not found !!!");

        goto name_check;
    }

     password1=PASSWORD;


    cout << endl;
    LOG_INFO("Enter  password : ");
    cin>>pass_input;

     for (login_attempt = 1 ; login_attempt <= MAXIMUM_ATTEMPT ; login_attempt ++){
        if (pass_input ==password1 ){
login_check=1;
            LOG_INFO("Login Successful !!!");

            break;
        }
	 cout << endl;
        cout << "Login Failed. Attempt " << login_attempt  << " of 3" << endl;
        LOG_ERROR ("Please re-enter Password: ") ;

        cin>>pass_input;

        if (pass_input == password1){
login_check=1;
           LOG_INFO( "Login Successful !!!");

                  
                break;
        }
    }
     if (login_attempt == MAXIMUM_ATTEMPT){
        cout << endl;
        LOG_ERROR("Login Failed. Attempt 3 of 3");
 
     }
        }
public:

void membership()
{
    char resp;
      cout << "\n\t\t========================\t==========================\t\n"
         << "\t\t* (A) NEW MEMBERSHIP  \t\t     *(B)   LOGIN *\t\t\n"
         << "\t\t========================\t==========================\t\n\n"
         << "\t\t\t\t\t\t\t Input :" ;
    cin >> resp;
    resp = tolower(resp);

    while ((resp != 'a') && (resp != 'b') )
    {
        LOG_ERROR( "Enter a valid response: ");
        cin >> resp;
    }

    if (resp == 'a'){

       newmembership();
    }
      else{
        login();
    }
  
  
     
}

void data_connection_send(unsigned long int dataPort,int sockfd,char* filename){
    struct sockaddr_in clientAddr;
    int clientfd;
    // creating data socket for data connection 
    clientfd = socket(AF_INET,SOCK_STREAM,0);
    if(clientfd < 0){
        LOG_ERROR("Error in creating socket for data connection");
        return;
    }
    memset(&clientAddr,0,sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = dataPort;

    //binding the socket with the corresponding data port
    int binding = bind(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
    if(binding < 0){
       LOG_ERROR("Error in binding in PUT");
        return;
    }
    //listening for the server request 
    int lis = listen(clientfd,MAX_LISTEN);
    if(lis < 0){
        LOG_ERROR("Error in listening in PUT");
        return;
    }
    struct sockaddr_in serverAddr;
    socklen_t l = sizeof(serverAddr);
    // accepting the request from the server
    int acc = accept(clientfd,(struct sockaddr*) &serverAddr,&l);
	LOG_INFO("Accepted");
    int already_exist = 0,overwrite = 1,filehandle;
    char filename_path[FILEPATH_SIZE];
    strcpy(filename_path,PATH);
    strcat(filename_path,filename);
    filehandle = open(filename_path,O_RDONLY);
    // receving whether the file already exists in the server or not 
    recv(sockfd,&already_exist,sizeof(int),0);

    if(already_exist){
        cout<<filename<<" file already exists in server \n press 1 for OVERWRITE \n press 0 for NO OVERWRITE\n";
        l:scanf("%d",&overwrite);
        if(overwrite != 0 && overwrite != 1){
            LOG_ERROR("Invalid type 0 or 1");
            goto l;
        }
    }
    //sending the overwrite option over control connection 
    send(sockfd,&overwrite,sizeof(int),0);
	char data[DATA_SIZE];
	memset(data,0,sizeof(data));
    if(overwrite == 1){

        struct stat obj;
        int status,size;
        stat(filename_path,&obj);
        size = obj.st_size;
		sprintf(data,"%d",size);
		send(sockfd,data,DATA_SIZE,0);
        //send(sockfd,&size,sizeof(int),0);

        sendfile(acc,filehandle,NULL,size);
        recv(sockfd,&status,sizeof(int),0);
        if(status){
            LOG_INFO("File successful PUT in the server");
        }
        else LOG_ERROR("File not PUT successfully in the server");
    }
    close(clientfd);
    close(acc);
}

void data_connection_receive_mget(int acc,unsigned long int dataPort,int sockfd,char* filename){

    

    int size,filehandle,status;
    int already_exist = 0,overwrite = 1;
    char filename_path[FILEPATH_SIZE];
    strcpy(filename_path,PATH);
    strcat(filename_path,filename);

    char data[DATA_SIZE];
    memset(data,0,sizeof(data));
    //recv(sockfd,&size,sizeof(int),0);
    recv(sockfd,data,DATA_SIZE,0);
    size = atoi(data);
   

    if(size){
        // checking whether the file already exists or not 
        if(access(filename_path, F_OK) != -1){
            already_exist = 1;
            cout<<filename<<" file already exists in server \n press 1 for OVERWRITE \n press 0 for NO OVERWRITE\n";
            l:scanf("%d",&overwrite);
            if(overwrite != 0 && overwrite != 1){
                    LOG_ERROR("Invalid type 0 or 1");
                    goto l;
            }
        }
        // sending the overwrite option 
        send(sockfd,&overwrite,sizeof(int),0);

        if(overwrite == 1){
            if(already_exist==1)
                filehandle = open(filename_path, O_WRONLY | O_CREAT | O_TRUNC, 644);
            else
                filehandle = open(filename_path, O_CREAT | O_EXCL | O_WRONLY, 666);

            char *receive_file = (char*)malloc(size*sizeof(char));
            // receving the size of the file
            recv(acc, receive_file, size, 0);
            status=write(filehandle, receive_file, size);
            close(filehandle);
            if(status){
                LOG_INFO("File successful GET in the server");
            }
            else LOG_ERROR("File not GET successfully in the server");
        }
    }
    else{
        LOG_ERROR("No such file exists in server");
    }
    return;
}

void data_connection_receive(unsigned long int dataPort,int sockfd,char* filename){

	struct sockaddr_in clientAddr;
    int clientfd;
    // creating data socket for data connection 
    clientfd = socket(AF_INET,SOCK_STREAM,0);
    if(clientfd < 0){
        LOG_ERROR("Error in creating socket for data connection");
        return;
    }
    memset(&clientAddr,0,sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = dataPort;
    //binding the socket with the corresponding data port
    int binding = bind(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
    if(binding < 0){
        LOG_ERROR("Error in binding in PUT");
        return;
    }
    //listening for the server request
    int lis = listen(clientfd,LISTEN_SIZE);
    if(lis < 0){
        LOG_ERROR("Error in listening in PUT");
        return;
    }
    struct sockaddr_in serverAddr;
    socklen_t l = sizeof(serverAddr);
    // accepting the request from the server
    int acc = accept(clientfd,(struct sockaddr*) &serverAddr,&l);

    int size,filehandle,status;
    int already_exist = 0,overwrite = 1;
    char filename_path[FILEPATH_SIZE];
    strcpy(filename_path,PATH);
    strcat(filename_path,filename);

	char data[DATA_SIZE];
	memset(data,0,sizeof(data));
    //recv(sockfd,&size,sizeof(int),0);
    //receving the size of the file
	recv(sockfd,data,DATA_SIZE,0);
	size = atoi(data);
	

    if(size){
        //checking the file already exists or not 
        if(access(filename_path, F_OK) != -1){
            already_exist = 1;
          cout<<filename<<" file already exists in server \n press 1 for OVERWRITE \n press 0 for NO OVERWRITE\n";
            l:
	    scanf("%d",&overwrite);
            if(overwrite != 0 && overwrite != 1){
                    LOG_ERROR("Invalid type 0 or 1");
                    goto l;
            }
        }
        // sending the overwrite option to the server over control connection 
        send(sockfd,&overwrite,sizeof(int),0);

        if(overwrite == 1){
            if(already_exist==1)
        		filehandle = open(filename_path, O_WRONLY | O_CREAT | O_TRUNC, 644);
        	else
        		filehandle = open(filename_path, O_CREAT | O_EXCL | O_WRONLY, 0666);

            char *receive_file = (char*)malloc(size*sizeof(char)); 
            recv(acc, receive_file, size, 0);  //receving the file
            status=write(filehandle, receive_file, size);
            close(filehandle);
            if(status){
                LOG_INFO("File successful GET in the server");
            }
            else LOG_ERROR("File not GET successfully in the server");
        }
    }
    else{
        LOG_INFO("No such file exists in server");
    }
    close(clientfd);
    close(acc);
    return;
}

};

int main()
{
LOG_INIT();
	client OBJ;

int sockfd;

	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if(sockfd < 0)
        {
		LOG_INFO("Creation of client socket failed") ;
		return 0;
        }
	 LOG_INFO("socked creted");

	struct sockaddr_in serverAddr ;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(PORT);
	if(connect(sockfd ,  (struct sockaddr*) & serverAddr , sizeof(serverAddr)) < 0)
	{
		LOG_ERROR("Connection Error...");
		return 0;
	}
	else
	{
		 LOG_INFO("Connection Established...");

       OBJ.membership();
     if(login_check==1){
    LOG_INFO("successfull");
    LOG_INFO("GOOD TO GO ");
    send(sockfd,&login_check,sizeof(int),0);
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
        scanf("%s %s",comm,filename);

        char buffer_comm[CMD_SIZE],buffer_soc[BUFFER_SIZE];
        memset(buffer_comm,0,sizeof(buffer_comm));
		memset(buffer_soc,0,sizeof(buffer_soc));
        strcpy(buffer_comm,comm);
        strcpy(filename_path,PATH);
        strcat(filename_path,filename);
		char newname[BUFFER_SIZE];
		memset(newname,0,sizeof(newname));
        if(strcmp(comm,"PUT") == 0){
            // checking whether the file exists or not in the client side
            if(access(filename_path,F_OK) == -1){
               cout<<filename<<" does not exist in client side\n";
            }
            else{
            // sending filename to the server
			send(sockfd,filename,BUFFER_SIZE,0);
            // generating an arbitrary port number
            unsigned long int dataPort = rand()%1000;
			strcat(buffer_comm,":");
			dataPort = dataPort + (long int)10000;

            sprintf(buffer_soc,"%ld",dataPort);

			strcat(buffer_comm,buffer_soc);
			strcat(buffer_comm,"#");
            //sending the command and data port over the control connection 
            send(sockfd,buffer_comm,sizeof(buffer_comm),0);
            OBJ.data_connection_send(dataPort,sockfd,filename);
            LOG_INFO("File PUT successful");
            }
        }
        else if(strcmp(comm,"GET") == 0){

            //sending filename to the server 
			send(sockfd,filename,BUFFER_SIZE,0);
            //generating an arbitrary port number 
            unsigned long int dataPort = rand()%1000;
			strcat(buffer_comm,":");
			dataPort = dataPort + (long int)10000;

            sprintf(buffer_soc,"%ld",dataPort);

			strcat(buffer_comm,buffer_soc);
			strcat(buffer_comm,"#");
            //sending the command and data port over the control connection
            send(sockfd,buffer_comm,sizeof(buffer_comm),0);
			//send(sockfd,&dataPort,sizeof(dataPort),0);

            OBJ.data_connection_receive(dataPort,sockfd,filename);

            
        }

        else if(strcmp(comm,"MPUT") == 0){
            //going through all the files in the directory 
			DIR *d;
			struct dirent *dir;
			d = opendir(PATH);
			while((dir = readdir(d)) != NULL){
				char *fname = dir -> d_name;
				strcpy(newname,fname);
				//printf("%s\n",newname);
				char* fextension = strrchr(newname,'.');  //gettting the file extension
				//printf("%s this is extension\n",fextension);
				if(fextension == NULL) continue;
				else if(!strcmp(filename,fextension)){  //comparing the extension with the given extension
                    //sending filename to the server 
					send(sockfd,newname,BUFFER_SIZE,0);
                    //generating an arbitrary port number 
		            unsigned long int dataPort = rand()%1000;
					memset(buffer_comm,0,sizeof(buffer_comm));
					memset(buffer_soc,0,sizeof(buffer_soc));
					strcpy(buffer_comm,"PUT");
					strcat(buffer_comm,":");
                    
					dataPort = dataPort + (long int)10000;
					sprintf(buffer_soc,"%ld",dataPort);

					strcat(buffer_comm,buffer_soc);
					strcat(buffer_comm,"#");
		            send(sockfd,buffer_comm,sizeof(buffer_comm),0);
		            OBJ.data_connection_send(dataPort,sockfd,newname);
		          
				}
			}
			closedir(d);
        }
        else if(strcmp(comm,"MGET") == 0){
            //sending file name
			send(sockfd,filename,20,0);
            //generating arbitrary port number
            unsigned long int dataPort = rand()%1000;
            strcat(buffer_comm,":");
            dataPort = dataPort + (long int)10000;

            sprintf(buffer_soc,"%ld",dataPort);

            strcat(buffer_comm,buffer_soc);
            strcat(buffer_comm,"#");
            //sending command and port number 
            send(sockfd,buffer_comm,sizeof(buffer_comm),0);
			int ready ;
			char newname[BUFFER_SIZE];
            memset(buffer_comm,0,sizeof(buffer_comm));
            struct sockaddr_in clientAddr;
            int clientfd;
            //creating socket for the control connection
            clientfd = socket(AF_INET,SOCK_STREAM,0);
            if(clientfd < 0){
                LOG_ERROR("Error in creating socket for data connection");
            }
            memset(&clientAddr,0,sizeof(clientAddr));
            clientAddr.sin_family = AF_INET;
            clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
            clientAddr.sin_port = dataPort;
            //binding the socket with the corresponding data port
            int binding = bind(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
            if(binding < 0){
                LOG_ERROR("Error in binding in PUT");
            }
            //listening for the server request
            int lis = listen(clientfd,LISTEN_SIZE);
            if(lis < 0){
                LOG_ERROR("Error in listening in PUT");
            }
            struct sockaddr_in serverAddr;
            socklen_t l = sizeof(serverAddr);
            // accepting the request from the server
            int acc = accept(clientfd,(struct sockaddr*) &serverAddr,&l);

			while(1){
                //ready = 0 implies all files are sent 
				recv(sockfd,&ready,sizeof(int),0);
               
				if(ready == 0) break;
				memset(newname,0,sizeof(newname));
				recv(sockfd,newname,BUFFER_SIZE,0);
				OBJ.data_connection_receive_mget(acc,dataPort,sockfd,newname);

			}
            close(clientfd);
            close(acc);
			LOG_INFO("File MGET successful");
        }
        else if(strcmp(comm,"QUIT")==0)
            {
                // strcpy(buffer, "quit");
                send(sockfd,filename,BUFFER_SIZE,0);
                unsigned long int dataPort = rand()%1000;
                strcat(buffer_comm,":");
                dataPort = dataPort + (long int)10000;

                sprintf(buffer_soc,"%ld",dataPort);

                strcat(buffer_comm,buffer_soc);
                strcat(buffer_comm,"#");
                send(sockfd,buffer_comm,sizeof(buffer_comm),0);
                int status = 0;                
                recv(sockfd, &status, CMD_SIZE, 0);                  
                if(status)
                {
                    LOG_INFO("Quitting..");
                    exit(0);
                }
                LOG_ERROR("Server failed to close connection");      
            }
        else{
            LOG_ERROR("command should be in form of GET,PUT,MGET,MPUT");
        }
    }

}
else
{
send(sockfd,&login_check,sizeof(int),0);
LOG_ERROR("client failed to login");
}


}
 LOG_DEINIT();
}
