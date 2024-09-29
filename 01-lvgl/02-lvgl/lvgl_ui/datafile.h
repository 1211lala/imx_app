#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "linux/ioctl.h"
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include "unistd.h"

#include "stdio.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <semaphore.h>

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include "lvgl/src/extra/libs/lv_libs.h"

#define SCREEN_HOR 600
#define SCREEN_VER 1024
#define DISP_BUF_SIZE (SCREEN_HOR * SCREEN_VER)


