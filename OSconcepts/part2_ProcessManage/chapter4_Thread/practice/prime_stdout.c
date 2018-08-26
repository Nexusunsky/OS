#include <pthread.h>
#include <stdio.h>

const int MAX_VALUE = 256;
int prime[MAX_VALUE] = {0};
void *printRunner(void *param);//the thread

int main(int argc,char *argv[])
{
    int i;
    pthread_t tid;
    pthread_attr_t attr;

    if(argc!= 2) {
      fprintf(stderr,"usage:a.out <integer value> \n");
      return -1;
    }

    if(atoi(argv[1]) < 2 ) {
      fprintf(stderr,"Argument %d must be <=0 \n", atoi(argv[1]));
      return -1;
    }

    pthread_attr_init(&attr);
    pthread_create(&tid,&attr,printRunner,argv[1]);//给printRunner 传入参数
    pthread_join(tid,NULL);

    /** now output the prime numbers */
    for (i = 1; i <= atoi(argv[1]); i++)
        if (prime[i] > 0)
            printf("%d\n", prime[i]);

}


//求N 以内 所有的素数
void *printRunner(void *param)
{
    int value = atoi(param);
    int flag[MAX_VALUE] = {0}, i, j, count;
    for(i = 2, count = 2; i < value; i++){
        if(!flag[i])
            prime[count++] = i;

        for(j = 2*i; j < value; j += i){
            flag[j] = 1;
        }
    }

    return 0;
}
