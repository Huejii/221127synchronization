#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define READER_SIZE 5

/*스레드 ID 선언*/
pthread_t reader[5], writer1, writer2, writer3;

// 스레드의 이름 배열 선언 및 초기화
char* readerName[5] = {"reader01", "reader02", "reader03", "reader04", "reader05"}; //reader
char* writerName[3] = {"writer_221225", "writer_221231", "writer_230101"}; //writer

/*reader와 writer가 접근할 문자열 선언*/
char* S = "Thanks giving day~!";

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;
FILE* file;

/*함수 선언*/
void *reader_task(void* name);
void *writer_221225(void* name);
void *writer_221231(void* name);
void *writer_230101(void* name);


/*semaphore variable 선언*/
int readcount = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shared_data_mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    printf("// Notion: %s\n", S);

    /*thread create*/
    pthread_create(&reader[0],NULL,reader_task,(void*)readerName[0]);
    pthread_create(&writer1, NULL,writer_221225,(void*)writerName[0]);
    pthread_create(&reader[1],NULL,reader_task,(void*)readerName[1]);
    pthread_create(&writer2,NULL,writer_221231,(void*)writerName[1]);
    pthread_create(&reader[2],NULL,reader_task,(void*)readerName[2]);
    pthread_create(&writer3,NULL,writer_230101,(void*)writerName[2]);
    pthread_create(&reader[3],NULL,reader_task,(void*)readerName[3]);
    pthread_create(&reader[4],NULL,reader_task,(void*)readerName[4]);

    // thread 종료
    for(int i = 0; i<5; i++)
    {
        pthread_join(reader[i],NULL);
    }
    pthread_join(writer1,NULL);
    pthread_join(writer2,NULL);
    pthread_join(writer3,NULL);

    return 0;
}

// reader start routine
void *reader_task(void* name)
{
    int i = 0;
    int more_read = 100;
    char* T = "November 24th";
    char* M = "December 25th";
    char* G = "December 31th";
    char* H = "January 1st";

    do {
        pthread_mutex_lock(&mutex);

        readcount++;
        if(readcount == 1) pthread_mutex_lock(&shared_data_mutex);
        pthread_mutex_unlock(&mutex);
        // 10번 수행한다.

        // 문자열 read 및 출력
        switch(S[0])
        {
            case 'T': {printf("%s thread id: %lx\tOh, It's %s\n",(char*)name, pthread_self(), T); break;}
            case 'H': {printf("%s thread id: %lx\tOh, It's %s\n",(char*)name, pthread_self(), H); break;}
            case 'G': {printf("%s thread id: %lx\tOh, It's %s\n",(char*)name, pthread_self(), G); break;}
            case 'M': {printf("%s thread id: %lx\tOh, It's %s\n",(char*)name, pthread_self(), M); break;}
        }

        more_read--;
        pthread_mutex_lock(&mutex);
        readcount--;
        if(readcount == 0) pthread_mutex_unlock(&shared_data_mutex);
        pthread_mutex_unlock(&mutex);
    } while (more_read);
}

// writer1 start routine
void *writer_221225(void* name)
{
    char* N = "Merry christmas~!";   // S 문자열을 변경할 dest 문자열
    int more_write = 10;

    do{
        pthread_mutex_lock(&shared_data_mutex);
        // 10번 수행한다.

        // 문자열 변경(write)
        S = N;
        // 진행 확인을 위한 출력
        printf("%s thread id: %lx\t // Notion: %s \n",(char*)name, pthread_self(), S);
        
        pthread_mutex_unlock(&shared_data_mutex);
        // 10번 수행한다.
        more_write--;
    }while(more_write);
}

// writer2 start routine
void *writer_221231(void* name)
{
    char* N = "Goodbye 2022~!";    // S 문자열을 변경할 dest 문자열
    int more_write = 10;
    
    do{
        pthread_mutex_lock(&shared_data_mutex);
        // 10번 수행한다.

        // 문자열 변경(write)
        S = N;
        // 진행 확인을 위한 출력
        printf("%s thread id: %lx\t // Notion: %s\n",(char*)name, pthread_self(), S);
        
        pthread_mutex_unlock(&shared_data_mutex);
        more_write--;
    }while(more_write);
}

// writer2 start routine
void *writer_230101(void* name)
{
    char* N = "Happy New Year~!";   // S 문자열을 변경할 dest 문자열
    int more_write = 10;
    
    do{
        pthread_mutex_lock(&shared_data_mutex);
        // 10번 수행한다.

        // 문자열 변경(write)
        S = N;
        // 진행 확인을 위한 출력
        printf("%s thread id: %lx\t // Notion: %s\n",(char*)name, pthread_self(), S);

        pthread_mutex_unlock(&shared_data_mutex);

        // 10번 수행한다.
        more_write--;
    }while(more_write);
}