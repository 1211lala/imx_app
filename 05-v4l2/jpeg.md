### 测试程序
```C
#include "main.h"
#include "datafile.h"
#include "jpeg.h"

struct _lcddev lcddev = {
    .init = rgblcd_init,
    .drawPonit = lcd_draw_point,
    .delete = lcd_delete,
};

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("/XXXX /xxx/xxx.jpg  xpos  ypos");
        exit(-1);
    }
    if (0 != lcddev.init(&lcddev))
    {
        printf("rgblcd_init failed!!");
    }
    memset(lcddev.screenBase, 0xFF, lcddev.screenSize);
    show_jpeg_image(&lcddev, argv[1], atoi(argv[2]), atoi(argv[3]));
    usleep(100000);

    lcddev.delete(&lcddev);
}

```