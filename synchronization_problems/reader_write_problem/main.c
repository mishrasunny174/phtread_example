#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t mutex;
sem_t wrt;
int shared;
int readers_count;

void* writer(void* arg)
{
  for (int i = 0; i < 10; i++) {
    printf("[writer] waiting for readers to become 0\n");
    sem_wait(&wrt);
    printf("[writer] 0 readers reading hence locking wrt to write the shared data\n");
    shared = i;
    printf("[writer] Writing : %d\n", i);
    sleep(2);
    printf("[writer] unlocking wrt\n");
    sem_post(&wrt);
  }
  return NULL;
}

void* reader(void* arg)
{
  for(int i=0; i<5; i++)
  {
    sem_wait(&mutex);
    readers_count++;
    if (readers_count == 1)
    {
      printf("[reader] waiting for writer to write stuff\n");
      sem_wait(&wrt);
    }
    sem_post(&mutex);
    printf("[reader] %d readers reading : %d\n", readers_count, shared);
    sleep(1);
    sem_wait(&mutex);
    readers_count--;
    if(readers_count == 0)
    {
      printf("[readers] 0 readers reading unlocking wrt\n");
      sem_post(&wrt);
    }
    sem_post(&mutex);
  }
  return NULL;
}

int main()
{
  sem_init(&mutex, 0, 1);
  sem_init(&wrt, 0, 1);
  pthread_t reader_threads[4], writer_threads[2];
  for(int i=0; i<2; i++)
  {
    pthread_create(&writer_threads[i], NULL, writer, NULL);
  }
  for(int i=0; i<4; i++)
  {
    pthread_create(&reader_threads[i], NULL, reader, NULL);
  }

  for(int i=0; i<2; i++)
  {
    pthread_join(writer_threads[i], NULL);
  }
  for(int i=0; i<4; i++)
  {
    pthread_join(reader_threads[i], NULL);
  }
  return 0;
}
