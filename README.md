# tinyrpc
C++ rpc framework, simplified version of tinyrpc

### 2.2 配置环境
#### 2.2.1 protobuf
```
1.去github上下载：https://github.com/protocolbuffers/protobuf/releases/tag/v3.19.4
2. 再把下载的东西移动到package文件夹下
3. 到package文件夹下的bash中执行 tar -xzvf protubuf-cpp-3.19.4 tar.gz
4.cd  protubuf-cpp-3.19.4
5../configure -prefix=/usr/local
6.sudo make install
注意：头文件在 /usr/local/include/google, 库文件位于 cd /usr/local/lib。

```
#### 2.2.2 tinyxml

```
1.在Linux上去官网：https://sourceforge.net/projects/tinyxml/下载
2. tar -xzvf tinyxml_2_6_2.tar.gz
```
### 2.3 日志模块开发
首先需要创建项目

日志模块：
```
1. 日志级别
2. 打印到文件， 支持日期命名，以及日志的滚动
3. 日志风格：C
4. 线程安全

```

LogLevel:
```
Debug
Info
Error
```

LogEvent:
```
文件名、行号
MsgNo
Process id
Thread id
日期，时间（精确到毫秒）
自定义消息
```


日志格式
```
[%y-%m-%d %h:%m:%s.%ms]\t[pid:thread_id]\t[file_name:line][%msg]

```

Logger 日志器
1.提供打印日志的方法
2.设置日志输出的路径


### 2.4 Reactor
Reactor, 又称为EventLoop, 它本质是一个时间循环模型。


在tinyrpc里面，使用的是主从Reactor模型，如下图所示
![](.imgs/mian-sub-reactor.drawio.png)

服务器有一个mainReactor和多个subReactor
mainReactor由主线程完成，他作用如下：通过epoll监听listenfd的可读事件，调用accept函数获取clientfd，然后随机取出一个subReactor，将clientfd的读写事件注册到这个subReactor的epoll上即可。也就是说，mainReactor只负责建立连接事件，不进行业务处理，也不关系已连接套接字的IO事件.

subReactor通常有多个，每个subReactor由一个线程来运行，subReactor的epoll中注册了clientfd的读写事件，当发生IO事件后，需要进行业务处理


#### 2.4.1 TimerEvet定时任务
'''

1.指定时间点arrive_time
2.interval, ms
3.is_repeated
4.is_cancled
5.task

cancle()

cancleRepeated()

'''

#### 2.4.2 Timer
定时器，他是一个TimerEvent的集合
Timer 继承FDEvent

```
addTimerEvent();
deleteTimerEvent();
onTimer();    //当发生IO事件后，需要执行的方法

reserArriveTime();

multimap 存储TimerEvent <key(arriveime),TimerEvent>
```


#### 2.5 IO线程
创建一个IO线程，他会帮我们执行
1.创建一个新线程(pthread_create)
2.在新线程里面创建一个Eventloop,完成初始化
3.开启loop
'''
class {
    pthread_t m_thread;
    pid_t m_thread_id;
    Eventloop event_loop;
}
'''




