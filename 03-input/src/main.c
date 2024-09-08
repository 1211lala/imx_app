/******************************************************
 * 读取单点触摸的数据
 *****************************************************/
#include "main.h"
#include "datafile.h"
#include "tslib.h"

#define TS_BLOCK 0
#define TS_NO_BLOCK 1

int main(int argc, char *argv[])
{
    struct tsdev *ts;
    struct ts_sample sample;
    unsigned int old_pressure = 0;
    if (argc != 2)
    {
        printf("argc \r\n");
        return -1;
    }
    if (NULL == (ts = ts_setup(argv[1], TS_BLOCK)))
    {
        printf("%s open error\n", argv[1]);
        return -1;
    }
    while (1)
    {
        if (0 > ts_read(ts, &sample, 1))
        {
            printf("ts_read()\r\n");
            return -1;
        }
        if (sample.pressure)
        {
            if (old_pressure)
            {
                printf("移动[x]: %d   [y]: %d\n", sample.x, sample.y);
            }
            else
            {
                printf("按下[x]: %d   [y]: %d\n", sample.x, sample.y);
            }
        }
        else
        {
            printf("松开\r\n");
        }
        old_pressure = sample.pressure;
    }
    ts_close(ts);
    return 0;
}