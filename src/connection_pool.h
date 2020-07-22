// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#ifndef TOOL_CONNECTION_POOL_H_
#define TOOL_CONNECTION_POOL_H_

#include <unistd.h>
#include <utility>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>


namespace tool {

class Connection {
  public:
    Connection() {}
    virtual ~Connection() {
        disconnect();
    }

    virtual bool connect() = 0;

    virtual bool isAlive() = 0;
    
    virtual bool disconnect() {
        return true;
    }
};


template<class Conn>
class ConnectionPool {
  public:
    ConnectionPool(size_t pool_size = 8, size_t keep_interval = 60) 
        : pool_size_(pool_size), keep_interval_(keep_interval),
          init_(false), running_(false) {}
    ~ConnectionPool() {
        if (running_) {
            running_ = false;
            exit_cond_.notify_all();
            keep_alive_thread_->join();
        }
    }
    
    template<typename... Args>
    bool initPool(Args&&... args) {
        if (init_) {
            return true;
        }
        for (size_t i = 0; i < pool_size_; ++i) {
            auto conn = std::make_shared<Conn>(std::forward<Args>(args)...);
            if (!conn->connect()) {
                continue;
            }
            conn_pool_.push(conn);
        }
        if (conn_pool_.empty()) {
            return false;
        }
        init_ = true;
        running_ = true;
        keep_alive_thread_ = std::make_shared<std::thread>(&ConnectionPool<Conn>::keepAlive,
                                                           this);
        return true;
    }

    bool getConnection(std::shared_ptr<Conn> &conn) {
        if (!init_) {
            return false;
        }
        std::unique_lock<std::mutex> locker(pool_mutex_);
        while (conn_pool_.empty() && running_) {
            pool_cond_.wait(locker);        
        }
        if (!running_) {
            conn = nullptr;
            return false;
        }
        conn = conn_pool_.front();
        conn_pool_.pop();
        return true;
    }

    bool returnConnection(std::shared_ptr<Conn> &conn) {
        if (!init_) {
            return false;
        }
        pool_mutex_.lock();
        conn_pool_.push(conn);
        pool_mutex_.unlock();
        pool_cond_.notify_one();
        return true;
    }

  private:
    bool keepAlive() {
        while (running_) {
            std::shared_ptr<Conn> connection;
            if (!getConnection(connection)) {
                continue;
            }
            if (connection && !connection->isAlive()) {
                connection->connect();
            }
            returnConnection(connection);
            if (!running_) {
                return true;
            }
            std::unique_lock<std::mutex> locker(exit_mutex_);
            auto now = std::chrono::system_clock::now();
            exit_cond_.wait_until(locker, now+std::chrono::seconds(keep_interval_));
        }
        return true;
    }

  private:
    size_t pool_size_;
    size_t keep_interval_;
    std::atomic<bool> init_;
    std::atomic<bool> running_;
    std::mutex pool_mutex_;
    std::mutex exit_mutex_;
    std::condition_variable pool_cond_;
    std::condition_variable exit_cond_;
    std::queue<std::shared_ptr<Conn>> conn_pool_;
    std::shared_ptr<std::thread> keep_alive_thread_;
};

};

#endif 
