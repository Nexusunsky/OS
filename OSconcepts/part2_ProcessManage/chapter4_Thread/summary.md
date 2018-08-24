# 概述：
    1，线程是 CPU使用基本单元，它由 线程ID，程序计数器，寄存器集合和栈组成。
    它与属于同一个进程的其他线程共享代码段，数据段和其他操作系统资源，如打开文件和信号。

    2，适应场景：
        a，应用程序 通常是作为一个具有多个 控制线程的独立进程 实现。
        b，在远程过程调用（RPC）通过提供一种类似于普通函数或子程序调用的通信机制，以允许进程通信。
        c，操作系统也是多线程的，少数线程在内核中运行，每个线程完成一个指定的任务，如管理设备和中断处理。

    3，优点：
        a，相应度高
        b，资源共享
        c，经济
        d，多处理器体系结构的利用

    4，多线程模型
        提供线程支持的两种方法：
            a，用户层的用户线程
            b，内核层的内核线程

        用户线程和内核线程之间的多种关系：
            a，多对一模型
               将许多 用户级线程 映射到一个 内核线程。
               优点：
                    线程管理 是由 线程库 在用户空间进行的，因而效率比较高。
               缺点：
                    首先，一个线程执行了阻塞系统调用，那么整个进程会阻塞。
                    其次，任意时刻只有一个线程能访问内核，多个线程不能并行运行在多处理器上。
            案例：Green thread（Solaris所用线程库），GNU可移植线程（GNU Portable Threads）

            b，一对一模型
               一对一模型将 每个 用户线程 映射到一个 内核线程。
               优点：
                    在一个线程执行阻塞系统调用时，能允许另一个线程继续执行，所以它提供了比多对一模型更好的并发
                    也允许多个线程能并行地允许在多处理器系统上。
               缺点：
                    由于每创建一个 用户线程 就需要创建一个 内核线程，而创建 内核线程 的开销会影响 应用程序的性能，
                    所以这种模型的实现系统基本都限制了系统所支持的线程数量。
            案例：Linux 系统于Windows操作系统族

            c，多对多模型
                多对多模型多路复用了许多用户线程到同样数量或更小数量的内核线程上。
                内核线程的数量可能与特定应用程序或特定机器有关。
                优点：
                    开发人员可创建 任意多的 用户线程，并且相应 内核线程 能在 多处理器系统上 并发 执行。
                    当一个线程执行阻塞 调用系统调用时，内核能调度另一个线程来执行。
            延伸：二级模型：
                在保留多路复用许多用户线程到同样数量或更小数量的内核线程上，但也允许将一个用户线程绑定到某个内核线程上。
            案例：
                IRIX，HP-UX，Tru64 UNIX等操作系统所支持。
                Solaris在9之前版本中支持二级模型，但从9开始使用一对一模型。


    5，线程库
       为程序员提供创建和管理线程的API，主要有两种方法来实现线程库
         第一种方法
            是在 用户空间 提供一个 没有内核支持 的库，此库的所有 代码和数据结构 都存储在 用户空间中。
            调用库中的一个函数只是导致了 用户空间的一个 本地函数调用，而不是 系统调用。

         第二种方法
            是执行一个有操作系统直接支持的 内核级的库。此时，库的代码和数据结构存在于 内核空间中。
            调用库中的一个API函数通常会导致内核的系统调用。

       目前使用的三种线程库：
       1，POSIX Pthread
            Pthread作为POSIX标准的扩展，可以提供用户级或内核级的库。
       2，Win32
            Win32适用Windows操作系统的内核级线程库，
       3，Java
            JVM的实例是允许在宿主操作系统之上，Java线程API通常采用宿主系统上的线程库来实现。

       Pthread
       是由 POSIX 标准为线程创建和同步定义的API。是线程行为的规范，却不是实现。

       Win32程序
       采用Win32程序线程库创建的技术类似于Pthread技术。

       Java线程
       a，JVM一般在操作系统之上实现，这种实现方式允许JVM隐藏基本的操作系统实现细节，
          并提供一种一致的，抽象的环境以允许Java程序能在任何支持JVM的平台上允许。

       b，JVM 的规范 并没有指明Java线程如何被映射到底层的操作系统，而是让特定的JVM来实现。

       c，Win系列操作系统的JVM实现可以创建Java线程时使用Win32 API，Linux和Solaris 系统则可以采用Pthread API

    6，多线程问题
        a，多线程程序中，系统调用fork()和exec(）的 语义不同于 用于创建独立的，复制的进程。

        b，不同的UNIX系统有两种形式的fork，一种是复制 所以线程，另一种是 复制 调用了fork的线程。

        c，系统调用exec的工作方式，于第三章所述的方式通常相同
           即如果一个线程调用了系统调用exec，那么exec参数所指定的程序会替换整个进程，包括所有线程。

        fork的两种形式的使用于应用程序有关
            a，如果调用fork之后立即调用exec，那么 没有必要 复制所有线程，因为exec参数所指定的 程序 会替换 整个进程。
            b，但是调用fork之后没有调用exec，那么另一进程就应复制所有线程


    7，取消
        线程取消 是在 线程完成之前 来终止 线程的任务。
        要取消的线程称为 目标线程，目标线程的取消可能在如下两种情况下取消：
        a，异步取消
            一个线程立即终止目标线程

        b，延迟取消
            目标线程不断地检查它是否一个终止，这允许目标线程有机会以有序方式来终止自己。

        实现难点：
            如果资源已经分配给要取消的线程，或要取消的线程正在更新于其他线程所共享的数据，那么取消就会有困难。
            异步取消的实现很麻烦，通常，异步取消线程并不会是所需的系统资源空闲。
            延迟取消，一个线程 指示 目标线程要被取消，那么一个线程将会检查 一个标志 以确定它是否应该取消。


    8，信号处理
        UNIX 中使用 信号 来通知 进程某个事件已发生了，根据 需要 通知 信号的来源 和 事件的理由
        信号 可以 同步 或 异步 接收。
            同步信号：
                信号是由 执行操作 而产生 信号 的同一进程发出。

            异步信号：
                当一个信号由运行进程之外的事件产生，那么进程就是异步接收这一信号。

        但是不论信号是 同步 或者 异步的，所有信号 都具有同样的模式。
            a，信号 是由 特定事件的发生所产生的。
            b，产生的信号要发送到进程。
            c，一旦发送，信号必须加以处理。

        每个信号 可能由 两种可能的 处理程序中的一种 来处理：
            1，默认信号处理程序
            2，用户定义的信号处理程序

        每个信号都有一个 默认信号处理程序，当处理信号时 是在内核中运行的。
        而用户定义的信号处理程序可以改写这种默认的动作。

        多线程程序信号的处理有如下几种情况：
            a，发送信号到信号所应用的线程。
            b，发送信号到进程内的每个线程。
            c，发送信号到进程内的某些固定线程。
            d，规定一个特定线程以接收进程的所有信号。

        因为信号只能处理一次，所以 信号 通常 发送到 不拒绝它的第一个线程。

        1，大多数多线程版UNIX允许线程描述它会接收什么信号和拒绝什么信号。
           标准的UNIX函数是kill(pid_t pid,int signal)，指定了信号的发送进程（pid）。

        2，不过POSIX Pthread 还提供了 pthread_kill(pthread_t tid,int signal)函数允许信号被发送到一个指定的线程（tid）

        3，Windows不明确提供对信号的支持，但是能通过异步过程调用来模拟（asynchronous procedure call，APC）
           APC工具允许用户线程指定一个函数以便在用户线程收到特定通知是能被调用。APC只发送给特定线程。


    9，线程池
        线程池 为解决 无限制的线程创建 会耗尽 系统资源，如CPU时间和内存。
        其主要思想：在进程开始时创建一定数量的线程，并放入池中等待工作。
        优点：
            a，通常用现有线程处理请求要比等待创建新的线程快
            b，线程池限制了任意时刻可用线程的数量，对那些不能支持大量并发线程的系统非常重要。

    10，线程特定数据
        同属于一个进程的 线程 共享进程数据。
        有些情况下，每个线程可能需要一定数据的自己的副本，这种数据称为线程特定数据(thread-specific data)

    11，调度程序激活
        内核于线程库之间的通信。
        借助多线程模型中 多对多模型 和 二级模型，这种协调允许 动态调整 内核的数量 以保证其 最好的性能。
        轻量级进程（LWP）：
            1，在用户和内核线程之间设置一种中间数据结构。
            2，对于 用户线程库，LWP 表现为一种 应用程序 可以调度 用户线程 来允许的 虚拟处理器。
            3，每个 LWP 与 内核线程 相连，该 内核线程 被 操作系统 调度到 物理处理器 上运行。
        如果内核线程阻塞（如在等待一个I/O操作结束），LWP也阻塞，而与LWP相连的用户线程也阻塞。

        调度器激活：解决 用户线程库 与 内核间通信 的方法
        工作方式：
            内核 提供一组 虚拟处理器（LWP）给 应用程序，应用程序 可调度 用户线程 到一个可用的 虚拟处理器 上。
            upcall：
                1，内核 必须告知与 应用程序 有关的特定事件。
                2，由具有 upcall处理句柄 的 线程库 处理，而该upcall处理句柄必须在虚拟处理器上运行。

            案例：
                1，当 应用线程 当要阻塞时，事件引发 一个upcall
                2，内核 向 应用程序 发出一个 upcall，通知它 线程阻塞 并标示 特殊的线程。
                3，然后 内核 分配一个 新的虚拟处理器 给应用程序，
                   应用程序在 这个新的虚拟处理器 上运行 upcall处理程序，该程序 保存阻塞线程状态 和 放弃 阻塞线程运行的 虚拟处理器。
                4，然后该upcall处理程序 调度 另一个适合在 新的虚拟处理器 上运行的 线程
                5，当 阻塞线程事件 等待发生时，内核 向线程库发出另一个upcall，来通知它 先前 阻塞的线程 现在可以运行了。


    12，操作系统实例
        a，Windows XP线程
            一个Windows XP 应用程序以独立的进程方式运行，每个进程 可包括 一个 或 多个线程
            XP提供了对fiber库的支持，该库提供了多对多模型的功能，通过使用线程库，同属于 一个进程 的 每个线程 都能访问 进程的地址空间。

            一个线程包括：
                1，一个线程ID，唯一标示线程
                2，一组寄存器集合，标示处理器状态
                3，一个用户栈，共线程在用户模式下运行，一个内核堆栈，供线程在内核模式下运行。
                4，一个私有存储区域，为各种运行时库和动态链接库（DLL）使用。

            上下文：
                寄存器集合，栈和私有存储区域通常称为线程的上下文。
                线程的主要数据结构包括：
                    ETHREAD：执行线程块；
                        主要包括线程所属进程的指针和线程开始控制的子程序的地址。
                        也包括相应KTHREAD的指针

                    KTHREAD：内核线程块；
                        线程的调度和同步信息。
                        也包括内核栈（线程在内核模式下运行）和TEB指针

                    TEB：线程执行环境块；
                        包括用户模式栈和用户特定数据的数组，（Windows称之为线程本地存储）

                    ETHREAD和KTHREAD完全处于内核空间，只有内核可以访问。
                    TEB是用户空间的数据结构，供线程在用户模式下运行时访问。


        b，Linux线程
            Linux并不区分 线程 和 进程
            在讨论程序控制流时，通常称之为任务而不是 进程 或 线程。
            系统中每个任务都有一个唯一的内核数据结构（struct task_struct），
            这个 数据结构 并不保存 任务本身的数据，而是 指向 其他存储这些数据的
            数据结构指针----如表示 打开文件列表，信号处理信息 和 虚拟内存等的数据结构。








