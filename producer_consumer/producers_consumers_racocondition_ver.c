#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_CIRCULAR_SIZE 90             // 링버퍼가 갖는 아이템 개수
#define TEAM_THREAD_SIZE 5

/*
    - 버퍼 게임
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
circular_t teamA = {.head=0, .tail=0};
circular_t teamB = {.head=0, .tail=0};
circular_t* teamA_buffer = &teamA;
circular_t* teamB_buffer = &teamB;

/*스레드 ID 선언*/
pthread_t A_thread[5], B_thread[5];

// 스레드의 이름 배열 선언 및 초기화
char* A_name[TEAM_THREAD_SIZE] = {"A_player01", "A_player02", "A_player03", "A_player04", "A_player05"};
char* B_name[TEAM_THREAD_SIZE] = {"B_player01", "B_player02", "B_player03", "B_player04", "B_player05"};

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;
char winner;

void circular_init();
void* teamA_get_item();
void* teamB_get_item();

/*semaphore variable 선언*/
pthread_mutex_t mutex =PTHREAD_MUTEX_INITIALIZER;	/* mutex lock for buffer */
pthread_cond_t A_cons = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t A_prod = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */
pthread_cond_t B_cons = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t B_prod = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

int main()
{

    int random = 0; // 정수형 변수 선언
    int i;
    
    circular_init();
    printf("// 버퍼게임 시작\n");
    srand(time(NULL));

    /*thread create*/
    for(i = 0; i <200; i++)
    {
        random = rand(); // 난수 생성
        random %= 2; // 난수 생성
        switch (random){
            case 0:
                {
                    pthread_create(&A_thread[i%TEAM_THREAD_SIZE],NULL,teamA_get_item,(void*)A_name[i%TEAM_THREAD_SIZE]);
                    break;
                }
            case 1:
                {
                    pthread_create(&B_thread[i%TEAM_THREAD_SIZE], NULL,teamB_get_item,(void*)B_name[i%TEAM_THREAD_SIZE]);
                    break;
                }
        }
        i++;
    }

    // thread 종료
    for(int i = 0; i<5; i++)
    {
        pthread_join(A_thread[i],NULL);
        pthread_join(B_thread[i],NULL);
    }

    if(teamA_buffer->head - teamA_buffer->tail == teamB_buffer->head - teamB_buffer->tail)
    {
        printf("동점입니다.\n");
    }
    else if(teamA_buffer->head - teamA_buffer->tail > teamB_buffer->head - teamB_buffer->tail)
    {
        printf("A팀의 승리입니다.\n");
    }
    else
    { 
        printf("B팀의 승리입니다.\n");
    }
    return 0;
}

void circular_init()
{
    for(int i =0; i < 50; i++)
    {
        // buffer에 데이터 저장
        teamA_buffer->item[teamA_buffer->head] = i+1;

        // head tail 조정
        teamA_buffer->head = ( teamA_buffer->head +1) % MAX_CIRCULAR_SIZE; // head 증가
        if ( teamA_buffer->head == teamA_buffer->tail ){ // 버퍼가 모두 찼다면
            printf("버퍼가 가득 찼습니다.");
        }

    }

    for(int i =0; i < 50; i++)
    {
        teamB_buffer->item[teamB_buffer->head] = i+51;

        // head tail 조정
        teamB_buffer->head = ( teamB_buffer->head +1) % MAX_CIRCULAR_SIZE; // head 증가
        if ( teamB_buffer->head == teamB_buffer->tail ){ // 버퍼가 모두 찼다면
            printf("버퍼가 가득 찼습니다.");
        }
    }

}

void* teamA_get_item(void* name)
{
int temp; // 데이터를 옮기기 위한 임시 저장소
    // 데이터 가져오기
    // 큐에 데이터가 없다면 복귀
    while(teamB_buffer->head == teamB_buffer->tail)
    {
        printf("A: B팀의 버퍼에 데이터가 없습니다.\n");
    }

    //consumer
    temp = teamB_buffer->item[teamB_buffer->tail];
    printf("%s thread id: %lx\t get B->A item %d\n",(char*)name, pthread_self(), temp);
    teamB_buffer->item[teamB_buffer->tail] = 0;
    teamB_buffer->tail = ( teamB_buffer->tail +1) % MAX_CIRCULAR_SIZE;  //B팀 tail 증가

    //producer
    teamA_buffer->item[teamA_buffer->head] = temp;
    teamA_buffer->head = (teamA_buffer->head +1) % MAX_CIRCULAR_SIZE; //A팀 head 증가
    // 진행 확인을 위한 출력
    printf("result: %s thread id: %lx\t // get B->A item %d, A_head: %d, A_tail: %d, B_head: %d, B_tail: %d \n",(char*)name, pthread_self(), temp, teamA_buffer->head, teamA_buffer->tail, teamB_buffer->head, teamB_buffer->tail);

    count++;
}

void* teamB_get_item(void* name)
{
    int temp; // 데이터를 옮기기 위한 임시 저장소
    // 데이터 가져오기
    // 큐에 데이터가 없다면 복귀

    while(teamA_buffer->head == teamA_buffer->tail)
    {
        printf("B: A팀의 버퍼에 데이터가 없습니다.\n");
    }
    // A팀의 버퍼에서 B팀의 버퍼로 아이템 가져오기
    temp = teamA_buffer->item[teamA_buffer->tail];
    printf("%s thread id: %lx\t get A->B item %d\n",(char*)name, pthread_self(), temp);
    teamA_buffer->item[teamA_buffer->tail] = 0;
    teamA_buffer->tail = ( teamA_buffer->tail +1) % MAX_CIRCULAR_SIZE;  //A팀 tail 증가

    //producer
    teamB_buffer->item[teamB_buffer->head] = temp;
    teamB_buffer->head = (teamB_buffer->head +1) % MAX_CIRCULAR_SIZE; //A팀 head 증가

    // 진행 확인을 위한 출력
    printf("result: %s thread id: %lx\t // get A->B item %d, A_head: %d, A_tail: %d, B_head: %d, B_tail: %d \n\n",(char*)name, pthread_self(), temp, teamA_buffer->head, teamA_buffer->tail, teamB_buffer->head, teamB_buffer->tail);
    count++;
}