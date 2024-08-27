## 非阻塞IO
### 添加 O_NONBLOCK 标志位

    int flag = 0;
    flag = fcntl(0, F_GETFL);
    flag |= O_NONBLOCK;
    ret = fcntl(0, F_SETFL, flag);


    mouse_fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
