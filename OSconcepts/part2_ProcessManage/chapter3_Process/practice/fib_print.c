#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int fib(int num);
int main(void)
{
    int num;
    scanf("%d", &num);

    if (num < 0){
        printf("num can't be negative.");
        exit(-1);
    }

    pid_t pid;
    /* fork a child process */
    pid = fork();
    printf(" pid is: %d \n", pid);

    if( pid < 0 ) {/* Error occurred */
        fprintf(stderr,"Fork failed.");
        exit(-1);
    }
    else if (pid == 0) {/* child process */
        fib(num);
    }
    else {/* parent process */
          /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete. \n");
        exit(0);
    }

    return 0;
}

int fib(int num)
{
    if (num == 0){
        printf("0 \n");
        return 0;
    }

    if (num == 1){
        printf("1 \n");
        return 1;
    }

    int result = fib(num - 1) + fib(num - 2);
    printf("%d \n", result);
    return result;
}