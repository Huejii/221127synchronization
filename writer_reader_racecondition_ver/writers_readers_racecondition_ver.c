#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#define READER_SIZE 5

/*스레드 ID 선언*/
pthread_t reader[5], writer_upper, writer_lower;

// 스레드의 이름 배열 선언 및 초기화
char* readerName[5] = {"reader01", "reader02", "reader03", "reader04", "reader05"}; //reader
char* writerName[2] = {"writer_221231", "writer_230101"}; //writer

/*semaphore variable 선언*/
// sem_t result;

/*reader와 writer가 접근할 문자열 선언*/
char* S = "Happy Merry Christmas~! ";

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;
FILE* file;

/*함수 선언*/
void *reader_task(void* name);
void *writer_221231(void* name);
void *writer_230101(void* name);

int main()
{
    file = fopen("event.log", "a");
    /*thread create*/
    pthread_create(&reader[0],NULL,reader_task,(void*)readerName[0]);
    pthread_create(&writer_upper,NULL,writer_221231,(void*)writerName[0]);
    pthread_create(&reader[1],NULL,reader_task,(void*)readerName[1]);
    pthread_create(&reader[2],NULL,reader_task,(void*)readerName[2]);
    pthread_create(&writer_lower,NULL,writer_230101,(void*)writerName[1]);
    pthread_create(&reader[3],NULL,reader_task,(void*)readerName[3]);
    pthread_create(&reader[4],NULL,reader_task,(void*)readerName[4]);


    for(int i = 0; i<5; i++)
    {
        pthread_join(reader[i],NULL);
    }
    pthread_join(writer_upper,NULL);
    pthread_join(writer_lower,NULL);

    fclose(file);
    return 0;
}

void *reader_task(void* name)
{
    printf("reader pid: %xd", getpid());
    time_t currentTime;
    struct tm* timeInfo;
    char currentTimeString[128];
    char* temp;
    file = fopen("event.log", "a");
    int i = 0;
    for (i = 0; i < 100; i++) {
        time(&currentTime);
        timeInfo = localtime(&currentTime);
        strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);
        sleep(1);
        printf("reader pid: %x\t%s\n",pthread_self(), S);
        fprintf(file, "%s\t%s\t%s\t%d\n", currentTimeString, (char*)name, S, count);
        count++;
    }
}

void *writer_221231(void* name)
{
        time_t currentTime;
    struct tm* timeInfo;
    char currentTimeString[128];
    char* N = "Goodbye 2022~!";

    file = fopen("event.log", "a");
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);
    S = N;
    printf("writer1 pid: %x\t%s\n",pthread_self(), S);
    fprintf(file, "%s\t%s\t%s\t%d\n", currentTimeString, (char*)name, S, count);
    count++;
}

void *writer_230101(void* name)
{
    time_t currentTime;
    struct tm* timeInfo;
    char currentTimeString[128];
    char* N = "Happy New Year~!";
    
    file = fopen("event.log", "a");
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);
    S = N;
    printf("writer pid: %x\t%s\n",pthread_self(), S);
    fprintf(file, "%s\t%s\t%s\t%d\n", currentTimeString, (char*)name, S, count);
}