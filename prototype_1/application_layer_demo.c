#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

#define BUFFER_SIZE 256

/**
 * @brief Structure to represent a topic
 * 
 * This structure holds information related to a topic, including its name,
 * message buffer, trigger flag, thread ID of the server, and termination flag.
 */
typedef struct {
    _Atomic int trig; /**< Trigger flag */
    char message[BUFFER_SIZE]; /**< Message buffer */
    pthread_t server_thread_id; /**< Thread ID of the server */
    char topic[BUFFER_SIZE]; /**< Topic name */
    _Atomic int kill; /**< Termination flag */
} topic_t;


/**
 * @brief Server function for a topic
 * 
 * This function runs in a loop, handling messages for the specified topic.
 * It checks the trigger flag to send messages and the termination flag to exit.
 * 
 * @param arguments Pointer to the topic_t structure
 * @return NULL
 */
void* fun_server(void *arguments) 
{
    topic_t *args = (topic_t *)arguments;
    while(1) 
    {
        if (atomic_load(&(args->trig)) == 1) 
        {
            printf("Sending message from %s server: %s\n",args->topic, args->message);
            atomic_store(&(args->trig), 0);
        }

        if(atomic_load(&(args->kill))!=0)
        {
            printf("%s KILL RECEIVED!!!\n", args->topic);
            pthread_exit(NULL);
        }
    }
}


/**
 * @brief Trigger function to publish a message for a topic
 * 
 * This function prompts the user to enter a message for the specified topic
 * and sets the trigger flag to notify the server thread.
 * 
 * @param arguments Pointer to the topic_t structure
 * @return EXIT_SUCCESS on success, EXIT_FAILURE if the topic is terminated
 */
int publish_message(void *arguments) 
{
    topic_t *args = (topic_t *)arguments;

    if (atomic_load(&(args->kill)) != 0)
    {
        printf("TOPIC is not live");
        // exit(EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    printf("Enter message for %s: ", args->topic);
    fgets(args->message, BUFFER_SIZE, stdin);

    atomic_store(&(args->trig), 1);

    while(1)
    {
        if (atomic_load(&(args->trig)) == 0)
        {
            return EXIT_SUCCESS;
        }
    }

}


/**
 * @brief Initializes a topic
 * 
 * This function creates a new topic with the given name and starts its server thread.
 * 
 * @param topic_name Name of the topic
 * @return Pointer to the created topic_t structure
 */
topic_t* publisher_init(char *topic_name) 
{
    pthread_t server_thread_id;
    topic_t *args = malloc(sizeof(topic_t));

    if (args == NULL) 
    {
        perror("Failed to allocate memory for arguments\n");
        exit(EXIT_FAILURE);
    }

    args->trig = 0;
    args->server_thread_id = server_thread_id;
    args->kill = 0;
    strcpy(args->topic,topic_name);    

    pthread_create(&server_thread_id, NULL, fun_server, args);
    pthread_detach(server_thread_id);

    return args;
}


/**
 * @brief Terminates a topic
 * 
 * This function sets the termination flag for the specified topic,
 * signaling the server thread to exit, and frees the memory allocated for the topic.
 * 
 * @param arguments Pointer to the topic_t structure
 */
int kill_topic(void *arguments)
{
    topic_t *args = (topic_t *)arguments;
    atomic_store(&(args->kill), 1);

    // free the topic struct 
    free(args);
}


/**
 * @brief Main function
 * 
 * This function demonstrates the usage of topics by initializing them,
 * publishing messages, and terminating them.
 * 
 * @return EXIT_SUCCESS on success
 */
int main() 
{
    /*TOPIC 1*/
    topic_t *TOPIC_1 = publisher_init("TOPIC_1");
    if(publish_message(TOPIC_1) == EXIT_SUCCESS)
    {
        printf("trigger success\n");
    }
    

    /*TOPIC 2*/
    topic_t *TOPIC_2 = publisher_init("TOPIC_2");
    if(publish_message(TOPIC_2) == EXIT_SUCCESS)
    {
        printf("trigger success\n");
    }

    sleep(2);

    kill_topic(TOPIC_1);

    kill_topic(TOPIC_2);

    sleep(2);


    if(publish_message(TOPIC_2) == EXIT_SUCCESS)
    {
        printf("trigger success\n");
    }
    else
    {
        printf("trigger failure\n");   
    }

    /*TOPIC 2*/
    TOPIC_2 = publisher_init("TOPIC_2");
    if(publish_message(TOPIC_2) == EXIT_SUCCESS)
    {
        printf("trigger success\n");
    }

    kill_topic(TOPIC_2);
}