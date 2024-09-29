#include "datafile.h"

lv_obj_t *tileview;
lv_obj_t *tile_1;
lv_obj_t *tile_2;
lv_obj_t *tile_3;
lv_obj_t *tile_4;
lv_obj_t *tile_5;
/***************************************************/
lv_obj_t *tile1_top_obj_1;
lv_obj_t *tile1_top_obj_2;

lv_obj_t *tile1_mid_obj_1;
lv_obj_t *tile1_bot_obj_1;

lv_obj_t *tile1_mid_obj_2;
lv_obj_t *tile1_bot_obj_2;

lv_obj_t *tile1_mid_obj_3;
lv_obj_t *tile1_bot_obj_3;

lv_obj_t *tile1_mid_obj_4;
#define TILE1_BLACK_COLOR 0XFF0000
/***************************************************/
lv_obj_t *tile2_top_obj_1;
lv_obj_t *tile2_top_obj_2;

lv_obj_t *tile2_mid_obj_1;
lv_obj_t *tile2_bot_obj_1;

lv_obj_t *tile2_mid_obj_2;
lv_obj_t *tile2_bot_obj_2;

lv_obj_t *tile2_mid_obj_3;
lv_obj_t *tile2_bot_obj_3;

lv_obj_t *tile2_mid_obj_4;
/****************************************************/
lv_obj_t *tabview;
/****************************************************/
lv_obj_t *tab2_beep_btn;
lv_obj_t *tab1_led_btn;
lv_obj_t *tab3_tabview_tab1_label;
lv_obj_t *tab3_tabview_tab2_label;
lv_obj_t *tab3_tabview_tab3_label;

lv_obj_t *chart;
lv_obj_t *img1;

