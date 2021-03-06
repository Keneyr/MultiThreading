# MultiThreading

C++11多线程编程，读这本[C++多线程](https://github.com/xiaoweiChen/CPP-Concurrency-In-Action-2ed-2019/blob/master/content/chapter1/1.4-chinese.md)

看这个[视频](https://www.bilibili.com/video/BV1xt411r7U2?from=search&seid=10832949688649776573)

同时巩固[C++11的一些新特性](http://c.biancheng.net/view/7909.html) 这个网站很好，可惜高级教程收费

虽然过了一遍书和代码，稍有一定的熟悉，但离熟练还差的远

希望自己将来工作还会有时间再过一遍

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

好难好难好难...并发编程咋这么难...

```c++
thread t1(some_function);
thread t2 = move(t1);
t1 = thread(some_other_function);
thread t3;
t3 = move(t2);
//t1 = move(t3);
```

要注意move()这个转移所有权函数，已经被用到飞起了..

量产线程，等待它们结束：

```c++
void do_work(unsigned id);

void f()
{
  std::vector<std::thread> threads;
  for (unsigned i = 0; i < 20; ++i)
  {
    threads.emplace_back(do_work,i); // 产生线程
  } 
  for (auto& entry : threads) // 对每个线程调用 join()
    entry.join();       
}
```

我们有时需要线程去分割一个算法的工作总量，所以在算法结束的之前，所有的线程必须结束。代码2.8中线程所做的工作都是独立的，并且结果仅会受到共享数据的影响。如果f()有返回值，这个返回值就依赖于线程得到的结果。写入返回值之前，程序会检查使用共享数据的线程是否终止。结果在不同线程中转移的方案，会在第4章中再次讨论。

将std::thread放入std::vector是向线程自动化管理迈出的第一步：并非为这些线程创建独立的变量，而是把它们当做一个组。创建一组线程(数量在运行时确定)，而非代码2.8那样创建固定数量的线程。

## 2.4 确定线程的数量

代码将整体工作拆分成小任务，交给每个线程去做，并设置最小任务数，避免产生太多的线程。

## 2.5 线程标识 没啥好说的

## 3.1 共享数据问题

线程之间，错误的数据共享是产生bug的主要原因

并行中常见错误：条件竞争(race condition)：拿双向链表删除其中一个结点举例的条件竞争

避免恶性条件竞争的方法有很多：

1、最简单的是对数据结构采用某种保护机制，确保只有修改线程才能看到不变量的中间状态

2、无锁编程：对数据结构和不变量进行修改，修改完的结构必须能完成一系列不可分割的变化，也就保证了每个不变量的状态

3、以事务的方式去处理数据结构，所需的一些数据和读取都存储在事务日志中，然后将之前的操作进行合并，再进行提交。当数据结构被另外一个线程修改后，或处理已经重启的情况下，提交就会无法进行。

说了这么多，保护共享数据结构最基本的方式，就是使用C++标准库提供的互斥量。

## 3.2 使用互斥量

这章比较重要(在操作系统中，一般也会多次提到互斥量的整套使用流程)

互斥量是C++保护数据最通用的机制，但也需要编排代码来保护数据的正确性(见3.2.2节)，并避免接口间的条件竞争(见3.2.3节)也非常重要。不过，互斥量也会造成死锁(见3.2.4节)，或对数据保护的太多(或太少)(见3.2.8节)。

### 3.2.1 互斥量

这里涉及到C++ RAII机制，资源获取初始化，可以理解为一种垃圾回收机制，主要是为了更好的回收系统资源：比如说多进程的互斥量、套接字、内存等。

通过mutex对共享数据上锁。但是有时候也不那么理想：

当其中一个成员函数返回的是保护数据的指针或引用时，也会破坏数据。具有访问能力的指针或引用可以访问(并可能修改)保护数据，而不会被互斥锁限制。这就需要对接口谨慎设计，要确保互斥量能锁住数据访问，并且不留后门。

## 3.2.2 保护共享数据

通过互斥量来保护数据，并不是在每一个成员函数中加入一个std::lock_guard对象那么简单。一个指针或引用，也会让这种保护形同虚设。

这也是编程中程序员常犯的错误~

## 3.2.3 接口间的条件竞争

双向链表删除一个结点，为了线程安全，会用一个mutex保护整个list，尽管链表的个别操作是安全的，但依旧可能遇到条件竞争。

例如，构建一个类似于std::stack的栈(代码3.3)，除了构造函数和swap()以外，需要对std::stack提供五个操作：push()一个新元素进栈，pop()一个元素出栈，top()查看栈顶元素，empty()判断栈是否是空栈，size()了解栈中有多少个元素。即使修改了top()，返回一个拷贝而非引用(即遵循了3.2.2节的准则)，这个接口仍存在条件竞争。这个问题不仅存在于互斥量实现接口中，在无锁实现接口中，也会产生条件竞争。这是接口的问题，与实现方式无关。


## 插曲

写了一个Master-Worker设计模式的简单demo，但是目前还有错，并且感觉自己写多线程写的很糟糕...等有经验了再来修改..

又写了一个Producer-Consumer设计模式的简单demo~依然有错ing， 淦，多线程好难...

虽然如此，仅仅学到第3章，我就好像找到了  看懂别人写的多线程C++项目代码 的光缝啦。可以顺藤摸瓜，慢慢啃啦













































