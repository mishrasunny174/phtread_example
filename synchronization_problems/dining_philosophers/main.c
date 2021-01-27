#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define NUM_OF_PHILOSOPHERS 5

int rice = 0x1337;
sem_t chopsticks[NUM_OF_PHILOSOPHERS];

void* philosopher(void * idx)
{
  int phil_id = (long)idx;
  for(int i=0; i<0x5; i++)
  {
    sem_wait(&chopsticks[phil_id]);
    sem_wait(&chopsticks[(phil_id+1)%NUM_OF_PHILOSOPHERS]);
    printf("[philospher %2d] eating rice\n", phil_id);
    sleep(1);
    sem_post(&chopsticks[phil_id]);
    sem_post(&chopsticks[(phil_id+1)%NUM_OF_PHILOSOPHERS]);
    printf("[philospher %2d] thinking...\n", phil_id);
    sleep(1);
  }
  return NULL;
}

int main()
{
  pthread_t threads[NUM_OF_PHILOSOPHERS];
  for(int i=0; i<NUM_OF_PHILOSOPHERS; i++)
  {
    sem_init(&chopsticks[i], 0, 1);
  }
  for(int i=0; i<NUM_OF_PHILOSOPHERS; i++)
  {
    pthread_create(&threads[i], NULL, philosopher, (void *)(long)i);
  }
  for(int i=0; i<NUM_OF_PHILOSOPHERS; i++)
  {
    pthread_join(threads[i], NULL);
  }
  for(int i=0; i<NUM_OF_PHILOSOPHERS; i++)
  {
    sem_destroy(&chopsticks[i]);
  }
  return 0;
}
