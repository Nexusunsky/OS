1，论述长期，中期，短期调度之间的区别。
    答：
    长期调度程序（或称作业调度程序）和短期调度程序（或称CPU调度程序）之间的 主要差别 是它们执行的频率。
    a，短期调度 必须频繁地为CPU选择新进程。进程可能执行 数毫秒(ms) 就会进行 I/O请求，短期调度程序 通常每100ms 至少执行一次。
    b，长期调度 执行得并不频繁，在系统内新进程的创建之间可能有 数分钟间隔。长期调度程序控制 多道程序设计的程度（内存中的进程数量）
    c，中期调度 的核心思想 是能将进程从内存中（或从CPU竞争）中移除，从而降低多道程序设计的程度。

2，描述内核在两个进程间进行上下文切换的过程。
    答：
    中断 使得CPU从当前任务改变为运行内核子程序。
    发生 中断，系统 保存当前运行在 CPU中进程的上下文 （PCB表示，包括CPU寄存器的值，进程状态和内存管理信息等）
    通过执行一个 状态保存（state save） 来保存CPU当前状态，
    之后执行一个 状态恢复（state restore）重新开始运行。
    内核将 旧进程的状态 保存在其 PCB中，然后装入 经调度要执行的并已保存的 新进程的上下文。


3，考虑 RPC机制，描述 因为没有强制 或者"最多一次" 或者"刚好一次" 的语义带来的不必要的后果。
   讨论没有提供任何保证的机制可能使用。
    答：
    本地过程调用只有在极端情况下才可能失败，
    但是由于普通网络错误，RPC可能会失败或重复多次。
    处理该问题的一种方法是 操作系统 确保一个消息 刚好执行一次，而不是 最多执行一次。
    大多数本地过程调用具有"刚好一次"的属性，但是很难实现。

4，说明程序LINE A可能输出什么？
    答：PARENT:value = 5

5，下面设计的优点和缺点分别是什么？系统层次和用户层次都要考虑。
    a，同步和异步通信。
       答：
    b，自动和显示缓冲。
       答：
    c，复制传送和引用传送。
       答：
    d，固定大小和可变大小消息。
       答：

6，Fibonacci 序列是一组数：0，1，1，2，3，5，8，···，通常它可以表示为：
    fib0 = 0；
    fib1 = 1；
    fibn = fibn-1 - fibn-2；
   答：使用系统调用fork()编写一个C程序，它在其子程序中生成 Fibonacci 序列，
   序列的号码将在命令行提供。

7，同6，由于父进程和子进程都有自己的数据副本，子进程必须输出 Fibonacci序列，
设计此程序的另一个方法是在父进程和子进程之间建立一个共享内存段，此方法允许子程序将Fibonacci
序列的内容写入共享内存片段，当子进程完成时，父进程输出此序列。由于内存是共享的，每个子进程的
变化都会影响到共享内存，也会影响到父进程。
```
Answer: /* Example shared memory program. */
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
```
