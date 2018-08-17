#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

void main(int argc,char *argv[])
{
    pid_t pid;
    /* fork a child process */
    pid = fork();
    printf(" pid is: %d ", pid);

    if( pid < 0 ) {/* Error occurred */
        fprintf(stderr,"Fork failed.");
        exit(-1);
    }
    else if ( pid == 0) {/* child process */
        execlp("/bin/ls", "ls", NULL);
    }
    else {/* parent process */
          /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete.");
        exit(0);
    }
}
