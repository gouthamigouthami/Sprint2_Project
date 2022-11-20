#include<bits/stdc++.h>
/*#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include<fstream>
*/
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
#define path "./client_files/"


#include "header.h"
using namespace std;
int login_check=0;
char NAME[MAXIMUM_NAME],PASSWORD[MAXIMUM_PSW],EMAIL[MAXIMUM_EMAIL],PHONE_NUMBER[MAXIMUM_PH_NO];
class Customer{
    protected:
        string name, email, phone_number;
    public:
        void setName(){
            cout<< "                  Enter your name : ";
            cin.ignore();
            getline(cin, name);
            cout << endl;
        }

        void setEmail(){
            int valid_email = 0;
            char temp;
            cout << "                  Enter your email address: ";
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
                cout << "               Enter a valid email address: ";
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
            cout << "                  Enter your phone number : ";
            cin >> phone_number;

            int phone_length = phone_number.length();

            while (phone_length != 10){
               cout << endl;
               cout << "                Enter a valid phone number: ";
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
            cout << "  ENTER YOUR PASSWORD : ";
            cin >> password;
            string pw = password;
            cout << "\n  REENTER YOUR PASSWORD :";
            cin >> password_1;
            string pw1 = password_1;
            if(pw1==pw){
              cout << "\n  Your password set. you are a member now !!\n";}
              else{
               cout << "\n  Your password does not match. Try again !!\n";
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
/*cout<<"press 1 to login  page\n";
  repeat:
    int choice;
      
cin>>choice;
if(choice==1)
              membership();

else
cout<<"enter 1 only to back to menu\n";
goto repeat;
*/
}
void login(){
    fstream member1;
    char pass_input[MAXIMUM_PSW];
    string line,password_input,name ,name_input, email1, password1;
    int login_attempt = 0, count = 0 ,  success=0;
    char  resp;

    name_check:
    member1.open("Membership.txt",ios::in);
    cout << "                                          Enter your name for login : ";
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
        cout << "                                  Your Name not found !!!"<< endl;

        goto name_check;
    }

     password1=PASSWORD;


    cout << endl;
    cout << "                                          Enter  password : ";
    cin>>pass_input;

     for (login_attempt = 1 ; login_attempt <= MAXIMUM_ATTEMPT ; login_attempt ++){
        if (pass_input ==password1 ){
login_check=1;
            cout << "Login Successful !!!";

            break;
        }
	 cout << endl;
        cout << "Login Failed. Attempt " << login_attempt  << " of 3" << endl;
        cout << "Please re-enter Password: " ;

        cin>>pass_input;

        if (pass_input == password1){
login_check=1;
            cout << "Login Successful !!!";

                  
                break;
        }
    }
     if ( login_attempt == MAXIMUM_ATTEMPT){
        cout << endl;
        cout << "Login Failed. Attempt 3 of 3" << endl;
 
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
        cout << "\nEnter a valid response: ";
        cin >> resp;
    }

    if (resp == 'a'){

       newmembership();
    }
      else{
        login();
    }
     
}
};

void data_connection_send(unsigned long int dataPort,int sockfd,char* filename){
    struct sockaddr_in clientAddr;
    int clientfd;
    // creating data socket for data connection 
    clientfd = socket(AF_INET,SOCK_STREAM,0);
    if(clientfd < 0){
        cout<<"Error in creating socket for data connection\n";
        return;
    }
    memset(&clientAddr,0,sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = dataPort;

    //binding the socket with the corresponding data port
    int binding = bind(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
    if(binding < 0){
        cout<<"Error in binding in PUT\n";
        return;
    }
//listening for the server request 
    int lis = listen(clientfd,20);
    if(lis < 0){
        cout<<"Error in listening in PUT\n";
        return;
    }
    struct sockaddr_in serverAddr;
    socklen_t l = sizeof(serverAddr);
    // accepting the request from the server
    int acc = accept(clientfd,(struct sockaddr*) &serverAddr,&l);
	cout<<"Accepted\n";
    int already_exist = 0,overwrite = 1,filehandle;
    char filename_path[50];
    strcpy(filename_path,path);
    strcat(filename_path,filename);
    filehandle = open(filename_path,O_RDONLY);
    // receving whether the file already exists in the server or not 
    recv(sockfd,&already_exist,sizeof(int),0);
if(already_exist){
        cout<<filename<<"file already exists in server \n press 1 for OVERWRITE \n press 0 for NO OVERWRITE\n";
        l:scanf("%d",&overwrite);
        if(overwrite != 0 && overwrite != 1){
            cout<<"Invalid type 0 or 1\n";
            goto l;
        }
    }
//sending the overwrite option over control connection 
    send(sockfd,&overwrite,sizeof(int),0);
	char data[10];
	memset(data,0,sizeof(data));
    if(overwrite == 1){

        struct stat obj;
        int status,size;
        stat(filename_path,&obj);
        size = obj.st_size;
		sprintf(data,"%d",size);
		send(sockfd,data,10,0);
        //send(sockfd,&size,sizeof(int),0);
 sendfile(acc,filehandle,NULL,size);
        recv(sockfd,&status,sizeof(int),0);
        if(status){
            cout<<"File successful PUT in the server\n";
        }
        else cout<<"File not PUT successfully in the server\n";
    }
    close(clientfd);
    close(acc);
}


int main()
{
	client OBJ;

int sockfd,serverSocket ;

	sockfd= socket(AF_INET , SOCK_STREAM , 0);
	if(sockfd < 0)
        {
		cout<< "Creation of client socket failed\n" ;
		return 0;
        }
	 cout<<"socked creted\n";

	struct sockaddr_in serverAddr , clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);
	if(connect(sockfd ,  (struct sockaddr*) & serverAddr , sizeof(serverAddr)) < 0)
	{
		cout<< "Connection Error...\n";
		return 0;
	}
	else
	{
		 cout<<"Connection Established...\n";

       OBJ.membership();


     if(login_check==1){
    cout<<"successfull\n";
    send(sockfd,&login_check,sizeof(int),0);



 while(1){
        char filename[50],comm[20],filename_path[50];
       cout<<"Enter the choice: command <filename>\n";
        // Taking input for command and filename 
        //scanf("%s %s",comm,filename);
       cin>>comm,filename;

        char buffer_comm[100],buffer_soc[20];
        memset(buffer_comm,0,sizeof(buffer_comm));
		memset(buffer_soc,0,sizeof(buffer_soc));
        strcpy(buffer_comm,comm);
        strcpy(filename_path,path);
        strcat(filename_path,filename);
		char newname[20];
		memset(newname,0,sizeof(newname));
        if(strcmp(comm,"PUT") == 0){
            // checking whether the file exists or not in the client side
            if(access(filename_path,F_OK) == -1){
               cout<< filename<<"does not exist in client side\n";
            }
else{
            // sending filename to the server
			send(sockfd,filename,20,0);
            // generating an arbitrary port number
            unsigned long int dataPort = rand()%1000;
			strcat(buffer_comm,":");
			dataPort = dataPort + (long int)10000;

            sprintf(buffer_soc,"%ld",dataPort);

			strcat(buffer_comm,buffer_soc);
			strcat(buffer_comm,"#");
            //sending the command and data port over the control connection 
            send(sockfd,buffer_comm,sizeof(buffer_comm),0);
            data_connection_send(dataPort,sockfd,filename);
            cout<<"File PUT successful\n";
            }
	}
        



else{
            printf("command should be in form of GET,PUT,MGET,MPUT");
        }
}
}

else
{
send(sockfd,&login_check,sizeof(int),0);
}
}
}




