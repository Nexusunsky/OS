# 进程同步

    1，背景：
        协作进程：系统内执行的 其他进程 相互影响 的进程，
        互相协作的进程
            可以
                直接共享 逻辑地址空间（代码和数据），通过 轻量级进程或线程 实现
            或者
                只通过 文件或消息 来共享数据。
        共享数据的 并发访问 后产生数据的 不一致。
        本章将讨论 各种机制，确保 共享 同一逻辑地址空间的协作进程 可 有序地执行。

        竞争条件：多个进程 并发 访问和操作 同一数据 且执行结果与 访问发生的特定顺序有关。
        避免 竞争条件 就需要 一端时间内 只有一个进程 能操作变量。

    2，临界区问题：
        每个进程有个代码段：临界区，在该区中 进程可能改变 共同变量（表，文件等）
        特征当一个进程进入临界区时，没有其他进程可被允许在 临界区执行

        设计一个 以便 进程协作的协议：
            每个进程必须请求进入其 临界区。实现这一请求的代码段称为 进入区（entry section）
            临界区（critical section）之后 可有退出区（exit section），其他代码为剩余区（remainder section）

        必须满足三项要求：
            a，互斥（mutual exclusion）：
                如果进程Pi在临界区执行，那么 其他进程 都不能在其 临界区执行。
            b，前进（progress）:
                如果没有进程在临界区内执行，且有进程需要进入临界区，那么只有那些不在剩余区内执行的进程可参加选择已确定能够下一个进入临界区，而且这种选择不可被无限推迟。
            c，有限等待（bounded waiting）：
                从一个进程 做出进入临界区的请求，直到该请求允许为止，其他进程 允许进入其 临界区的次数 有上限。


        问题：
            操作系统中，存在有产生竞争条件的情况：
                某个时刻，可同时存有多个处于内核模式的活动进程，实现操作系统的代码会出现竞争条件，
                有两种方法用于处理操作系统内的临界区问题：
                    抢占内核（preemptive）:
                        允许处于内核模式的进程被抢占，抢占内核需要认真设计确保数据结构不会导致竞争条件，SMP体系结构，抢占内核更难设计，
                        因为处于两个内核模式的进程同时运行在不同的处理器上。
                    非抢占内核(nonpreemptive kernel) :
                        不允许处于内核模式的进程被抢占，而非抢占内核模式的数据结构，从根本上而言，不会导致竞争条件。
                        处于内核模式运行的 进程会一直运行，直到它退出内核模式，阻塞或自动退出CPU的控制。


    3，Peterson算法：
        适用：
            两个进程在 临界区 和 剩余区 交替执行。
        共享：
            Peterson需要在 两个进程之间 共享两个数据项，
                int turn;
                    表示哪个进程 可以 进入其临界区。

                boolean flag[2];
                    表示哪个进程 想要 进入其临界区。
```
        //进程 Pi
        do {
            flag[i] = TRUE;
            turn = j;
            while (flag[j] && turn == j)
                ;

            // 临界区

            flag[i] = FALSE

            //剩余区
        } while(TRUE);

        //进程 Pj
        do {
            flag[j] = TRUE;
            turn = i;
            while (flag[i] && turn == i)
                ;

            // 临界区

            flag[j] = FALSE

            //剩余区
        } while(TRUE);
```
        为了进入临界区，进程Pi 首先设置flag[i] = true ，且turn=j
            表示：表示Pi想要进入临界区，Pj进程 可以 进入临界区，

        现在需要 算法 能解决 临界区问题：
            1，互斥：
                当flag[i] = flag[j] = TRUE
                turn只能在同一时刻 i, j之间的某一个值。
                并且当 turn的 值 确定时，只能有一个进程 进入其临界区。另一个会 在while中等待。

            2，前进：
                假设：flag[j]==true 和 turn==j 成立。然后Pi陷入while，那么将不能进入临界区。
                Pj处于剩余区 不准备进入临界区，则 flag[j] == false ，Pi进入从while 中出来进入临界区。
                如果Pj设置了flag[j] = true，且进入了 while中。
                此时 turn == i 或 turn == j，一个在临界区，一个在while中
                    当turn == i时 则 Pi进入临界区，
                    当turn == j时 则 Pj进入临界区
                当其中之一退出时，假设为Pj退出临界区，此时，flag[j] = false，那么进入了while的进程Pi将 进入到临界期。
                而 Pj 便在 剩余区中，
                
            3，有限等待：
                由上述的 Pi和Pj进程 的所执行的 代码区段 的交替，那么当 其中之一进程 跨越临界区一次 之后就能进入自己的临界区。
        
        
    4，硬件同步
        一般而言，任何临界区都需要 一个简单的工具 ---- 锁。
        原理：通过要求临界区 用锁 来保护，就可以避免 竞争条件，即 一个进程在进入 临界区之前必须得到锁，而在退出临界区时 释放锁。
        
        单处理器环境：
            非抢占内核使用：修改共享变量时，禁止中断出现，确保当前指令序列的执行不会被中断，确保共享变量不会被意外修改。
        
        多处理器环境：
            许多现代计算机系统提供了 特殊硬件指令 以允许 原子地 检查和修改字的内容或交换两个字的内容。    

