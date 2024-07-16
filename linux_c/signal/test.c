#include "stdio.h"
#include <signal.h>
#include <unistd.h>
#include "sys/types.h"
#include <stdlib.h>

void sig_handler(int sig, siginfo_t *info, void *context)
{
    sigval_t sig_val = info->si_value;

    printf("接收到实时信号: %d\n", sig);
    printf("伴随数据为: %d\n", sig_val.sival_int);
    printf("PID: %d\n", info->si_pid);
    printf("UID: %d\n", info->si_uid);
}
int main(int argc, char **argv)
{
    struct sigaction sig = {0};

    sig.sa_sigaction = sig_handler;
    sig.sa_flags = SA_SIGINFO;

    if (sigaction(35, &sig, NULL) < 0)
    {
        perror("sigaction error");
        exit(-1);
    }
    while (1)
    {
        sleep(1);
    }
    return 0;
}