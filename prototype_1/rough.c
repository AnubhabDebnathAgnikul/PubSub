#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 10


// Struct to hold multiple arguments
typedef struct {
    int trig;
    char message[BUFFER_SIZE];
} thread_args;

int fun_server(void *arguments)
{
    thread_args *args = (thread_args *)arguments;
    while(1)
    {
        /* Broadcasting Part*/
        if(args->trig == 1)
        {
            // printf("var: %d\n", args->trig);
            printf("sending message through server: %s\n", args->message);
            args->trig = 0;
        }
    }
}


/*Remove while loop in Flight code usage*/
int fun_trigger(void *arguments)
{
    thread_args *args = (thread_args *)arguments;
    while(1)
    {
        printf("enter message: \n");
        fgets(args->message,BUFFER_SIZE,stdin);
        args->trig = 1;
    }
}

// init process 
int main()
{
    pthread_t thread_id1, thread_id2;

    thread_args *arg_1 = malloc(sizeof(thread_args));

    if (arg_1 == NULL) 
    {
        perror("Failed to allocate memory for arguments\n");
        exit(EXIT_FAILURE);
    }

    arg_1->trig = 0;
    pthread_create(&thread_id1,NULL, (void *)fun_server, arg_1);
    pthread_create(&thread_id2,NULL, (void *)fun_trigger, arg_1);

    pthread_exit(NULL);
}