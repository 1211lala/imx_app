#include "./lvgl_ui/datafile.h"

void lvgl_demo(void);
void *lvgl_tick_task(void *arg);

#define DISP_BUF_SIZE (600 * 1024)

int main(void)
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();
#if 0
    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = 1024;
    disp_drv.ver_res = 600;
    lv_disp_drv_register(&disp_drv);
#else
    static lv_disp_draw_buf_t draw_buf_dsc_3;
    static lv_color_t buf_3_1[DISP_BUF_SIZE];                                /*A screen sized buffer*/
    static lv_color_t buf_3_2[DISP_BUF_SIZE];                                /*Another screen sized buffer*/
    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, DISP_BUF_SIZE); /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf_dsc_3;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = 1024;
    disp_drv.ver_res = 600;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);
#endif
    evdev_init();
    lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    lvgl_demo();
    pthread_t tick_tid;
    pthread_t ui_tid;
    pthread_t ap3216c_tid;
    pthread_t network_tid;

    pthread_create(&tick_tid, NULL, lvgl_tick_task, NULL);
    pthread_create(&ui_tid, NULL, lvgl_ui_task, NULL);
    pthread_create(&ap3216c_tid, NULL, ap3216c_task, NULL);
    // pthread_create(&network_tid, NULL, network_task, NULL);
    pthread_join(tick_tid, NULL);
    pthread_join(ui_tid, NULL);
    pthread_join(ap3216c_tid, NULL);
    // pthread_join(network_tid, NULL);
}

void *lvgl_tick_task(void *arg)
{
    while (1)
    {
        lv_timer_handler();
        usleep(5000);
    }
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if (start_ms == 0)
    {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

void lvgl_demo(void)
{

    lv_fs_dir_t dir;
    lv_fs_res_t res;
    res = lv_fs_dir_open(&dir, "L:/");
    char fn[256];
    while (1)
    {
        res = lv_fs_dir_read(&dir, fn);
        if (strlen(fn) == 0)
        {
            break;
        }
        printf("%s\n", fn);
    }
    // lv_png_init();
    // lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_STATE_DEFAULT);
    // lv_obj_t *image = lv_img_create(lv_scr_act());
    // lv_img_set_src(image, "L:/img.png");
}
