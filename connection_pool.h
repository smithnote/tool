// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#ifndef TOOL_CONNECTION_POOL_H_
#define TOOL_CONNECTION_POOL_H_

#include <unistd.h>
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace tool {

template<class Conn>
class ConnectionPool;

class Connection {
  public:
    Connection() {}
    ~Connection() {}

    virtual bool connect() = 0;

    virtual bool disconnect() = 0;

    virtual bool isAlive() = 0;
};


template<class Conn>
class ConnectionPool {
  public:
    ConnectionPool() : init_(false), running_(false), pool_size_(8) {}
    ~ConnectionPool() {
        running_ = false;
        keep_alive_thread_->join();
    }
    
    bool initPool();

    bool getConnection(std::shared_ptr<Conn> &conn);

    bool returnConnection(std::shared_ptr<Conn> &conn);

  private:
    bool keepAlive();

  private:
    bool init_;
    bool running_;
    int keep_interval_;
    size_t pool_size_;
    std::mutex pool_mutex_;
    std::condition_variable pool_cond_;
    std::queue<std::shared_ptr<Conn>> conn_pool_;
    std::shared_ptr<std::thread> keep_alive_thread_;
};

};

#endif 
