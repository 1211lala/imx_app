```C
/******************************************************
 * 读取单点触摸的数据
 *****************************************************/
#include "main.h"
#include "datafile.h"
#include "tslib.h"

int main(int argc, char *argv[])
{
    struct tsdev *ts = NULL;
    struct ts_sample samp;
    int pressure = 0;
    /* 打开并配置触摸屏设备 */
    ts = ts_setup("/dev/input/event1", 0);
    if (NULL == ts)
    {
        fprintf(stderr, "ts_setup error");
        exit(EXIT_FAILURE);
    }
    /* 读数据 */
    while (1)
    {
        if (0 > ts_read(ts, &samp, 1))
        {
            fprintf(stderr, "ts_read error");
            ts_close(ts);
            exit(EXIT_FAILURE);
        }
        if (samp.pressure)
        {                 // 按压力>0
            if (pressure) // 若上一次的按压力>0

                printf("移动(%d, %d)\n", samp.x, samp.y);
            else
                printf("按下(%d, %d)\n", samp.x, samp.y);
        }
        else
            printf("松开\n"); // 打印坐标
        pressure = samp.pressure;
    }
    ts_close(ts);
    exit(EXIT_SUCCESS);
}
```