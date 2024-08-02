## epoll()

### 触发方式
    1. 边缘触发
    2. 状态触发

###


```C
typedef union epoll_data /* 用户数据变量 */
{
  void *ptr;
  int fd;   
  uint32_t u32;
  uint64_t u64;
} epoll_data_t; 

struct epoll_event
{
  uint32_t events;	    /* Epoll 事件 */
  epoll_data_t data;	/* 用户数据变量 */
} __EPOLL_PACKED;


// 创建
int epoll_create1 (int __flags)
// 操作函数
/*
  __epfd : epoll实例的文件描述符
  __op  : EPOLL_CTL_ADD // 将fd添加到epoll中
          EPOLL_CTL_DEL // 将epoll中的fd删除
          EPOLL_CTL_MOD // 将fd的原来的 __event 修改为 新的 __event
  __fd : 监听的文件描符
  __event ：关联的描述信息
*/
int epoll_ctl (int __epfd, int __op, int __fd, struct epoll_event *__event)

/*
  __epfd : epoll实例的文件描述符
  __events 提供给内核用于返回已就绪的文件描述符信息
  __maxevents 可以返回的最大的文件描述符的数量
  __timeout 超时时间 
                    -1  永久等待
                     0  不等待
                     x  等待xms
*/
int epoll_wait (int __epfd, struct epoll_event *__events,int __maxevents, int __timeout);


```