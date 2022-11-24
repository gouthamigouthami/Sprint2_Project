#system_programming project

These functionalities have been implemented in the the program :

PUT : Transfer a file from client to server

GET : Transfer a file from server to client

MGET (example : MGET .txt): Transfer all files of a given extension from server to client

MPUT (example : MPUT .txt): Transfer all files of a given extension from client to server

File overwrite feature has been implemented along with it i.e. if the file is already present at the destination, then the client will be asked whether to overwrite the file or not.

Compiling the code :
go to Makefile directory and type make commad it will create to excutable files i.e server and client 

Commands : Enter a choice: GET PUT MGET MPUT QUIT

Closing Client and Server Use command “QUIT server” to quit client program. This will disconnect the client. 
