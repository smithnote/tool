# tool

cpp的一些常用的工具

|tool|ok|
|----|----|
|[thread_pool 线程池工具](#thread_pool线程池工具)| :heavy_check_mark:|
|[connection_pool 连接池工具](#connection_pool连接池工具)|:heavy_check_mark:|
|[segment 中文一元二元切词工具](#segment中文一元二元切词工具)|:heavy_check_mark:|
|[transform 编码解码工具](#transform文本转换工具)|:heavy_check_mark:|

### thread_pool 线程池工具

使用方法:
* 继承 tool::PoolTask 类用来实现自己的具体任务
* 实现 tool::PoolTask::run() 类实现任务执行入口

```
#include "thread_pool.h"


// 自定任务类型
class Task: public tool::PoolTask {
  public:
    Task(int a, int b): a_(a), b_(b) {}

    virtual bool run() {
        int sum = a_ + b_;
        std::cerr << sum << std::endl;
        return true;
    }

  private:
    // 任务内部保存相关数据
    int a_;
    int b_;
};


int main(int argc, char** argv) {
    tool::ThreadPool pool;
    pool.start(10);
    for (size_t i = 0; i < 100; ++i) {
        pool.pushTask(std::make_shared<Task>(i,i+1));
    }
    pool.stop();
}

```

### connection_pool连接池工具

两种使用方法:
1. 使用内置的两个连接类型 mysql, redis 定义模板类ConnnecctionPool
    ```
    //以redis连接为例
    #include "connection_pool.h"
    #include "redis_connection.h"
    
    int main(int argc, char** argv) {
        tool::ConnectionPool<tool::RedisConnection> pool;
        pool.setHost("127.0.0.1");
        pool.setPort("6379");
        pool.initPool();
        std::shared_ptr<tool::RedisConnection> conn;
        pool.getConnection(conn);
        // conn to do something
        pool.returnConnection(conn);
    }
    ```
2. 继承 tool::Connection 类用来实现自己的具体连接, 需要实现以下方法
    * tool::Connection::connect();  // 建立连接
    * tool::Connection::disconnect(); // 断开连接
    * tool::Connection::isAlive(); // 连接是否有效

    ```
    #include "connection_pool.h"
    
    class UserConnection: public tool::Connection {
      public:
        UserConnection(const std::string &host, const std::string &port,
                         const std::string &user, const std::string &password,
                         const std::string &db)
            : tool::Connection(host, port, user, password, db) {}
        ~UserConnection() {}
        
        virtual bool connect() {
            // do something to connect
            return true;
        }
        virtual bool disconnect() {
            // do something to disconnect
            return true;
        }
        virtual bool isAlive() {
            // do something to jeduge conn is alive 
        }
    };
    
    int main(int argc, char** argv) {
        tool::ConnectionPool<tool::UserConnection> pool;
        pool.setHost("127.0.0.1");
        pool.setPort("6379");
        pool.initPool();
        std::shared_ptr<tool::UserConnection> conn;
        pool.getConnection(conn);
        // conn to do something
        pool.returnConnection(conn);
    }
    ```

### segment中文一元二元切词工具

使用方法
```
#include "segment.h"

int main(int argc, char** argv) {
    tool::Segment segment;
    std::string sentence = "这是一个美好的世界";
    std::vector<std::string> unigram_vec, bigram_vec;
    segment.segUnigram(cn_sentence, unigram_vec);// 一元切词
    segment.segBigram(cn_sentence, bigram_vec);// 二元切词
    for (auto &w: unigram_vec) {
        std::cout << w << std::endl;
    }
    for (auto &w: bigram_vec) {
        std::cout << w << std::endl;
    }
    return 0;
}

```

### transform文本转换工具

* [base64编码工具](#base64编码工具)

#### base64编码工具

使用方法:直接调用类的两个方法，encode, decode

```
#include <iostream>
#include "transform.h"

int main(int argc, char** argv) {
    std::string src_string = "我爱我的祖国, I love my homeland";
    std::string encode_string, decode_string;
    base64.encode(src_string, encode_string);
    base64.decode(encode_string, decode_string), true);
    std::cout << "raw_string:" << src_string << std::endl;
    std::cout << "encode_string:" << encode_string << std::endl;
    std::cout << "decode_string:" << decode_string << std::endl;
    return 0;
}
```
