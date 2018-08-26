#include <pthread.h>
#include <stdio.h>

const int MAX_VALUE = 256;
int fibArr[MAX_VALUE] = {0};
void fib(int count);// fibonacci
void *assembleFib(void *param);

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
    pthread_create(&tid,&attr,assembleFib,argv[1]);//给printRunner 传入参数
    pthread_join(tid,NULL);

    /** now output the prime numbers */
    for (i = 0; i < atoi(argv[1]); i++)
        printf("%d\n", fibArr[i]);

}

void fib(int count)
{
    fibArr[0] = 0;
    fibArr[1] = 1;

    for (int i = 2; i < count; i++)
        fibArr[i] = fibArr[i-1] + fibArr[i-2];
}

//求N 以内 所有的素数
void *assembleFib(void *param)
{
    int value = atoi(param);
    fib(value);

    return 0;
}
