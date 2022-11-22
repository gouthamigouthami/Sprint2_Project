#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <cstdio>
#define buffer 30
using namespace std;

int main()
{
        int spe;
lab:
       cout<<"enter choice:";
        cin>>spe;

        while(spe!=0){
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key,10244,06666|IPC_CREAT);

    // shmat to attach to shared memory
    char *str = (char*) shmat(shmid,(void*)0,0);

    cout<<"Write Data : ";
    fgets(str, sizeof(buffer), stdin);
fgets(str, sizeof(buffer), stdin);


    printf("Data written in memory: %s\n",str);


    //detach from shared memory
    shmdt(str);
    
    // ftok to generate unique key
    key_t key1 = ftok("Gouthami",65);

    // shmget returns an identifier in shmid
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT);

    // shmat to attach to shared memory
    char *str1 = (char*) shmat(shmid,(void*)0,0);

    printf("Data read from memory: %s\n",str1);

    //detach from shared memory
    shmdt(str1);

    // destroy the shared memory
    shmctl(shmid,IPC_RMID,NULL);
    goto lab;

        }


    return 0;
}

