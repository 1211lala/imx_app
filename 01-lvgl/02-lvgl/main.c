#include "./lvgl_ui/datafile.h"
#include "./lvgl_ui/lvgl_ui.h"

int main(void)
{
    pthread_t tick_tid;
    pthread_t ui_tid;

    pthread_create(&tick_tid, NULL, lvgl_tick_task, NULL);
    pthread_create(&ui_tid, NULL, lvgl_ui_task, NULL);
    pthread_join(ui_tid, NULL);
    pthread_join(tick_tid, NULL);
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

// void lvgl_demo(void)
// {

//     lv_fs_dir_t dir;
//     lv_fs_res_t res;
//     res = lv_fs_dir_open(&dir, "L:/");
//     char fn[256];
//     while (1)
//     {
//         res = lv_fs_dir_read(&dir, fn);
//         if (strlen(fn) == 0)
//         {
//             break;
//         }
//         printf("%s\n", fn);
//     }
//     // lv_png_init();
//     // lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_STATE_DEFAULT);
//     // lv_obj_t *image = lv_img_create(lv_scr_act());
//     // lv_img_set_src(image, "L:/img.png");
// }
