#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* thread_function(void* arg) {
    while (1) 
    {
        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) 
        {
            pthread_cond_wait(&cond, &mutex);
        }
        int value = rand() % 100; // Generate some random value to push into the buffer
        buffer[count++] = value;
        printf("Pushed %d into the buffer\n", value);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void push_value_into_thread(int value) 
{
    pthread_mutex_lock(&mutex);
    while (count == BUFFER_SIZE) 
    {
        pthread_cond_wait(&cond, &mutex);
    }
    buffer[count++] = value;
    printf("Pushed %d into the buffer\n", value);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_function, NULL);

    // Wait for a while to let the thread run
    sleep(2);

    // Now push some value into the thread
    push_value_into_thread(50);

    // push_value_into_thread(20);

    // Wait for the thread to complete (which won't happen in this example)
    pthread_join(tid, NULL);

    return 0;
}