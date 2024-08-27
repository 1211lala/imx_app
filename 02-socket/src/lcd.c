#include "datafile.h"
#include <sys/mman.h>
#include <linux/fb.h>

int width;                          // LCD X 分辨率
int height;                         // LCD Y 分辨率
unsigned short *screen_base = NULL; // 映射后的显存基地址

int lcd_get_info(void)
{
    unsigned int screen_size;
    int fd;
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;
    /* 打开 framebuffer 设备 */
    if (0 > (fd = open("/dev/fb0", O_RDWR)))
    {
        perror("open error");
        return -1;
    }
    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    screen_size = fb_fix.line_length * fb_var.yres;
    width = fb_var.xres;
    height = fb_var.yres;
    /* 将显示缓冲区映射到进程地址空间 */
    screen_base = mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == (void *)screen_base)
    {
        perror("mmap error");
        close(fd);
        return -1;
    }
    return 0;
}

/*******************************************************************************************/
/*******************************************************************************************/
/*******************************************************************************************/
pthread_t lcd_tid;
int lcd_show_thread_start(void)
{
    printf("lcd 线程开启\r\n");
    if (lcd_get_info() != 0)
    {
        exit(-1);
    }
    pthread_create(&lcd_tid, NULL, lcd_show, NULL);
    pthread_detach(lcd_tid);
}

void *lcd_show(void *arg)
{
    /* 画正方形方块 */
    int w = height * 0.25;                                            // 方块的宽度为 1/4 屏幕高度
    lcd_fill(0, width - 1, 0, height - 1, 0x0);                       // 清屏（屏幕显示黑色）
    lcd_fill(0, w, 0, w, 0xFF0000);                                   // 红色方块
    lcd_fill(width - w, width - 1, 0, w, 0xFF00);                     // 绿色方块
    lcd_fill(0, w, height - w, height - 1, 0xFF);                     // 蓝色方块
    lcd_fill(width - w, width - 1, height - w, height - 1, 0xFFFF00); // 黄色方块
    while (1)
    {
        sleep(1);
    }
}