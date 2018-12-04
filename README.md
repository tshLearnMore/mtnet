# mtnet
mtnet：my Tcp Network library

较早之前实现了一个基于reactor模式的网络库，但是管理的比较乱，现在将它进行整改移植到git上来进行管理。移植的过程经量不CTR+C，再熟悉一遍网络库整体流程。

特性：

1. 个人的网络库，现在只支持在Linux上运行

2. 不支持udp，只支持tcp

3. 网络库的基本模式是：非阻塞IO + 每个事件循环(loop)对应一个线程


线程模型：

1.单线程：accept + event处理在同一个线程中

2.多线程：accept + event处理在不同线程中，其中event处理线程可以有不只一个；accept针对新连接会按照循环分配的方式指定event线程，以后可以给event线程指定优先级，按照优先级来分配。

说明记录：
#include <functional>		function和bind
#include <memory>			shared_ptr