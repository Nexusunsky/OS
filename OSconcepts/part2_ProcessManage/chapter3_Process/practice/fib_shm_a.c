#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define PERMS (S_IRUSR | S_IWUSR)
#define MAX_SEQUENCE 10
typedef struct {
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

int main(void)
{
    int num, i;
    scanf("%d", &num);

    pid_t pid;

    if (num < 0 || num > MAX_SEQUENCE){
        printf("num can't be illegal.");
        return 1;
    }

    /* The identifier for the shared memory segment*/
    int segment_id;

    /* a pointer to the shared memory segment*/
    shared_data* shared_memory;

    /* allocate a shared memory segment */
    if((segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), PERMS)) == -1) {
        fprintf(stderr, "Unable to create shared memory segment\n");
        return 1;
    }

    printf("Created shared memory segment %d\n", segment_id);

    /* attach the shared memory segment */
    if ((shared_memory = (shared_data *)shmat(segment_id, 0, 0)) == (shared_data *)-1) {
        fprintf(stderr, "Unable to attach to segment %d\n",segment_id);
        return 0;
    }

    /* set the size of the sequence */
    shared_memory->sequence_size = num;

    /* now fork a child process */
    if ((pid = fork()) == (pid_t)-1) {
        return 1;
    }

    if (pid == 0) {/* child process */
        printf("CHILD: shared memory attached at address %p\n", shared_memory);

        /* now have the child generate the Fibonacci sequence .... */
        shared_memory->fib_sequence[0] = 0;
        shared_memory->fib_sequence[1] = 1;
        for (i = 2; i < shared_memory->sequence_size; i++)
            shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i-1] + shared_memory->fib_sequence[i-2];
            /* now detach the shared memory segment */
            shmdt((void *)shared_memory);

    }
    else {/* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        for (i = 0; i < shared_memory->sequence_size; i++)
            printf("PARENT: %d:%ld\n",i, shared_memory->fib_sequence[i]);

        /* now detach and remove the shared memory segment */
        shmdt((void *)shared_memory);
        shmctl(segment_id, IPC_RMID, NULL);
        exit(0);
    }

    return 0;
}

