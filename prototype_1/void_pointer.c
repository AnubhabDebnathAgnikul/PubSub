#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define BUFFER_SIZE 256

/*topic struct*/
typedef struct
{
    int n;
    void * m;
}my_struct;

/*message structure*/
typedef struct 
{
    int x;
    char y[BUFFER_SIZE];
}message;

/*function server*/
int fun_a(void *arguments)
{
    my_struct *args = (my_struct *)arguments;

    printf("id %d\n", args->n);
    /*demo*/
    message *msg_pack = (message *)args->m;
    printf("int value: %d | message value: %s\n", msg_pack->x, msg_pack->y);
}

/*function trigger*/
int fun_b(void *arguments, void *packet)
{
    my_struct *args = (my_struct *)arguments;
    
    args->m = packet;
}

/*initializer*/
my_struct* fun_init()
{
    my_struct *args = malloc(sizeof(my_struct));

    if (args == NULL) 
    {
        perror("Failed to allocate memory for arguments\n");
        exit(EXIT_FAILURE);
    }

    args->n = 1;

    return args;
}

/*driver*/
int main(void *arguments)
{
    message *msg = malloc(sizeof(message));
    if (msg == NULL) 
    {
        perror("Failed to allocate memory for arguments\n");
        exit(EXIT_FAILURE);
    }

    msg->x = 19;
    strcpy(msg->y, "Anubhab");

    my_struct *topic = fun_init();

    fun_b(topic,msg);

    fun_a(topic);
}


