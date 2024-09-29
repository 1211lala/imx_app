#include "./lvgl_ui/datafile.h"
#include "./lvgl_ui/lvgl_ui.h"

#define MAIN_OBJ_NUM 4

void main_obj_press_cb_fun(lv_event_t *e);
void main_obj_long_cb_fun(lv_event_t *e);

/* 外部资源 */
LV_IMG_DECLARE(lvgl_image_background);

lv_obj_t *main_backgroundimg = NULL;

typedef struct MY_MAIN_OBJ
{
    lv_obj_t *obj;
    lv_obj_t *label;
    uint16_t xsize;
    uint16_t ysize;
    uint16_t x;
    uint16_t y;
    bool isSelected;
} MAIN_OBJ;

MAIN_OBJ *main_obj = NULL;

void lvgl_main_ui(void)
{
    main_backgroundimg = lv_img_create(lv_scr_act());
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    /* 创建主背景图片 */
    lv_obj_set_y(main_backgroundimg, 0);
    lv_obj_set_x(main_backgroundimg, 0);
    lv_img_set_src(main_backgroundimg, &lvgl_image_background);
    /* 为主界面的obj分配内存 */
    main_obj = malloc(sizeof(MAIN_OBJ) * MAIN_OBJ_NUM);

    for (int i = 0; i < MAIN_OBJ_NUM; i++)
    {
        switch (i)
        {
        case (0):
            main_obj[i].obj = lv_obj_create(main_backgroundimg);
            main_obj[i].xsize = 100;
            main_obj[i].ysize = 100;
            main_obj[i].isSelected = false;
            lv_obj_set_pos(main_obj[i].obj, 100, 0);
            lv_obj_set_size(main_obj[i].obj, main_obj[i].xsize, main_obj[i].ysize);
            lv_obj_set_style_bg_color(main_obj[i].obj, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
            break;
        case (1):
            main_obj[i].obj = lv_obj_create(main_backgroundimg);
            main_obj[i].xsize = 100;
            main_obj[i].ysize = 100;
            main_obj[i].isSelected = false;
            lv_obj_set_pos(main_obj[i].obj, 200, 0);
            lv_obj_set_size(main_obj[i].obj, main_obj[i].xsize, main_obj[i].ysize);
            lv_obj_set_style_bg_color(main_obj[i].obj, lv_color_hex(0xf00ff00), LV_STATE_DEFAULT);
            break;
        case (2):
            main_obj[i].obj = lv_obj_create(main_backgroundimg);
            main_obj[i].xsize = 100;
            main_obj[i].ysize = 100;
            main_obj[i].isSelected = false;
            lv_obj_set_pos(main_obj[i].obj, 300, 0);
            lv_obj_set_size(main_obj[i].obj, main_obj[i].xsize, main_obj[i].ysize);
            lv_obj_set_style_bg_color(main_obj[i].obj, lv_color_hex(0x0000ff), LV_STATE_DEFAULT);
            break;
        default:
            main_obj[i].obj = lv_obj_create(main_backgroundimg);
            main_obj[i].xsize = 100;
            main_obj[i].ysize = 100;
            main_obj[i].isSelected = false;
            lv_obj_set_pos(main_obj[i].obj, 0, 0);
            lv_obj_set_size(main_obj[i].obj, main_obj[i].xsize, main_obj[i].ysize);
            lv_obj_set_style_bg_color(main_obj[i].obj, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
            break;
        }
        lv_obj_add_event_cb(main_obj[i].obj, main_obj_press_cb_fun, LV_EVENT_PRESSED, NULL);
        lv_obj_add_event_cb(main_obj[i].obj, main_obj_long_cb_fun, LV_EVENT_LONG_PRESSED, NULL);
    }
}

/* 将主界面的未被选中的obj.isSelected 设置为 false */
void main_obj_uncheck_other(MAIN_OBJ *obj)
{
    for (int i = 0; i < MAIN_OBJ_NUM; i++)
    {
        if (main_obj + i != obj)
        {
            (main_obj + i)->isSelected = false;
        }
        else
        {
            (main_obj + i)->isSelected = true;
        }
    }
}
/* 将主界面所有的obj.isSelected 设置为 false */
void main_obj_uncheck(MAIN_OBJ *obj)
{
    for (int i = 0; i < MAIN_OBJ_NUM; i++)
    {
        (main_obj + i)->isSelected = false;
    }
}

bool isok = false;
void main_obj_press_cb_fun(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    for (int i = 0; i < MAIN_OBJ_NUM; i++)
    {
        if (target == main_obj[i].obj)
        {
            main_obj_uncheck_other((main_obj + i));
            isok = true;
            break;
        }
    }
}
void main_obj_long_cb_fun(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    int i = 0;
    for (i = 0; i < MAIN_OBJ_NUM; i++)
    {
        if (target == main_obj[i].obj)
        {
            break;
        }
    }
    printf("x: %d y: %d\n", main_obj[i].x, main_obj[i].y);
}
void *lvgl_ui_task(void *arg)
{
    /* 初始化LVGL */
    lv_init();
    /* 打开屏幕设置设备 */
    fbdev_init();
    /* 采用双buf */
    static lv_disp_draw_buf_t draw_buf_dsc_3;
    static lv_color_t buf_3_1[DISP_BUF_SIZE];
    static lv_color_t buf_3_2[DISP_BUF_SIZE];
    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    /* 初始化 lv_disp_drv_t 数据 设置默认数据 */
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf_dsc_3;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = 1024;
    disp_drv.ver_res = 600;
    disp_drv.full_refresh = 1;
    /* 向lvgl注册一个屏幕 */
    lv_disp_drv_register(&disp_drv);
    /* 打开输入设备 */
    evdev_init();
    lv_indev_drv_t indev_drv_1;
    /* 初始化 lv_indev_drv_t 数据 设置默认数据 */
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = evdev_read;
    /* 注册一个输入设备 */
    lv_indev_t *ts_indev = lv_indev_drv_register(&indev_drv_1);
    printf("/*********************lvgl初始化成功***********************/\n");
    lvgl_main_ui();
    int cnt = 0;
    while (1)
    {
        if (isok == true)
        {
            for (int i = 0; i < MAIN_OBJ_NUM; i++)
            {
                if (main_obj[i].isSelected == true)
                {
                    main_obj[i].x = ts.x;
                    main_obj[i].y = ts.y;
                    lv_obj_set_pos(main_obj[i].obj, main_obj[i].x - main_obj[i].xsize / 2, main_obj[i].y - main_obj[i].ysize / 2);
                }
            }
            if (!ts.status)
            {
                if (cnt++ > 5)
                {
                    cnt = 0;
                    isok = false;
                    main_obj_uncheck(main_obj);
                }
            }
        }
        usleep(50);
    }
}

void *lvgl_tick_task(void *arg)
{
    while (1)
    {
        lv_timer_handler();
        usleep(5000);
    }
}