CFLAGS = -Wall -g  # g++ ../source/server.cpp -o server
CXX = g++ -fprofile-arcs -ftest-coverage
LIBS =  -c
C=-c
SRS = ../source
OBJ = ../object
CFLAG = -o
EXES = server
EXEC = client
all:${EXES} ${EXEC} 

server.gcnno:
	g++ -fprofile-arcs -ftest-coverage -c ../source/server.cpp

${OBJ}/server.o: ${SRS}/server.cpp
	${CXX} ${C} ${SRS}/server.cpp

${OBJ}/client.o: ${SRS}/client.cpp
	${CXX} ${LIBS} ${SRS}/client.cpp



server:${OBJ}/server.o
	${CXX} ${CFLAGS} ${CFLAG} ${EXES} ${OBJ}/server.o

client:${OBJ}/client.o
	${CXX} ${CFLAGS} ${CFLAG} ${EXEC} ${OBJ}/client.o


clean:
	rm -f *.o *.gcno
