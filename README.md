# MultiThreading

C++11多线程编程，读这本[C++多线程](https://github.com/xiaoweiChen/CPP-Concurrency-In-Action-2ed-2019/blob/master/content/chapter1/1.4-chinese.md)

看这个[视频](https://www.bilibili.com/video/BV1xt411r7U2?from=search&seid=10832949688649776573)

## 1.4 Hello Concurrent World

thread 头文件声明管理线程的函数和类，保护共享数据的函数和类在其他头文件

每个线程都必须一个执行单元，对于应用程序来说，初始线程是main，但对于其他线程，可以再thread 对象的构造函数中指定其执行函数

join()让初始线程等待新创建的线程，不然主线程可能就会在新线程结束之前结束

这个例子不具备工业实用性，只是简单看看

## 2.1 