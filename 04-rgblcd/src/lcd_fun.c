#include "datafile.h"



int rgblcd_init(struct _lcddev *lcd)
{
    /* 固定参数 */
    struct fb_fix_screeninfo fb_fix;
    /* 可变参数 */
    struct fb_var_screeninfo fb_var;
    if (0 > (lcd->fd = open("/dev/fb0", O_RDWR)))
    {
        perror("");
    }
    ioctl(lcd->fd, FBIOGET_VSCREENINFO, &fb_var);

    lcd->height = fb_var.yres;
    lcd->width = fb_var.xres;

    lcd->bpp = fb_var.bits_per_pixel;
    lcd->screenSize = lcd->width * lcd->height * (lcd->bpp / 8);
    lcd->screenBase = mmap(NULL, lcd->screenSize, PROT_WRITE, MAP_SHARED, lcd->fd, 0);

    lcd->lineSize = lcd->width * (lcd->bpp / 8);
    #ifdef USE_VER_SCREEN
    lcd->lineSize = lcd->height * (lcd->bpp / 8);
    #endif

    if (MAP_FAILED == (void *)lcd->screenBase)
    {
        perror("");
        close(lcd->fd);
        exit(-1);
    }
    printf("***************** Screen Information *****************\n");
    printf("*                                                    *\n");
    printf("*   Resolution : %4d x %-4d pixels                    *\n", lcd->width, lcd->height);
    printf("*   Color Depth: %4d bytes per pixel                 *\n", lcd->bpp / 8);
    printf("*                                                    *\n");
    printf("******************************************************\n");
    return 0;
}

void lcd_delete(struct _lcddev *lcd)
{
    munmap(lcd->screenBase, lcd->screenSize);
    close(lcd->fd);
    printf("lcd memery release success!!!\n");
    exit(-1);
}
void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color)
{
#ifdef USE_VER_SCREEN
    if (x >= lcddev.height)
        return;
    if (y >= lcddev.width)
        return;
    lcddev.screenBase[(lcddev.height - 1 - x) * lcddev.width + y] = color;
#else
    if (x >= lcddev.width)
        return;
    if (y >= lcddev.height)
        return;
    lcddev.screenBase[y * lcddev.width + x] = color;
#endif
}