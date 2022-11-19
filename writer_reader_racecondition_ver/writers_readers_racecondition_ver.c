#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <ctype.h>
#define READER_SIZE 5

/*스레드 ID 선언*/
pthread_t reader[5], writer_upper, writer_lower;

// 스레드의 이름 배열 선언 및 초기화
char* readerName[5] = {"reader01", "reader02", "reader03", "reader04", "reader05"}; //reader
char* writerName[2] = {"writer_upper", "writer_lower"}; //writer

/*semaphore variable 선언*/
// sem_t result;

/*reader와 writer가 접근할 문자열 선언*/
char* S = "Happy Merry Christmas~! ";

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;

FILE* shrfile;
/*함수 선언*/
void *reader_task(void* name);
void *writer_upper_task(void* name);
void *writer_lower_task(void* name);
void upper();
void lower();

int main()
{
    /*thread create*/
    pthread_create(&reader[0],NULL,reader_task,(void*)readerName[0]);
    
    pthread_create(&writer_upper,NULL,writer_upper_task,(void*)writerName[0]);  // 데이터를 대문자로 바꾸는 writer => HAPPY MERRY CHRISTMAS~!
    
    pthread_create(&reader[1],NULL,reader_task,(void*)readerName[1]);
    pthread_create(&reader[2],NULL,reader_task,(void*)readerName[2]);

    pthread_create(&writer_lower,NULL,writer_lower_task,(void*)writerName[1]);    // 데이터를 소문자로 바꾸는 writer => happy merry christmas~!
   
    pthread_create(&reader[3],NULL,reader_task,(void*)readerName[3]);
    pthread_create(&reader[4],NULL,reader_task,(void*)readerName[4]);


    for(int i = 0; i<5; i++)
    {
        pthread_join(reader[i],NULL);
    }
    pthread_join(writer_upper,NULL);
    pthread_join(writer_lower,NULL);
    return 0;
}

void *reader_task(void* name)
{
    time_t currentTime;
    struct tm* timeInfo;
    char currentTimeString[128];
    char* temp;

    FILE* file;
    int i = 0;
        file = fopen("event.log", "a");

    for (i = 0; i < 10000; i++) {
        time(&currentTime);
        timeInfo = localtime(&currentTime);
        strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);
        printf("%s\n", S);
        fprintf(file, "%s\t%s\t%s\t%d\n", currentTimeString, (char*)name, S, count);
        count++;
    }

    fclose(file);
}

void *writer_upper_task(void* name)
{
        time_t currentTime;
    struct tm* timeInfo;
    char currentTimeString[128];

    FILE* file;
    int i = 0;
        file = fopen("event.log", "a");

    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);

    upper();
    fprintf(file, "%s\t%s\t%s\t%d\n", currentTimeString, (char*)name, S, count);

    count++;
    fclose(file);
}

void *writer_lower_task(void* name)
{
    time_t currentTime;
    struct tm* timeInfo;
    char currentTimeString[128];

    FILE* file;
    int i = 0;
    file = fopen("event.log", "a");

    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(currentTimeString, 128, "%Y-%m-%d %H:%M:%S", timeInfo);
    lower();
    fprintf(file, "%s\t%s\t%s\t%d\n", currentTimeString, (char*)name, S, count);

    count++;
    fclose(file);
}

//문자열을 대문자로 바꾸는 함수
void upper() {
    int i;
    for (int i = 0; i< strlen(S); i++) {
        if (S[i] >= 'a' && S[i] <= 'z')
            S[i] = toupper(S[i]);
    }
}

//문자열을 소문자로 바꾸는 함수
void lower() {
    int i;

    for (i = 0; i< strlen(S); i++) {
        if (S[i] >= 'A' && S[i] <= 'Z')
            S[i] = tolower(S[i]);
    }
}