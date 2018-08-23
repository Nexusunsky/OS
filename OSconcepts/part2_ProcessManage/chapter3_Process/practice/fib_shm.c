/* Example shared memory program. */
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PERMS (S_IRUSR | S_IWUSR)
#define MAX_SEQUENCE 10
typedef struct {
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

int main(int argc, char *argv[])
{
    int i, seq_size;
    /* the process identifier */
    pid_t pid;
    /* the id of the shared memory segment */
    int segment_id;
    /* a pointer to the shared memory segment */
    shared_data* shared_memory;

    /* do some error checking to ensure the parameter was passed */
    if (argc != 2) {
        fprintf(stderr,"Usage: ./shm-fib <sequence size>\n");
        return -1;
    }
    seq_size = atoi(argv[1]);
    if (seq_size > MAX_SEQUENCE) {
        fprintf(stderr,"sequence size must be < %d\n",MAX_SEQUENCE);
        return -1;
    }
    /* allocate a shared memory segment */
    if ((segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), PERMS)) == -1) {
            fprintf(stderr,"Unable to create shared memory segment\n");
        return 1;
    }
    printf("Created shared memory segment %d\n",segment_id);
    /* now attach the shared memory segment at the specified address */
    if ((shared_memory = (shared_data *)
        shmat(segment_id, 0, 0)) == (shared_data *)-1) {
        fprintf(stderr,"Unable to attach to segment %d\n",segment_id);
        return 0;
    }
    /* set the size of the sequence */
    shared_memory->sequence_size = seq_size;
    /* now fork a child process */
    if ((pid = fork()) == (pid_t)-1) {
        return 1;
    }
    /**
      * now create a child process and have the child process set
      * the the shared memory segment to a certain value.
      * The parent process will inquire on this shared value when
      * it returns from wait(). Thus, the call to wait() provides the synchronization.
    */
    if (pid == 0) { /** child code */
        printf("CHILD: shared memory attached at address %p\n", shared_memory);
        /* now have the child generate the Fibonacci sequence .... */
        shared_memory->fib_sequence[0] = 0;
        shared_memory->fib_sequence[1] = 1;
        for (i = 2; i < shared_memory->sequence_size; i++)
            shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i-1] + shared_memory->fib_sequence[i-2];
        /* now detach the shared memory segment */
            shmdt((void *)shared_memory);
    }
    else { /* parent code */
        wait(NULL);
        for (i = 0; i < shared_memory->sequence_size; i++)
            printf("PARENT: %d:%ld\n",i, shared_memory->fib_sequence[i]);
        /* now detach and remove the shared memory segment */
        shmdt((void *)shared_memory);
        shmctl(segment_id, IPC_RMID, NULL);
    }
    return 0;
}