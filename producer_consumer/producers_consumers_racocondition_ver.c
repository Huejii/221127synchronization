#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MAX_CIRCULAR_SIZE 20             // 링버퍼가 갖는 아이템 개수
#define TEAM_THREAD_SIZE 5

/*
    2개의 팀이 있다. 각 팀에는 circular buffer가 한개씩 부여된다.
    각 팀의 thread는 상대 팀의 버퍼에서 자신의 버퍼로 숫자 1~20이 적힌 아이템을 가져온다.
    게임이 종료되는 순간 버퍼에 더 많은 아이템이 있는 팀이 승리한다.
    첫 시작은 팀당 숫자 10개씩  각각 1~10, 11~20이 적힌 아이템을 부여한다.

    즉, 이 게임에서 각 thread는 상대 팀에 있는 데이터를 지울 때 consumer 역할을 하고,
    데이터를 가져와서 자신의 팀에 입력할 때 producer 역할을 한다.
*/

typedef struct{
  int head;                         // 쓰기 위치
  int tail;                         // 읽기 위치
  int item[MAX_CIRCULAR_SIZE];        // CIRCULAR 내의 저장 장소
} circular_t;        // circular buffer

// 팀당 circular buffer가 한개씩 부여
circular_t* teamA_buffer;
circular_t* teamB_buffer;

/*스레드 ID 선언*/
pthread_t A_thread[5], B_thread[5];

// 스레드의 이름 배열 선언 및 초기화
char* A_name[TEAM_THREAD_SIZE] = {"A_player01", "A_player02", "A_player03", "A_player04", "A_player05"}; //reader
char* B_name[TEAM_THREAD_SIZE] = {"B_player01", "B_player02", "B_player03", "B_player04", "B_player05"}; //writer

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;
FILE* file;


void circular_init();
void* teamA_get_item();
void* teamB_get_item();

void circular_init()
{
    for(int i =0; i < 10; i++)
    {
        teamA_buffer->item[i] = i+1;
    }

    for(int i =0; i < 10; i++)
    {
        teamB_buffer->item[i] = i+11;
    }
}

void* teamA_get_item();
void* teamB_get_item();

int main()
{

    printf("// Notion: %s\n", S);

    /*thread create*/
    for(int i = 0; i <TEAM_THREAD_SIZE; i++)
    {
        pthread_create(&reader[0],NULL,reader_task,(void*)readerName[0]);
        pthread_create(&writer1, NULL,writer_221225,(void*)writerName[0]);
    }

    // thread 종료
    for(int i = 0; i<5; i++)
    {
        pthread_join(reader[i],NULL);
    }
    pthread_join(writer1,NULL);
    pthread_join(writer2,NULL);
    pthread_join(writer3,NULL);

    fclose(file);
    return 0;
}