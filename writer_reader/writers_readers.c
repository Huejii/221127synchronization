#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#define READER_SIZE 5

/*스레드 ID 선언*/
pthread_t reader[5], writer_upper, writer_lower;

/*semaphore variable 선언*/
// sem_t result;

/*reader와 writer가 접근할 문자열 선언*/
char* S = "Happy Merry Christmas~!";

/*데이터에 접근한 순서를 파악하기 위한 count variable 선언*/
int count = 1;

/*함수 선언*/
void upper(char* s);
void lower(char* s);
void *reader_task();
void *writer_upper_task();
void *writer_upper_task();

int main()
{
    /*writer thread 생성*/
    for(int i =0; i < READER_SIZE; i++)
    {
        pthread_create(&reader[i],NULL,reader_task,NULL);
    }

    /*writer thread 생성*/
    pthread_create(&writer_upper,NULL,writer_upper_task,NULL);  // 데이터를 대문자로 바꾸는 writer => HAPPY MERRY CHRISTMAS~!
    pthread_create(&writer_lower,NULL,writer_lower_task,NULL);   // 데이터를 소문자로 바꾸는 writer => happy merry christmas~!




    return 0;
}

void *reader_task()
{
    while(1){
       char    ch;
       int     result;

       result = read (fd[0],&ch,1);
       if (result != 1) {
            perror("read");
            exit(3);
    }    printf ("Reader: %c\n", ch);  }
}

void *writer_upper_task()
{
    upper(S);
}

void *writer_lower_task()
{
    lower(S);
}

void *writer_ABC()
{
     int     result;
     char    ch='A';

     while(1){
           result = write (fd[1], &ch,1);
           if (result != 1){
               perror ("write");
               exit (2);
           }

           printf ("Writer_ABC: %c\n", ch);
           if(ch == 'Z')
              ch = 'A'-1;

           ch++;
      }
}

void *writer_abc()
{
  int     result;  char    ch='a';

  while(1){
      result = write (fd[1], &ch,1);
      if (result != 1){
            perror ("write");
            exit (2);
      }

      printf ("Writer_abc: %c\n", ch);
      if(ch == 'z')
            ch = 'a'-1;

     ch++;
  }
}

int main()
{
   pthread_t       tid1,tid2,tid3;
   int             result;

   result = pipe (fd);
   if (result < 0){
       perror("pipe ");
       exit(1);
   }

    pthread_create(&tid1,NULL,reader,NULL);
    pthread_create(&tid2,NULL,writer_ABC,NULL);
    pthread_create(&tid3,NULL,writer_abc,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
}

void upper(char* s) {
    int i;
    for (i = 0; s[i] != NULL; i++) {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 'a' - 'A';
    }
}

// 소문자로 바꿔주는 함수
void lower(char* s) {
    int i;

    for (i = 0; s[i] != NULL; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 'a' - 'A';
    }
}