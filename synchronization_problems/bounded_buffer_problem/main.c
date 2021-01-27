#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t mutex;
sem_t full;
sem_t empty;
int buffer[16];
int consumer_idx = 0;

void* producer(void* arg)
{
  int i = 0, idx=0;
  while (i < 40) {
    sem_wait(&empty);
    sem_wait(&mutex);
    idx = (idx+1)%16;
    buffer[idx] = i;
    printf("produced : %d\n", i);
    sem_post(&mutex);
    sem_post(&full);
    i++;
  }
  return NULL;
}

void* consumer(void* arg)
{
  int i = 0;
  while (i < 20) {
    sem_wait(&full);
    sem_wait(&mutex);
    consumer_idx = (consumer_idx+1)%16;
    printf("consumed : %d\n", buffer[consumer_idx]);
    sem_post(&mutex);
    sem_post(&empty);
    i++;
    sleep(1);
  }
  return NULL;
}

int main()
{
  pthread_t t1, t2, t3;
  sem_init(&mutex, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, 16);
  pthread_create(&t1, NULL, producer, NULL);
  pthread_create(&t2, NULL, consumer, NULL);
  pthread_create(&t3, NULL, consumer, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  sem_close(&mutex);
  sem_close(&full);
  sem_close(&empty);
}
