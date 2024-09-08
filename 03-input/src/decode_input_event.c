#include "decode_input_event.h"

void decode_ev_key(struct input_event *event)
{
    printf("按键类事件\t\t");
    printf("键值 %d\t", event->code);
    switch (event->value)
    {
    case (0):
        printf("释放\t\n");
        break;
    case (1):
        printf("按下\t\n");
        break;
    case (2):
        printf("长按\t\n");
        break;
    default:
        break;
    }
}

void decode_ev_abs(struct input_event *event)
{
    static u_int8_t pointNum = 0;

    switch (event->code)
    {
    case (ABS_MT_TRACKING_ID):
        if (event->value == -1)
            pointNum -= 1;
        else
            pointNum += 1;
        printf("触摸点 %d\n", pointNum);
        break;
    case (ABS_MT_SLOT):
        printf("触摸点 %d\n", event->value + 1);
        break;
    case (ABS_MT_POSITION_X):
        printf("POSITION_X:%d\n", event->value);
        break;
    case (ABS_MT_POSITION_Y):
        printf("POSITION_Y:%d\n", event->value);
        break;
    case (ABS_X):
        printf("X:%d\n", event->value);
        break;
    case (ABS_Y):
        printf("Y:%d\n", event->value);
        break;
    }
}

void decode_input_event(struct input_event *event)
{
    switch (event->type)
    {
    case (EV_SYN):
        if (event->code == 0)
        {
            if (event->value == 0)
            {
                printf("同步事件\t\n\n");
            }
        }
        break;
    case (EV_KEY):
        decode_ev_key(event);
        break;
    case (EV_REL):
        printf("相对位移类事件\t\n");
        break;
    case (EV_ABS):
        printf("绝对位移类事件\t\n");
        decode_ev_abs(event);
        break;
    case (EV_MSC):
        printf("其它杂类事件\t\n");
        break;
    }
}
