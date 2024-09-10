#include "datafile.h"
#include "png.h"
#include "png_fun.h"

int show_png_image(struct _lcddev *lcd, const char *path, u_int16_t x, u_int16_t y)
{
    u_int32_t *addr = lcd->screenBase + (y * lcd->width + x);
    FILE *png_file = fopen(path, "r");
    if (png_file == NULL)
    {
        perror("");
        exit(-1);
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        perror("");
        fclose(png_file);
        exit(-1);
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(png_file);
        return -1;
    }
    /* 设置错误返回点 */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }

    /* 指定数据源 */
    png_init_io(png_ptr, png_file);
    /* 读取 png 文件 */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_ALPHA, NULL);
    u_int16_t image_h = png_get_image_height(png_ptr, info_ptr);
    u_int16_t image_w = png_get_image_width(png_ptr, info_ptr);
    printf("分辨率: %d*%d\n", image_w, image_h);

    /* 判断是不是 RGB888 */
    if ((8 != png_get_bit_depth(png_ptr, info_ptr)) && (PNG_COLOR_TYPE_RGB != png_get_color_type(png_ptr, info_ptr)))
    {
        printf("Error: Not 8bit depth or not RGB color");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }
    u_int16_t min_w, min_h;
    /* 判断图像和 LCD 屏那个的分辨率更低 */
    if (image_w > lcd->width)
        min_w = lcd->width;
    else
        min_w = image_w;
    if (image_h > lcd->height)
        min_h = lcd->height;
    else
        min_h = image_h;
    u_int32_t valid_bytes = min_w * 3; /* 这里分配三个字节内存 重点 */

    /* 读取解码后的数据 */
    u_int32_t *fb_line_buf = malloc(valid_bytes);
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr); // 获取数据
    for (int i = 0; i < min_h; i++)
    {
        if (y + i > lcd->height - 1)
            break; /* 防止内存溢出 */
        for (int j = 0; j < valid_bytes; j += 3)
        {
            if (x + j / 3 > lcd->width - 1)
                break; /* 防止内存溢出 */
            addr[j / 3] = row_pointers[i][j] << 16 | row_pointers[i][j + 1] << 8 | row_pointers[i][j + 2];
        }
        addr += lcd->width;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(fb_line_buf);
    fclose(png_file);
    return 0;
}

int png_decode(struct PNG_DATA *png_data, const char *path)
{
    FILE *png_file = fopen(path, "r");
    if (png_file == NULL)
    {
        perror("");
        exit(-1);
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        perror("");
        fclose(png_file);
        exit(-1);
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(png_file);
        return -1;
    }
    /* 设置错误返回点 */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }

    /* 指定数据源 */
    png_init_io(png_ptr, png_file);
    /* 读取 png 文件 */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_ALPHA, NULL);
    u_int16_t image_h = png_get_image_height(png_ptr, info_ptr);
    u_int16_t image_w = png_get_image_width(png_ptr, info_ptr);
    png_data->width = image_w;
    png_data->height = image_h;
    printf("分辨率: %d*%d\n", png_data->width, png_data->height);

    /* 判断是不是 RGB888 */
    if ((8 != png_get_bit_depth(png_ptr, info_ptr)) && (PNG_COLOR_TYPE_RGB != png_get_color_type(png_ptr, info_ptr)))
    {
        printf("Error: Not 8bit depth or not RGB color");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(png_file);
        return -1;
    }
    u_int32_t valid_bytes = png_data->width * 3; /* 这里分配三个字节内存 重点 */

    /* 读取解码后的数据 */
    u_int32_t cnt = 0;
    png_data->data = malloc(png_data->height * png_data->width * sizeof(u_int32_t));
    printf("分配: %d字节\n", png_data->height * png_data->width * sizeof(u_int32_t));
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr); // 获取数据
    for (int i = 0; i < png_data->height; i++)
    {
        for (int j = 0; j < valid_bytes; j += 3)
        {
            cnt = i * png_data->width + j / 3;
            png_data->data[cnt] = row_pointers[i][j] << 16 | row_pointers[i][j + 1] << 8 | row_pointers[i][j + 2];
        }
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(png_file);
    return 0;
}