```C
#include "main.h"
#include "datafile.h"

#define KEY_INPUT_PATH "/dev/input/event2"

int tsfd = -1;

struct input_event tsinput;

int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        printf("./xxx /dev/input/eventx\n");
    }
    if (0 > (tsfd = open(argv[1], O_RDONLY)))
    {
        perror("open()");
        exit(-1);
    }
    while (1)
    {
        if (sizeof(struct input_event) != read(tsfd, &tsinput, sizeof(struct input_event)))
        {
            perror("read()");
            exit(-1);
        }
        decode_input_event(&tsinput);
        printf("type:%d code:%d value:%d\n", tsinput.type, tsinput.code, tsinput.value);
    }
    return 0;
}

```