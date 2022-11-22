#include<unistd.h>
#include<pthread.h>
#include<stdio.h>

int first()
{
        int i;
        for(i=0;;i++){
                printf("\nFirst: %d",i);
                sleep(1);
        }}
int main()
{
        pthread_t th;
        int i;
        pthread_create(&th, 0,(void *)&first,NULL);
        for(i=0;;i++)
        {
                printf("\nMain: %d",i);
                sleep(1);
        }
        pthread_join(th, NULL);
        return  0;
}

