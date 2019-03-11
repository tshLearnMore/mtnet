# mtnet
mtnet：my Tcp Network library

较早之前实现了一个基于reactor模式的网络库，但是管理的比较乱，现在将它进行整改移植到git上来进行管理。再熟悉一遍网络库整体流程。

特性：

1. 个人的网络库，现在只支持在Linux上运行

2. 不支持udp，只支持tcp

3. 网络库的基本模式是：one (event) loop per thread + thread pool
   - event loop（即IO loop）用作IO复用，配合非阻塞IO和定时器 [one (event) loop per thread]
   - thread pool用来做计算，具体可以是任务队列或者生产者消费者队列 [thread pool]
   - 可能还有个别特殊任务的线程，例如logging(日志)线程

线程模型：

1.单线程：accept + event处理在同一个线程中

2.多线程：accept + event处理在不同线程中，其中event处理线程可以有不只一个；accept针对新连接会按照循环分配的方式指定event线程，以后可以给event线程指定优先级，按照优先级来分配。

头文件说明记录：

```
#include <functional>		function和bind
#include <memory>			shared_ptr
```

#TODO:
##base
|名称|完成情况|内容|
| :------ | :------ | :------|
|01.Atomic         |    Done | 原子操作 |
|02.thread         |    Done | 线程操作封装 |
|03.Uncopyable     |    Done | 类不可使用拷贝构造函数和赋值构造函数 |
|04.Mutex          |    Done | 互斥访问控制 |
|05.Condition      |    Done | 条件变量封装 |
|06.Logging        |    Done | 日志操作封装（暂时比较简单后期要单独起线程+2个缓冲处理） |
|07.Timestamp      |    Done | 时间戳操作封装 |

##mtnet
|名称|完成情况|内容|
| :------ | :------ | :------|
|01.EventLoop           |  Done   | 主事件循环 |
|02.Channel             |  Done   | fd通道管理 |
|03.Poller              |  Done   | ePoll封装 |
|04.Timer               |  Done   | 单独启动线程运行EventLoop |
|05.EventLoopThread     |  Done   | server fd操作封装 |
|06.Acceptor            |  Done   | server fd操作封装 |
|07.InetAddress         |  Done   | 网络地址操作封装 |
|08.Socket              |  Done   | socket操作封装 |
|09.TcpConnect          |  Done   | connection过程封装 |
|10.TCPServer           |  Done   | server fd封装 |
|11.TCPClient           |  Done   | client fd封装 |
|12.Buffer              |  Done   | 应用层Buffer |
|13.EventLoopThreadPool |Done | 多线程EventLoop封装 |


##测试
1.只完成功能性测试
2.尚未完成压力测试（多连接10K）


-----------