'''
    do{
        waiting[i] = TRUE;
        key = TRUE;
        while (waiting[i] && key)
            key = TestAndSet(&lock);
        waiting[i] = FALSE;
        
        // critical section
        
        //循环扫描 waiting[i] (i+1, i+2, i+3....,n-1,0,....,i-1) 
        //根据 这一顺序而指派第一个等待进程 作为下一个进入临界区的进程。
        j = (i + 1) % n;
        while((j!=i) && !waiting[j])
            j = ( j + 1 ) % n;
        
        if(j == i)
            lock = FALSE;
        else 
            waiting[j] = FALSE
        
        //reminder section            
    } while(TRUE);
'''       
         使用指令 TestAndSet算法，公用数据结构如下：
                 boolean waiting[n];
                 boolean lock;    
         只有当waiting[i] == false 或 key == false时，进程Pi才进入临界区。    
         只有当TestAndSet执行时，key的值才能变成false 所有其他进程必须等待。
         只有其他进程离开临界区时，变量waiting[i]的值才能变成false，每次只有一个waiting[i]被设置为false。以满足互斥。   
         由于进程在退出临界区时 或将 lock 设为false 或将 waiting[j]设为false。这两种情况 都允许 等待进程 进入 临界区以执行。
         任何等待进入临界区的进程只需要等待 n-1 次。   

    5，信号量：
        基于硬件的临界问题解决方案（采用TestAndSet() 与 Swap()指令）但是对于 应用程序而言，使用比较复杂，
        为了 解决这个困难，可以使用 信号量（semaphore）的同步工具。
        信号量S 是个整数变量，除了初始化之外，
            它只能通过两个标准原子操作：
                wait()
                signal()

```
    //减少信号量的计数
    wait(S) {
        while(S<=0)
            ;// no-op
        S--;
    }
    
    //增加信号量的计数
    signal(S) {
        S++;
    }
```
    在 wait() 和 signal() 操作中，对 信号量整形值 的修改必须 不可分地执行，
    即当一个进程 修改信号量值时，不能有 其他进程 同时 修改同一信号量的值。
    另外，对于wait(S) 对 S的整形值 的测试(S<=0) 和对其可能修改（S--）也必须不被中断执行

        a，用法：
            通常操作系统 区分
                计数信号量：值域不受限制；
                二进制信号量：只能为 0 或 1；还称为互斥锁
'''
//使用 二进制信号量 的互斥实现
    do {
        waiting(mutex);

        //critical section

        signal(mutex);

        //remainder section
    } while(TRUE);
'''

        b，实现
        这里定义的信号量的主要缺点 是 都要
            忙等待（busy waiting）：
                while（TRUE）的连续循环 浪费了CPU时钟。也称为 自旋锁（Spinlock）。
            优点：进程在等待锁时不需要进行 上下文的切换（比较耗时）。因此在锁的占用时间比较短时，可以使用 自旋锁。
            场景：常用于 多处理 系统中，一个线程 在一个处理器自旋时，另一个线程 可以在另一处理器上在其临界区内执行。
            替代方案：将 忙等待 改为 阻塞，即 阻塞操作 将一个 进程 放入到 信号量相关的等待队列中，
                    并将该 进程状态 切换称 等待状态，然后 控制 转到CPU调度程序，以选择另一个进程来执行。

            数据结构：
                typedef struct {
                    int value;
                    struct process *list;
                } semaphore;
            每个信号量都有 一个整型值 和 一个进程链表。
            说明：当一个进程 必须 等待信号量时，就加入到 进程链表上。signal()会从 等待进程链表 中取一个进程 以唤醒。

        信号量新的 定义 如下：
