#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MAX_CIRCULAR_SIZE 200             // 링버퍼가 갖는 아이템 개수
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
char* A_name[TEAM_THREAD_SIZE] = {"A_player01", "A_player02", "A_player03", "A_player04", "A_player05"}; //reader
char* B_name[TEAM_THREAD_SIZE] = {"B_player01", "B_player02", "B_player03", "B_player04", "B_player05"}; //writer

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;


void circular_init();
void* teamA_get_item();
void* teamB_get_item();

int main()
{

    int random = 0; // 정수형 변수 선언
    char winner;
    int i;
    
    circular_init();
    printf("// 버퍼게임 시작\n");


    /*thread create*/
    for(i = 0; i <1000; i++)
    {
        srand(GetTickCound());
        random = rand()%2; // 난수 생성
        printf("%d\n",random);
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

        if(teamA_buffer->tail == teamA_buffer->head && teamB_buffer->tail == teamB_buffer->head)
        {
            printf("i ==2000, C\n");
            i=2000; // 게임 종료
            winner = 'C';
        }
        else if(teamA_buffer->tail == teamA_buffer->head)
        {
            printf("i ==2000, A\n");
            i=2000; // 게임 종료
            winner = 'A';
        }
        else if(teamB_buffer->tail == teamB_buffer->head)
        {   
            printf("i ==2000, B\n");
            i=2000; // 게임 종료
            winner = 'B';
        }
    }

    switch(winner)
    {
        case 'c': {printf("동점입니다.\n"); break;}
        defualt: printf("승자는 %c입니다.\n", winner);

    }

    // thread 종료
    for(int i = 0; i<5; i++)
    {
        pthread_join(A_thread[i],NULL);
        pthread_join(B_thread[i],NULL);
    }
    return 0;
}

void circular_init()
{
    // teamA_buffer->head= 0;
    // teamA_buffer->tail = 0; // 태그 값을 0으로 초기화
    // teamB_buffer->head=0;
    // teamB_buffer->tail = 0; // 태그 값을 0으로 초기화
    
    for(int i =0; i < 100; i++)
    {
        // buffer에 데이터 저장
        teamA_buffer->item[teamA_buffer->head] = i+1;

        // head tail 조정
        teamA_buffer->head = ( teamA_buffer->head +1) % MAX_CIRCULAR_SIZE; // head 증가
        if ( teamA_buffer->head == teamA_buffer->tail ){ // 버퍼가 모두 찼다면
            printf("버퍼가 가득 찼습니다.");
        }

    }

    for(int i =0; i < 100; i++)
    {
        teamB_buffer->item[teamB_buffer->head] = i+101;

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
    if ( teamB_buffer->head == teamB_buffer->tail){
        printf("A: B팀의 버퍼에 데이터가 없습니다.");
        return 0; // 테이터 없음
    }

    temp = teamA_buffer->item[teamA_buffer->head] = teamB_buffer->item[teamB_buffer->tail];
    teamA_buffer->head = (teamA_buffer->head +1) % MAX_CIRCULAR_SIZE; //A팀 head 증가
    teamB_buffer->item[teamB_buffer->tail] = 0;
    teamB_buffer->tail = ( teamB_buffer->tail +1) % MAX_CIRCULAR_SIZE;  //V팀 tail 증가

    // 진행 확인을 위한 출력
    printf("%s thread id: %lx\t // get B->A item %d, A_head: %d, A_tail: %d, B_head: %d, B_tail: %d \n",(char*)name, pthread_self(), temp, teamA_buffer->head, teamA_buffer->tail, teamB_buffer->head, teamB_buffer->tail);
    count++;
}

void* teamB_get_item(void* name)
{
    int temp; // 데이터를 옮기기 위한 임시 저장소

    // 데이터 가져오기
    // 큐에 데이터가 없다면 복귀
    if ( teamA_buffer->head == teamA_buffer->tail){
        printf("B: A팀의 버퍼에 데이터가 없습니다.");
        return 0; // 테이터 없음
    }

    // A팀의 버퍼에서 B팀의 버퍼로 아이템 가져오기
    temp = teamB_buffer->item[teamB_buffer->head] = teamA_buffer->item[teamA_buffer->tail];
    teamB_buffer->head = (teamB_buffer->head +1) % MAX_CIRCULAR_SIZE; //B팀 head 증가
    teamA_buffer->item[teamA_buffer->tail] = 0;
    teamA_buffer->tail = ( teamA_buffer->tail +1) % MAX_CIRCULAR_SIZE;  //A팀 tail 증가

    // 진행 확인을 위한 출력
    printf("%s thread id: %lx\t // get A->B item %d, A_head: %d, A_tail: %d, B_head: %d, B_tail: %d \n",(char*)name, pthread_self(), temp, teamA_buffer->head, teamA_buffer->tail, teamB_buffer->head, teamB_buffer->tail);
    count++;
}