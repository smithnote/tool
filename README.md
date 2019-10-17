# tool
cpp的一些常用的工具

### 1. thread_pool 线程池工具

两种使用方法:
* 继承 tool::PoolTask 类用来实现自己的具体任务
* 实现 tool::PoolTask::run() 类实现任务执行入口

### 2. connection_pool 连接池工具

两种使用方法:
* 使用内置的两个连接类型 mysql, redis 定义模板类ConnnecctionPool
    ```
    auto pool = shd::make_shared<tool::ConnnectionPool<tool::MysqlConnection>>();
    ```
* 继承 tool::Connection 类用来实现自己的具体连接, 需要实现以下方法
    * tool::Connection::connect();  // 建立连接
    * tool::Connection::disconnect(); // 断开连接
    * tool::Connection::isAlive(); // 连接是否有效
