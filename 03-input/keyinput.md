```C
#include "main.h"
#include "datafile.h"


int keyfd = -1;

struct input_event keyinput;

int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        printf("./xxx /dev/input/eventx\n");
    }
    if (0 > (keyfd = open(argv[1], O_RDONLY)))
    {
        perror("open()");
        exit(-1);
    }
    while (1)
    {
        if (sizeof(struct input_event) != read(keyfd, &keyinput, sizeof(struct input_event)))
        {
            perror("read()");
            exit(-1);
        }
        decode_input_event(&keyinput);
        printf("type:%d code:%d value:%d\n", keyinput.type, keyinput.code, keyinput.value);
    }
    return 0;
}

```