#include <stdio.h>
#include <pthread.h>
#include <time.h>

void* performThread(void* data);

int count = 0;

int main(int argc, char* argv[]) {
  pthread_t threads[3];
  char threadNames[3][128] = {
    {"thread1"},
    {"thread2"},
    {"thread3"}
  };

  pthread_create(&threads[0], NULL, performThread, (void*)threadNames[0]);
  pthread_create(&threads[1], NULL, performThread, (void*)threadNames[1]);
  pthread_create(&threads[2], NULL, performThread, (void*)threadNames[2]);

  printf("%d\n", count);

  return 0;
}

void* performThread(void* data) {
  time_t currentTime;
  struct tm* timeInfo;
  char currentTimeString[128];

  char* threadName = (char*)data;
  FILE* file;
  int i = 0;

  file = fopen("event.log", "a");

  for (i = 0; i < 100000; i++) {
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);

    fprintf(file, "%s\t%s\t%d\n", currentTimeString, threadName, count);

    count++;
  }

  fclose(file);
}