void chart_timer_cb(struct _lv_timer_t *timer);
lv_coord_t ap3216c_data[10] = {};
lv_coord_t xxx_data[10] = {};
#define TILE2_BLACK_COLOR 0X00FF00
void chart_draw_event_cb(lv_event_t *e);
void lvgl_ui(void)
{
    tileview = lv_tileview_create(lv_scr_act());

    tile_1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_VER);
    tile_2 = lv_tileview_add_tile(tileview, 0, 1, LV_DIR_VER);
    tile_3 = lv_tileview_add_tile(tileview, 0, 2, LV_DIR_VER);
    tile_4 = lv_tileview_add_tile(tileview, 0, 3, LV_DIR_VER);
    tile_5 = lv_tileview_add_tile(tileview, 0, 4, LV_DIR_VER);
    lv_obj_set_style_bg_color(tileview, lv_color_hex(0X000000), LV_STATE_DEFAULT);
    /********************************************************************************************************* */
    /* top */
    tile1_top_obj_1 = lv_obj_create(tile_1);
    lv_obj_set_size(tile1_top_obj_1, 800 - 8, 100);
    lv_obj_align(tile1_top_obj_1, LV_ALIGN_TOP_LEFT, 6, 4);
    lv_obj_set_style_bg_color(tile1_top_obj_1, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_top_obj_1, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_top_obj_1, 0, LV_STATE_DEFAULT);

    tile1_top_obj_2 = lv_obj_create(tile_1);
    lv_obj_set_size(tile1_top_obj_2, 215, 100);
    lv_obj_align(tile1_top_obj_2, LV_ALIGN_TOP_RIGHT, -6, 4);
    lv_obj_set_style_bg_color(tile1_top_obj_2, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_top_obj_2, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_top_obj_2, 0, LV_STATE_DEFAULT);

    /* middle */
    tile1_mid_obj_1 = lv_obj_create(tile_1);
    lv_obj_align(tile1_mid_obj_1, LV_ALIGN_LEFT_MID, 6, 0);
    lv_obj_set_size(tile1_mid_obj_1, 260, 380);
    lv_obj_set_style_bg_color(tile1_mid_obj_1, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_mid_obj_1, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_mid_obj_1, 0, LV_STATE_DEFAULT);
    tile1_bot_obj_1 = lv_obj_create(tile_1);
    lv_obj_align_to(tile1_bot_obj_1, tile1_mid_obj_1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile1_bot_obj_1, 260, 90);
    lv_obj_set_style_bg_color(tile1_bot_obj_1, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_bot_obj_1, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_bot_obj_1, 0, LV_STATE_DEFAULT);

    tile1_mid_obj_2 = lv_obj_create(tile_1);
    lv_obj_align_to(tile1_mid_obj_2, tile1_mid_obj_1, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_size(tile1_mid_obj_2, 260, 380);
    lv_obj_set_style_bg_color(tile1_mid_obj_2, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_mid_obj_2, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_mid_obj_2, 0, LV_STATE_DEFAULT);
    tile1_bot_obj_2 = lv_obj_create(tile_1);
    lv_obj_align_to(tile1_bot_obj_2, tile1_mid_obj_2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile1_bot_obj_2, 260, 90);
    lv_obj_set_style_bg_color(tile1_bot_obj_2, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_bot_obj_2, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_bot_obj_2, 0, LV_STATE_DEFAULT);

    tile1_mid_obj_3 = lv_obj_create(tile_1);
    lv_obj_align_to(tile1_mid_obj_3, tile1_mid_obj_2, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_size(tile1_mid_obj_3, 260, 380);
    lv_obj_set_style_bg_color(tile1_mid_obj_3, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_mid_obj_3, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_mid_obj_3, 0, LV_STATE_DEFAULT);
    tile1_bot_obj_3 = lv_obj_create(tile_1);
    lv_obj_align_to(tile1_bot_obj_3, tile1_mid_obj_3, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile1_bot_obj_3, 260, 90);
    lv_obj_set_style_bg_color(tile1_bot_obj_3, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_bot_obj_3, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_bot_obj_3, 0, LV_STATE_DEFAULT);

    tile1_mid_obj_4 = lv_obj_create(tile_1);
    lv_obj_align_to(tile1_mid_obj_4, tile1_top_obj_2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile1_mid_obj_4, 215, 476);
    lv_obj_set_style_bg_color(tile1_mid_obj_4, lv_color_hex(TILE1_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile1_mid_obj_4, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile1_mid_obj_4, 0, LV_STATE_DEFAULT);

    /********************************************************************** */
    /* top */
    tile2_top_obj_1 = lv_obj_create(tile_2);
    lv_obj_set_size(tile2_top_obj_1, 800 - 8, 100);
    lv_obj_align(tile2_top_obj_1, LV_ALIGN_TOP_LEFT, 6, 4);
    lv_obj_set_style_bg_color(tile2_top_obj_1, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_top_obj_1, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_top_obj_1, 0, LV_STATE_DEFAULT);

    tile2_top_obj_2 = lv_obj_create(tile_2);
    lv_obj_set_size(tile2_top_obj_2, 215, 100);
    lv_obj_align(tile2_top_obj_2, LV_ALIGN_TOP_RIGHT, -6, 4);
    lv_obj_set_style_bg_color(tile2_top_obj_2, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_top_obj_2, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_top_obj_2, 0, LV_STATE_DEFAULT);

    /* middle */
    tile2_mid_obj_1 = lv_obj_create(tile_2);
    lv_obj_align(tile2_mid_obj_1, LV_ALIGN_LEFT_MID, 6, 0);
    lv_obj_set_size(tile2_mid_obj_1, 260, 380);
    lv_obj_set_style_bg_color(tile2_mid_obj_1, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_mid_obj_1, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_mid_obj_1, 0, LV_STATE_DEFAULT);
    tile2_bot_obj_1 = lv_obj_create(tile_2);
    lv_obj_align_to(tile2_bot_obj_1, tile2_mid_obj_1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile2_bot_obj_1, 260, 90);
    lv_obj_set_style_bg_color(tile2_bot_obj_1, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_bot_obj_1, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_bot_obj_1, 0, LV_STATE_DEFAULT);

    tile2_mid_obj_2 = lv_obj_create(tile_2);
    lv_obj_align_to(tile2_mid_obj_2, tile2_mid_obj_1, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_size(tile2_mid_obj_2, 260, 380);
    lv_obj_set_style_bg_color(tile2_mid_obj_2, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_mid_obj_2, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_mid_obj_2, 0, LV_STATE_DEFAULT);
    tile2_bot_obj_2 = lv_obj_create(tile_2);
    lv_obj_align_to(tile2_bot_obj_2, tile2_mid_obj_2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile2_bot_obj_2, 260, 90);
    lv_obj_set_style_bg_color(tile2_bot_obj_2, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_bot_obj_2, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_bot_obj_2, 0, LV_STATE_DEFAULT);

    tile2_mid_obj_3 = lv_obj_create(tile_2);
    lv_obj_align_to(tile2_mid_obj_3, tile2_mid_obj_2, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);
    lv_obj_set_size(tile2_mid_obj_3, 260, 380);
    lv_obj_set_style_bg_color(tile2_mid_obj_3, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_mid_obj_3, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_mid_obj_3, 0, LV_STATE_DEFAULT);
    tile2_bot_obj_3 = lv_obj_create(tile_2);
    lv_obj_align_to(tile2_bot_obj_3, tile2_mid_obj_3, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile2_bot_obj_3, 260, 90);
    lv_obj_set_style_bg_color(tile2_bot_obj_3, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_bot_obj_3, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_bot_obj_3, 0, LV_STATE_DEFAULT);

    tile2_mid_obj_4 = lv_obj_create(tile_2);
    lv_obj_align_to(tile2_mid_obj_4, tile2_top_obj_2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_obj_set_size(tile2_mid_obj_4, 215, 476);
    lv_obj_set_style_bg_color(tile2_mid_obj_4, lv_color_hex(TILE2_BLACK_COLOR), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tile2_mid_obj_4, 180, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tile2_mid_obj_4, 0, LV_STATE_DEFAULT);

    lv_obj_set_tile(tileview, tile_3, LV_ANIM_OFF);

    /**************************************************************************************************** */
    tabview = lv_tabview_create(tile_3, LV_DIR_LEFT, 150);
    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *btns = lv_tabview_get_tab_btns(tabview);

    lv_obj_set_style_text_font(btns, &lv_font_montserrat_20, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btns, lv_palette_main(LV_PALETTE_INDIGO), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(btns, lv_color_hex(0XFFFFFF), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(btns, lv_palette_main(LV_PALETTE_AMBER), LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_obj_set_style_text_font(btns, &lv_font_montserrat_26, LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(btns, lv_palette_main(LV_PALETTE_INDIGO), LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btns, lv_color_hex(0XFFFFFF), LV_STATE_DEFAULT);

    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "LED");
    lv_obj_set_style_bg_color(tab1, lv_color_hex(0XFFFFFF), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tab1, LV_OPA_COVER, LV_STATE_DEFAULT);

    tab1_led_btn = lv_btn_create(tab1);
    lv_obj_set_size(tab1_led_btn, 200, 130);
    lv_obj_align(tab1_led_btn, LV_ALIGN_CENTER, 0, -100);
    lv_obj_set_style_bg_color(tab1_led_btn, lv_palette_lighten(LV_PALETTE_BLUE, 3), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tab1_led_btn, lv_palette_lighten(LV_PALETTE_BLUE, 1), LV_STATE_PRESSED);

    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "BEEP");
    tab2_beep_btn = lv_btn_create(tab2);
    lv_obj_set_size(tab2_beep_btn, 200, 130);
    lv_obj_align(tab2_beep_btn, LV_ALIGN_CENTER, 0, -100);
    lv_obj_set_style_bg_color(tab2_beep_btn, lv_palette_lighten(LV_PALETTE_LIGHT_GREEN, 3), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tab2_beep_btn, lv_palette_lighten(LV_PALETTE_LIGHT_GREEN, 1), LV_STATE_PRESSED);

    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "AP3216C");

    lv_obj_t *tab3_tabview = lv_tabview_create(tab3, LV_DIR_TOP, 80);
    lv_obj_t *tab3_tabview_btns = lv_tabview_get_tab_btns(tab3_tabview);
    lv_obj_set_style_text_font(tab3_tabview_btns, &lv_font_montserrat_18, LV_STATE_DEFAULT);

    lv_obj_t *tab3_tabview_tab1 = lv_tabview_add_tab(tab3_tabview, "ALS");
    lv_obj_t *tab3_tabview_tab2 = lv_tabview_add_tab(tab3_tabview, "PS");
    lv_obj_t *tab3_tabview_tab3 = lv_tabview_add_tab(tab3_tabview, "IR");

    tab3_tabview_tab1_label = lv_label_create(tab3_tabview_tab1);
    lv_label_set_text_fmt(tab3_tabview_tab1_label, "%d", ap3216c.als);
    lv_obj_align(tab3_tabview_tab1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(tab3_tabview_tab1_label, &lv_font_montserrat_40, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tab3_tabview_tab1_label, lv_palette_main(LV_PALETTE_BLUE), LV_STATE_DEFAULT);

    tab3_tabview_tab2_label = lv_label_create(tab3_tabview_tab2);
    lv_obj_align(tab3_tabview_tab2_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text_fmt(tab3_tabview_tab2_label, "%d", ap3216c.ps);
    lv_obj_set_style_text_font(tab3_tabview_tab2_label, &lv_font_montserrat_40, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tab3_tabview_tab2_label, lv_palette_main(LV_PALETTE_BLUE), LV_STATE_DEFAULT);

    tab3_tabview_tab3_label = lv_label_create(tab3_tabview_tab3);
    lv_obj_align(tab3_tabview_tab3_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text_fmt(tab3_tabview_tab3_label, "%d", ap3216c.ir);
    lv_obj_set_style_text_font(tab3_tabview_tab3_label, &lv_font_montserrat_40, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tab3_tabview_tab3_label, lv_palette_main(LV_PALETTE_BLUE), LV_STATE_DEFAULT);

    lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "NETWORK");
    lv_obj_t *tab4_tabview = lv_tabview_create(tab4, LV_DIR_TOP, 60);
    lv_obj_t *tab4_tabview_btns = lv_tabview_get_tab_btns(tab4_tabview);
    lv_obj_set_style_text_font(tab4_tabview_btns, &lv_font_montserrat_26, LV_STATE_DEFAULT);

    lv_obj_t *tab4_tabview_tab2 = lv_tabview_add_tab(tab4_tabview, "Client");

    lv_obj_t *tab4_tabview_tab1 = lv_tabview_add_tab(tab4_tabview, "Server");

    lv_obj_t *tab5 = lv_tabview_add_tab(tabview, "Tab_5");
    lv_obj_t *tab5_label = lv_label_create(tab5);
    lv_label_set_text(tab5_label, "tab-5");

    lv_obj_add_event_cb(tab1_led_btn, tabview_btn_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(tab1_led_btn, tabview_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(tab2_beep_btn, tabview_btn_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(tab2_beep_btn, tabview_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_tabview_set_act(tabview, 3, LV_ANIM_OFF);
    /**************************************************************************************************** */
    img1 = lv_img_create(tile_4);
    lv_img_set_src(img1, "L:/lvgl_src/sd_256.bin");
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);

    /****************************************************************************************************/
    lv_obj_set_tile(tileview, tile_5, LV_ANIM_OFF);

    lv_timer_t *timer = lv_timer_create(chart_timer_cb, 50, NULL);
    /* 创建图表部件 */
    chart = lv_chart_create(tile_5);
    /* 设置图表大小 */
    lv_obj_set_size(chart, 900, 500);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 21, 3, true, 50);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 2000);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 10, 1, true, 50);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 10);
    /* 中间对齐 */
    lv_chart_set_point_count(chart, 10);
    lv_obj_center(chart);
    /* 设置折线图类型 */
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    /* 添加数据序列 */
    lv_obj_add_event_cb(chart, chart_draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

    lv_chart_series_t *ser1 = lv_chart_add_series(chart,
                                                  lv_color_hex(0x000fff),
                                                  LV_CHART_AXIS_SECONDARY_X);
    lv_chart_series_t *ser2 = lv_chart_add_series(chart,
                                                  lv_palette_main(LV_PALETTE_GREEN),
                                                  LV_CHART_AXIS_SECONDARY_X);
    // /* 为 Y 轴数据点设置一个外部阵列 */
    lv_chart_set_ext_y_array(chart, ser1, (lv_coord_t *)ap3216c_data);
    lv_chart_set_ext_y_array(chart, ser2, (lv_coord_t *)xxx_data);
    lv_chart_set_div_line_count(chart, 21, 10);

    lv_obj_set_style_line_width(chart, 5, LV_PART_ITEMS);
    lv_obj_set_style_width(chart, 5, LV_PART_INDICATOR);
    lv_obj_set_style_height(chart, 5, LV_PART_INDICATOR);

    // lv_chart_cursor_t *cursor = lv_chart_add_cursor(chart, lv_color_hex(0x0000ff), LV_DIR_ALL);
    // lv_point_t point = {1000, 100};
    // lv_chart_set_cursor_pos(chart, cursor, &point);

    lv_obj_set_style_bg_color(chart, lv_palette_main(LV_PALETTE_PINK), LV_PART_MAIN);
    lv_chart_set_zoom_x(chart, 400);

    // /* 更新图表 */
    lv_chart_refresh(chart);
}

void chart_draw_event_cb(lv_event_t *e)
{
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL))
        return;

    if (dsc->id == LV_CHART_AXIS_PRIMARY_X && dsc->text)
    {
        const char *month[] = {"T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8", "T9", "T10"};
        lv_snprintf(dsc->text, dsc->text_length, "%s", month[dsc->value]);
    }
}

void chart_timer_cb(struct _lv_timer_t *timer)
{

    for (uint32_t i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            ap3216c_data[i] = ap3216c.ps;
        }
        else
        {
            ap3216c_data[i] = ap3216c_data[i + 1];
        }
    }
    lv_chart_refresh(chart);
}
void tabview_btn_cb(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (target == tab1_led_btn)
    {
        if (code == LV_EVENT_CLICKED)
        {
            ctl_char_io("/dev/led_01_03", "off");
        }
        if (code == LV_EVENT_PRESSED)
        {
            ctl_char_io("/dev/led_01_03", "on");
        }
    }
    else if (target == tab2_beep_btn)
    {
        if (code == LV_EVENT_CLICKED)
        {
            ctl_char_io("/dev/beep_05_01", "off");
        }
        if (code == LV_EVENT_PRESSED)
        {
            ctl_char_io("/dev/beep_05_01", "on");
        }
    }
}

void ctl_char_io(const char *dev, const char *status)
{
    int fd = open(dev, O_RDWR);
    if (strcmp(status, "on") == 0)
    {
        uint8_t cmd = 1;
        write(fd, &cmd, 1);
    }
    else
    {
        uint8_t cmd = 0;
        write(fd, &cmd, 1);
    }
    close(fd);
}

LV_FONT_DECLARE(f24);
void *lvgl_ui_task(void *arg)
{

    // lv_obj_t *label = lv_label_create(lv_scr_act());
    // lv_obj_set_style_text_font(label, &f24, LV_STATE_DEFAULT);
    // lv_label_set_text(label, "111你好sssssssssssssssssssssssssssssssssssssssssssssssd");
    // lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), LV_STATE_DEFAULT);
    // lv_obj_align(label, LV_ALIGN_CENTER, 100, 0);
    // lv_obj_t *label2 = lv_label_create(lv_scr_act());
    // lv_obj_set_style_text_font(label2, &lv_font_montserrat_48, LV_STATE_DEFAULT);
    // lv_label_set_text_fmt(label2, "%s", "11111");
    // lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_text_color(label2, lv_color_hex(0xFF0000), LV_STATE_DEFAULT);
    // while (1)
    // {
    //     usleep(10000);
    // }
    lvgl_ui();
    while (1)
    {
        static int flag = 0;
        static int cnt = 0;
        if (cnt++ > 50)
        {
            flag++;
            if (flag == 1)
            {
                img1 = lv_img_create(tile_4);
                lv_img_set_src(img1, "L:/lvgl_src/set_48.bin");
                lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
            }
            else if (flag == 2)
            {
                img1 = lv_img_create(tile_4);
                lv_img_set_src(img1, "L:/lvgl_src/trash.bin");
                lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
            }
            else if (flag = 3)
            {
                flag = 0;
                img1 = lv_img_create(tile_4);
                lv_img_set_src(img1, "L:/lvgl_src/sd_256.bin");
                lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
            }

            cnt = 0;
            lv_label_set_text_fmt(tab3_tabview_tab1_label, "%d", ap3216c.als);
            lv_label_set_text_fmt(tab3_tabview_tab2_label, "%d", ap3216c.ps);
            lv_label_set_text_fmt(tab3_tabview_tab3_label, "%d", ap3216c.ir);
        }
        usleep(10000);
    }
}

#if 0 
void lv_btn_callback(lv_event_t *e)
{
    lv_obj_t *target = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (target == btn2)
    {
        if (code == LV_EVENT_CLICKED)
        {
            ctl_char_io("/dev/beep_05_01", "off");
        }
        if (code == LV_EVENT_PRESSED)
        {
            ctl_char_io("/dev/beep_05_01", "on");
        }
    }
    if (target == btn1)
    {
        if (code == LV_EVENT_CLICKED)
        {
            ctl_char_io("/dev/led_01_03", "off");
        }
        if (code == LV_EVENT_PRESSED)
        {
            ctl_char_io("/dev/led_01_03", "on");
        }
    }
}

#endif