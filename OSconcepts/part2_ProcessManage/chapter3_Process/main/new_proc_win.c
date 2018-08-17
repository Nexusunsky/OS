#include <stdio.h>
#include <windows.h>

int main(VOID)
{
    STARTUPINFO si; //指明新进程的许多特性，如长窗口大小，标准输入及输出文件的句柄。
    PROCESS_INFORMATION pi;//包含一个句柄以及新生成进程和线程的标识

    // 为两个结构清空内存
    //allocate memory
    ZeroMemory(&si ,sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi ,sizeof(pi));

    //create child process
    if (!CreateProcess(
    NULL, //use command line ，指明的是应用名，NULL表示命令行参数指明要装入的应用
    "C:\\WINDOWS\\system32\\mspaint.exe", //command line

    /*使用系统默认参数来继承进程和线程句柄和指定不创建标志，还使用父进程的已有环境块和启动目录*/
    NULL, //don`t inherit process handle
    NULL, //don`t inherit process handle
    FALSE, //disable handle inheritance
    0, //no creation flags
    NULL, //use parent's environment block
    NULL, //use parent's existing directiry

    /*提供了两个指向程序刚开始生成的结构指针*/
    &si,
    &pi))
    {
        fprintf(stderr, "create Process failed");
        return -1;
    }

    //parent will wait for the child to complete
    /*参数指定子进程的句柄，即等待进程结束，一旦子进程结束，控制将从函数 WaitForSingleObject 返回到父进程*/
    WaitForSingleObject(pi.hProcess, INFINITE);
    Print("Child Complete");

    //close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}