'''
wait(semaphore *S) {
    S->value--;
    if (S->value < 0) {
        add this process P to S->list;
        block();
    }
}

signal(semaphore *S) {
    S->value++;
    if (S->value <= 0) {
        remove a process P from S ->list;
        wakeup(P);
    }
}
'''
    等待进程 的链表可以利用 进程控制块PCB 中 的一个链接域 加以实现。
    每个信号量 包括 一个 整型值 和 一个 PCB链表的指针；
    信号量 中链表可使用 任何排队策略（FIFO）信号量的正确使用并 不依赖 于链表的特定排队机制（策略）。
    信号量的关键在于 原子地执行。必须确保 没有两个进程能同时对同一个信号量执行操作wait()和 signal()。
    必须承认 目前对wait()和signal()操作的定义 也没有完全取消忙等，而是取消了 应用程序 进入临界区的 忙等。


    6，死锁与饥饿
        死锁（deadlocked）:两个或多个进程 无限等待 一个事件，而该 事件 只能由这些 等待进程 之一来产生。
        该被等待的事件 指 signal()操作的发生。

        无限期阻塞（indefinite blocking） 或 饥饿(starvation)，即进程在 信号量内 无限期 等待，
        如果对与信号量 相关的链表按LIFO顺序来增加和移动进程，可能发生 无限期阻塞。

    7，经典同步问题
        采用信号量 作为同步问题的解答

        a，有限缓冲问题：
            通常用来说明 同步原语 的能力。
            该方案的 通用解决结构：
                假定缓冲池有 n个缓冲项，每个缓冲项能 存一个数据项。信号量mutex 提供了 对缓冲池访问的互斥要求，并初始化为1。
                信号量 empty 和 full 分别用来表示 空缓冲项 和 满缓冲项 个数，empty初试化为n，full初始化为0；
'''
do {                                |do {
    ...                             |    wait(full);
    //produce an item in nextp      |    wait(numtex);
    ...                             |    ...
    wait(empty);                    |    //remove an item from buffer to nextc
                                    |
    wait(mutex);                    |    ...
    ...                             |    signal(mutex);
    //add nextp to buffer           |    signal(empty);
    ...                             |    ...
    signal(mutex);                  |    //consume the item in nextc
                                    |
    signal(full);                   |    ...
} while(TRUE);                      | } while(TRUE);
'''

        b，读者--写者问题
            读者--写者问题：
                1，两个读者同时访问共享数据，不会产生不利的结果。
                2，如果一个写者和其他线程（即可能是读者也可能是写者）同时访问共享对象，很可能混乱。
                要求写者对共享数据库有排他访问。这一同步问题称为读者--写者问题；

            两个变种：都与优先级有关。
            第一 读者-写者问题：要求没有读者需要保持等待除非已有一个写者已获得允许访问共享数据。
            第二 读者-写者问题：一旦写者就绪，那么读写者会尽可能地执行写操作。

            对于第一 读者-写者问题：读者进程共享一下数据：
                semaphore mutex,wrt;
                int readcount;
            1，信号量 mutex 和 wrt 初始化为1，readcount初始化为0；
            2，信号量wrt为 读者和写者进程 所共用。
            3，mutex确保在 更新变量readcount时 的互斥。变量readcount 用来 跟踪有多少进程 正在读对象。
            4，信号量wrt 供写者作为 互斥信号量。它为 第一个进入 临界区 和 最后一个 离开 临界区的读者所使用。
'''
//写者进程结构                  | //读者进程结构
 do {                         | do {
    wait(wrt);                |    wait(mutex);// 对于操作（读或写）的控制锁
    ...                       |    readcount++;
    //writing is performed    |    if(readcount == 1) // 控制实际的写者的个数
    ...                       |         wait(wrt);
    signal(wrt);              |    signal(mutex);
 } while(TRUE);               |    ...
                              |    //reading is performed
                              |    ...
                              |    wait(mutex);
                              |    readcount--;
                              |     if(readconunt == 0)
                              |         signal(wrt);
                              |    signal(mutex);
                              | } while(TRUE);
'''
如果有 一个进程 在 临界区 内，且 n个进程 处于等待，那么一个读者在wrt上等待，而 n-1个在mutex上等待。
而且，当一个写者 执行signal(wrt)时，可以重新启动等待读者或写者的执行。
在获取 读写锁 时，指定锁的模式：读访问 或 写模式。
            使用场景：
                1，当可以区分那些进程只需要读取共享数据而哪些进程只需要写共享数据。
                2，当 读者进程 比 写者进程 多时。读写锁的 建立开销 通常比信号量或互斥锁大，而这一开销可以通过允许多个读者增加并发度来进行弥补。


      c，哲学家进餐问题、
           哲学家同步问题：
               典型的同步问题，是一个并发控制问题，需要在 多个 进程之间 分配 多个资源 且不会出现 死锁和饥饿 的典型例子。

           简单解决方案：
                每只筷子 都用一个 信号量 来表示，一个哲学家通过执行wait()操作试图获取相应的筷子它会通过执行signal释放相应的筷子

'''
//哲学家i进程结构
 do {
    wait(chopstick[i]);
    wait(chopstick[(i+1)]%5);
    ...
    //eat
    ...
    signal(chopstick[i]);
    signal(chopstick[(i+1)]%5);
    ...
    //think
    ...
 } while(TRUE);
'''
            总结：本方案确实确保了 没有 两个科学家 同时使用 同一只筷子，但是 却可能导致死锁。





