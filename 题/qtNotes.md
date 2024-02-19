# Qt题
### 11、Qt如果一个信号的处理方法一直未被执行，有哪些可能性？ 
断开了，连接的时候失败了，多线程的时候再排队或者启动了锁死。
### 12、在Qt5的信号处理中，如何使用lambda机制？
信号定义了，但是不写对应的槽函数，直接将函数写到槽的位置。
```C++
connect(musicPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(slotReflushStartTime(qint64)));
connect(musicPlayer,SIGNAL(positionChanged(qint64)),SLOT(slotReflushStartTime(qint64)));
```
### 13、段错误一般是什么原因造成的，如何快速排查？
一般是指针的问题，出现野指针空指针；用点灯或者Debug去排查问题。
### 14、Qt三大核心机制：信号槽、元对象系统、事件模型
#### 信号槽
信号槽有五种连接方式；(参考第17题)
connect(信号发出者，信号，信号接收者，槽，连接方式(隐藏默认自动连接));

#### 元对象系统
元对象系统分为三大类：QObject类、Q_OBJECT宏和元对象编译器moc;
Qt的类包含Q_OBJECT宏，moc编译器会对该类编译成标准的C++代码。

#### 事件模型
##### 事件的创建
鼠标事件、键盘事件、窗口调整事件、模拟事件

##### 事件的交付
Qt通过调用虚函数QObject::event()来交付事件。

##### 事件循环模型
主事件循环通过调用QCoreApplicat::exec()启动，随着QCoreApplication::exit()结束，本地的事件循环可利用QEventLoop构建。
一般来说，事件是由出发当前的窗口系统产生的，但也可以通过使用QCoreApplication::sendEvent()和QCoreApplication::postEvent()来手工产生事件。需要说明的是QCoreApplication::sendEvent()会立即发送事件，QCoreApplication::postEvent()则会将事件放在事件队列分发。

#### 自定义事件

### 15、Qt对象树
QT提供了对象树机制，能够自动、有效的组织和管理继承自QObject的对象。
每个继承自QObject类的对象通过链表(QObjectList)来管理子类对象，当用户创建一个字对象时，其对象链表相应更新子类对象的信息，对象链表可通过children()获取。
当每个父类对象析构的时候，其对象链表中的所有(子类)对象也会被析构，父对象会自动，将其从父对象列表中删除，QT保证没有对象会被delete两次。开发中手动回收资源时建议使用deleteLater代替delete，因为deleteLater多次时安全的。

### 16、描述QTextSTream(文件流)和QDataStram(数据流)的区别
文本流用来操作轻量级的数据，比如内置的int、QString等，写入文件以后文本的方式呈现数据流，可以操作各种类型数据，总之，两者都可以进行操作磁盘文件以及内存数据。

### 17、信号槽的四种写法和五种连接方式？
connect(信号发出者,信号,信号接收者,槽,连接方式(隐藏默认自动连接)) // 五个参数
四种写法：
1. 用宏：
   ```C++
   connect(this,SIGNAL(clicked()),this,SLOT(close()));  // 连接方式(隐藏默认自动连接)
   ```
2. 用函数指针
   ```C++
   connect(this,&mainwindow::my_signal,this,&mainwindow::my_slot);
   ```
3. 用重载函数指针QOverload
   ```C++
   connect(this,Qoverload<参数>::of(&mainwindow::my_signal),this,Qoverload<参数>::of(&mainwindow::my_slot));
   ```
4. lambda表达式(匿名函数)匿名函数代替槽
   ```C++
   connect(this,&mainwindow::my_signal,this,[=]{qDebug()<<100;});
   ```

连接方式：
- 自动连接(默认连接方式)
- 直接连接(用于单线程，自动匹配)
- 队列（用于多线程也可以用于单线程，自动匹配)
- 阻塞队列(跨线程，多线程)
- 唯一连接(跨线程，多线程)

### 18、Qt模型
Qt的View主要有三种QListView, QTreeView, QTabelView
而对应的Model是：QStringListModel, QAbstractltemModel, QStandarItemModel.
抽象、标准

### 19、Qt中的MVD了解吧？
Qt的MVD包含三个部分Model(模型)，View(视图)，代理(Delegate)。Model否则保存数据，View负责展示数据，Delegate负责Item样式绘制或处理输入。这三部分通过信号来进行通信，当Model中数据发生变化时，将会发送信号到View，在View中编辑数据时，Delegate负责将编辑状态发送给Model层。基类分别为QAbstracItemModel、QAbstractItemView、QAbstractItemDelegate。Qt中提供了默认实现的MVD类，如QTableWidget、QListWidget、QTreeWidget等。

### 20、Qt如果要进行网络编程首先需要在.pro中添加如下代码QT network
1. 在头文件中包含相关头文件：
```C++
#include <QHostInfo>
#include <QNetworkInterface>
```
2. QT的UdpSocket接收消息使用原则
- 第一步：new一个UdpSocket
- 第二步：调用UdpSocket的bind方法，同时指定端口号
- 第三步：使用connect将接收消息函数和UdpSocket对象做关联
- 第四步：在接收消息槽函数当中调用readDatagram接收消息