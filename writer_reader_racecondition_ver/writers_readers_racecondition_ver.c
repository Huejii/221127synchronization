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
char* readerName[5];
char* writerName[2];

/*semaphore variable 선언*/
// sem_t result;

/*reader와 writer가 접근할 문자열 선언*/
char* S = "Happy Merry Christmas~!";

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;

/*함수 선언*/
void upper();
void lower();
void *reader_task();
void *writer_upper_task();
void *writer_lower_task();
int data_logger(char* name, void task());

int main()
{
        // reader의 이름 배열 선언 및 초기화 Ex) reader1, reader2, ...
    char* readerName[5];
    for(int i = 0; i<5; i++)
    {
        readerName[i] = strcat("reader", itoa(i+1));
    }

    // writer의 이름 배열 선언 및 초기화
    char* writerName[2] = {"writer_upper", "writer_lower"};

    /*thread create*/
    pthread_create(&reader[0],NULL,data_logger,reader_task(readerName[0]));
    pthread_create(&reader[1],NULL,data_logger,reader_task(readerName[1]));

    pthread_create(&writer_upper,NULL,data_logger,writer_upper_task(writerName[0]));  // 데이터를 대문자로 바꾸는 writer => HAPPY MERRY CHRISTMAS~!

    pthread_create(&reader[2],NULL,data_logger,reader_task(readerName[2]));
    pthread_create(&reader[3],NULL,data_logger,reader_task(readerName[3]));

    pthread_create(&writer_lower,NULL,data_logger,writer_upper_task(writerName[1]));    // 데이터를 소문자로 바꾸는 writer => happy merry christmas~!

    pthread_create(&reader[4],NULL,data_logger,reader_task(readerName[4]));


    for(int i = 0; i<5; i++)
    {
        pthread_join(reader[i],NULL);
    }
    pthread_join(writer_upper,NULL);
    pthread_join(writer_lower,NULL);
    return 0;
}

void *reader_task(char* reader_name)
{
    printf("%s", S);
}

void *writer_upper_task()
{
    int i;
    for (i = 0; S[i] != NULL; i++) {
        if (S[i] >= 'a' && S[i] <= 'z')
            S[i] -= 'a' - 'A';
    }
}

void *writer_lower_task(void * writer_name)
{
    int i;
    for (i = 0; S[i] != NULL; i++) {
        if (S[i] >= 'A' && S[i] <= 'Z')
            S[i] += 'a' - 'A';
    }
}

void upper() {
    int i;
    for (i = 0; S[i] != NULL; i++) {
        if (S[i] >= 'a' && S[i] <= 'z')
            S[i] -= 'a' - 'A';
    }
}

// 소문자로 바꿔주는 함수
void lower() {
    int i;

    for (i = 0; S[i] != NULL; i++) {
        if (S[i] >= 'A' && S[i] <= 'Z')
            S[i] += 'a' - 'A';
    }
}

int data_logger(char* name, void task())
{
    time_t currentTime;
    struct tm* timeInfo;
    char currentTimeString[128];

    FILE* file;
    int i = 0;
        file = fopen("event.log", "a");

    for (i = 0; i < 100; i++) {
        time(&currentTime);
        timeInfo = localtime(&currentTime);
        strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);
        task(name);
        fprintf(file, "%s\t%s\t%s\t%d\n", currentTimeString, (char*)name, S, count);

        count++;
    }
}