# tool

cpp的一些常用的工具

|tool|ok|
|----|----|
|[thread_pool 线程池工具](#thread_pool线程池工具)| :heavy_check_mark:|
|[connection_pool 连接池工具](#connection_pool连接池工具)|:heavy_check_mark:|
|[segment 中文一元二元切词工具](#segment中文一元二元切词工具)|:heavy_check_mark:|
|[hz2py 中文转拼音工具](#hz2py中文转拼音工具)|:heavy_check_mark:|
|[transform 文本转换工具](#transform文本转换工具)|:heavy_check_mark:|
|[inotify 文件监控工具](#inotify文件监控工具)|:heavy_check_mark:|

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
        tool::ConnectionPool<tool::RedisConnection> pool(8);
        std::string host("127.0.0.1");
        std::string port("6379");
        pool.initPool(host, port);
        std::shared_ptr<tool::RedisConnection> conn;
        pool.getConnection(conn);
        // conn to do something
        pool.returnConnection(conn);
        // run work to pool without get and return client
        book res = pool.run(&tool::RedisConnection::isAlive);
    }
    ```
2. 继承 tool::Connection 类用来实现自己的具体连接, 需要实现以下方法
    * tool::Connection::connect();  // 建立连接
    * tool::Connection::disconnect(); // 断开连接
    * tool::Connection::isAlive(); // 连接是否有效

    在初始化话词的时候，将Connection的构造参数加上

    ```
    #include "connection_pool.h"
    
    class UserConnection: public tool::Connection {
      public:
        UserConnection(const std::string arg){}
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
        
        inline std::string show(std::string &name) {
            return "hello " + name;
        }

      private:
        // some data
    };
    
    int main(int argc, char** argv) {
        std::string arg;
        tool::ConnectionPool<tool::UserConnection> pool;
        pool.initPool(arg);
        std::shared_ptr<tool::UserConnection> conn;
        pool.getConnection(conn);
        // conn to do something
        pool.returnConnection(conn);
        // without get and return clint
        assert(pool.run(&tool::UserConnection::show, "smith") == "hello smith");
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

### hz2py中文转拼音工具
利用硬编码直接将拼音数据写入代码中，使用例子
```
#include <iostream>
#include <vector>
#include "hz2py.h"

int main(int argc, char** argv) {
    tool::HZ2PY hz2py;
    std::string src_string = "我爱我的祖国";
    std::vector<std::string> py_vec, jp_vec;
    hz2py.toPinYin(src_string, py_vec);
    hz2py.toJianPin(src_string, jp_vec);
    for (auto &py: py_vec) {
        std::cout << py << std::endl;
    }
    for (auto &jp: jp_vec) {
        std::cout << jp << std::endl;
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
    tool::Base64 base64;
    base64.encode(src_string, encode_string);
    base64.decode(encode_string, decode_string), true);
    std::cout << "raw_string:" << src_string << std::endl;
    std::cout << "encode_string:" << encode_string << std::endl;
    std::cout << "decode_string:" << decode_string << std::endl;
    return 0;
}
```

### inotify文件监控工具
使用cpp对inotify的一个封装使用，独立线程处理回调函数, 例子:
```
int main() {
    FileWatcher file_watcher;
    file_watcher.isInit();
    const std::string path1{"/tmp/datachannel.txt"};
    file_watcher.add(path1, IN_DELETE_SELF|IN_DONT_FOLLOW,
                     std::make_shared<std::function<void()>>([&](){
                         ++triger_count;
                     }));
    return 0;
}
```
