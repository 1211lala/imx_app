## API
```C
/*
 * 打开触摸屏设备
 * dev_name 指定了触摸屏的设备节点；
 * 参数 nonblock 表示是否以非阻塞方式打开触摸屏设备
 *      --> nonblock 等于 0 表示阻塞方式，如果为非 0 值则表示以非阻塞方式打开。
*/
struct tsdev *ts_open(const char *dev_name, int nonblock);


/*
 * 打开触摸屏设备
 * 与 ts_open() 函数中的 dev_name 参数意义相同；但对于 ts_setup()来说，参数 dev_name 可* 以设置为 NULL，当 * * dev_name 设置为 NULL 时，ts_setup()函数内部会读取
 * TSLIB_TSDEVICE 环境变量，获取该环境变量的内容以得知触摸屏的设备节点。
 */
struct tsdev *ts_setup(const char *dev_name, int nonblock)

/**
 * 关闭触摸屏设备
*/
int ts_close(struct tsdev *);

/**
 * 指向触摸屏句柄
 * 成功返回 0，失败返回-1。
 * 所谓配置其实指的就是解析 ts.conf 文件中的配置信息
 */
int ts_config(struct tsdev *ts)


/**
 * 读取单点触摸数据
 * nr 表示对一个触摸点的采样数
 * 
*/
struct ts_sample {
 int x; //X 坐标
 int y; //Y 坐标
 unsigned int pressure; //按压力大小
 struct timeval tv; //时间
};
int ts_read(struct tsdev *ts, struct ts_sample *samp, int nr)


/**
 * 参数 ts 指向一个触摸屏设备句柄
 * max_slots 参数，表示触摸屏支持的最大触摸点数
 * nr 表示对一个触摸点的采样数，设置为 1 即可！
 * 
*/
struct ts_sample_mt {
 /* ABS_MT_* event codes. linux/include/uapi/linux/input-event-codes.h
 * has the definitions.
 */
 int x; //X 坐标
 int y; //Y 坐标
 unsigned int pressure; //按压力大小
 int slot; //触摸点 slot
 int tracking_id; //ID
 int tool_type;
 int tool_x;
 int tool_y;
 unsigned int touch_major;
 unsigned int width_major;
 unsigned int touch_minor;
 unsigned int width_minor;
 int orientation;
 int distance;
 int blob_id;
 struct timeval tv; //时间
 /* BTN_TOUCH state */
 short pen_down; //BTN_TOUCH 的状态
 /* valid is set != 0 if this sample
 * contains new data; see below for the
 * bits that get set.
 * valid is set to 0 otherwise
 */
 short valid; //此次样本是否有效标志 触摸点数据是否发生更新
};
int ts_read_mt(struct tsdev *ts, struct ts_sample_mt **samp, int max_slots, int nr)
```