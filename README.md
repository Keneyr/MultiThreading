# MultiThreading

C++11多线程编程，读这本[C++多线程](https://github.com/xiaoweiChen/CPP-Concurrency-In-Action-2ed-2019/blob/master/content/chapter1/1.4-chinese.md)

看这个[视频](https://www.bilibili.com/video/BV1xt411r7U2?from=search&seid=10832949688649776573)

同时巩固[C++11的一些新特性](http://c.biancheng.net/view/7909.html) 这个网站很好，可惜高级教程收费


## 1.4 Hello Concurrent World

thread 头文件声明管理线程的函数和类，保护共享数据的函数和类在其他头文件

每个线程都必须一个执行单元，对于应用程序来说，初始线程是main，但对于其他线程，可以再thread 对象的构造函数中指定其执行函数

join()让初始线程等待新创建的线程，不然主线程可能就会在新线程结束之前结束

这个例子不具备工业实用性，只是简单看看

## 2.1 线程安全 

在实际应用中，原始线程要么有自己的工作要做，要么会启动多个子线程来做一些有用的工作，并**等待**这些线程结束。

Notepad++这种应该就是每次打开一个新文件就相当于开启一个新进程吧。只是这些write的操作所用到的function都是一样的而已

虽然每个窗口看起来都是独立的，但它们运行在同一个Notepad.exe 运行实例中，一种内部的处理方式是，让每个文档处理窗口拥有自己的线程。

每个线程运行同样的代码，并隔离不同窗口处理的数据。如此这般，打开一个文档就要启动一个新线程，因为是对独立文档进行操作，所以没有必要必须等待其他线程完成。

这里就可以让文档处理窗口运行在分离线程上。

```c++
void edit_document(std::string const& filename)
{
  open_document_and_display_gui(filename);
  while(!done_editing())
  {
    user_command cmd=get_user_input();
    if(cmd.type==open_new_document)
    {
      std::string const new_name=get_filename_from_user();
      std::thread t(edit_document,new_name);  // 1
      t.detach();  // 2
    }
    else
    {
       process_user_input(cmd);
    }
  }
}
```

如果用户选择打开一个新文档，需要启动一个新线程去打开新文档①，并分离线程②。与当前线程做出的操作一样，新线程只不过是打开另一个文件而已。所以，edit_document函数可以复用， 并通过传参的形式打开新的文件。

这个例子也展示了传参启动线程的方法：不仅可以向std::thread构造函数①传递函数名，还可以传递函数所需的参数(实参)。当然，也有其他方法可以完成这项功能，比如：使用带有数据的成员函数，代替需要传参的普通函数。

仅仅在这个小节里，我就已经看到多线程的代码是多么恶心了。。要照顾的情况实在是太多了。

try...catch...

join和detach什么时候该用，什么时候不该用，用在哪个地方都应该小心...blabla

## 2.2 传递参数

其实这时候我脑子已经有些晕了，理论不是经常说：

进程拥有独立的内存地址空间(堆、栈),线程没有独立的内存地址空间.

> 注意堆栈=栈，堆栈≠堆、栈

其实仔细来理解应该是，线程共享堆区，但是线程必须有独立的堆栈空间吧~

为什么这么说，看一看2.1的代码就会明白：

线程函数可以调用函数，而被调用函数中又是可以层层嵌套的，所以线程必须拥有自己的函数堆栈， 使得函数调用可以正常执行，不受其他线程的影响。

[进程，线程的地址空间问题](https://blog.csdn.net/qq_33921804/article/details/57403414?ops_request_misc=%25257B%252522request%25255Fid%252522%25253A%252522161261315016780261971438%252522%25252C%252522scm%252522%25253A%25252220140713.130102334.pc%25255Fall.%252522%25257D&request_id=161261315016780261971438&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_v2~rank_v29-2-57403414.pc_search_result_cache&utm_term=%25E7%25BA%25BF%25E7%25A8%258B%25E5%2588%25B0%25E5%25BA%2595%25E6%259C%2589%25E6%25B2%25A1%25E6%259C%2589%25E7%258B%25AC%25E7%25AB%258B%25E7%259A%2584%25E5%259C%25B0%25E5%259D%2580)

这节主要是讲线程函数的传参问题，加不加&引用这个符号的差别很大，涉及到是否调用拷贝构造函数这种很大的开销：

这节竟然还涉及到C++中的bind()和智能指针unique_ptr以及move constructor，在C++STLAnalysis这个仓库中，侯捷老师有讲到bind函数，我当时还觉得这种不会用到呢。

```c++
void process_big_object(std::unique_ptr<big_object>);

std::unique_ptr<big_object> p(new big_object);
p->prepare_data(42);
std::thread t(process_big_object,std::move(p));
```
通过在std::thread构造函数中执行std::move(p)，big_object 对象的所有权首先被转移到新创建线程的的内部存储中，之后再传递给process_big_object函数

## 2.3 转移所有权

