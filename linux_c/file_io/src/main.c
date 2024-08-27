/**
 *  ./bin/main ../data.csv ../result.csv
 */

#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static int read_line(int fd, char *Rbuf)
{
    int i = 0;
    char c;
    while (1)
    {
        int cnt = read(fd, &c, 1);
        if (cnt <= 0)
        {
            i = -1;
            break;
        }
        if (c == '\r' || c == '\n')
        {
            break;
        }
        else
        {
            Rbuf[i] = c;
            i += 1;
        }
    }
    return i;
}

int process_data(char *dataBuf, char *resultBuff)
{
    char name[100];
    int scores[3];
    int sum;
    char *level;
    if (dataBuf[0] == ',')
    {
        sprintf(dataBuf, "%s\r\n", dataBuf);
        strcpy(resultBuff, dataBuf);
    }
    else
    {
        sscanf(dataBuf, "%[^,],%d,%d,%d,", name, &scores[0], &scores[1], &scores[2]);

        sum = scores[0] + scores[1] + scores[2];
        if (sum > 250)
        {
            level = "A+";
        }
        else if (sum > 200)
        {
            level = "B+";
        }
        else
        {
            level = "C+";
        }
        sprintf(resultBuff, "%s,%d,%d,%d,%d,%s\r\n", name, scores[0], scores[1], scores[2], sum, level);
    }
}

int write_data(int fd, char *wBuf)
{
    write(fd, wBuf, strlen(wBuf));
}

char *dataPath;
char *resultPath;
int dataFd;
int resultFd;
char Rxbuff[1024];
char Wxbuff[1024];
int seek_cnt = 0;
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("use %s  <data.csv>   <result.csv>\r\n", argv[0]);
        return -1;
    }
    dataPath = argv[1];
    resultPath = argv[2];

    int dataFd = open(dataPath, O_RDWR, 0644);
    if (dataFd < 0)
    {
        perror(dataPath);
        return -1;
    }

    int resultFd = open(resultPath, O_RDWR | O_CREAT, 0644);
    if (resultFd < 0)
    {
        perror(resultPath);
        return -1;
    }

    while (1)
    {
        int len = read_line(dataFd, Rxbuff);
        if (len < 0)
        {
            break;
        }
        else if (len == 0)
        {
        }
        else if (len > 0)
        {
            Rxbuff[len] = '\0';
            process_data(Rxbuff, Wxbuff);
            write_data(resultFd, Wxbuff);
        }
    }

    close(dataFd);
    close(resultFd);
    printf("\r\n**********************文件处理完成**********************\r\n");
    return 0;
}
