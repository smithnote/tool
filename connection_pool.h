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
    Connection(const std::string &host, const std::string &port,
               const std::string &user, const std::string &password,
               const std::string &db = "")
        :host_(host), port_(port), user_(user), password_(password), db_(db) {}
    ~Connection() {}

    virtual bool connect() = 0;

    virtual bool disconnect() = 0;

    virtual bool isAlive() = 0;
  protected:
    std::string host_;
    std::string port_;
    std::string user_;
    std::string password_;
    std::string db_;
};


template<class Conn>
class ConnectionPool {
  public:
    ConnectionPool() : init_(false), running_(false), pool_size_(8) {}
    ConnectionPool(const std::string &host, const std::string &port,
                   const std::string &user, const std::string &password,
                   const std::string &db = "", const bool init = false,
                   const bool running = false, const size_t pool_size = 8)
        : host_(host), port_(port), user_(user), password_(password), db_(db),
          init_(init), running_(running), pool_size_(pool_size) {}
    ~ConnectionPool() {
        running_ = false;
        keep_alive_thread_->join();
    }
    
    bool initPool();

    bool getConnection(std::shared_ptr<Conn> &conn);

    bool returnConnection(std::shared_ptr<Conn> &conn);

    inline bool setHost(const std::string &host) {
        host_ = host;
        return true;
    }
    inline bool setPort(const std::string &port) {
        port_ = port;
        return true;
    }
    inline bool setUser(const std::string &user) {
        user_ = user;
        return true;
    }
    inline bool setPassword(const std::string &password) {
        password_ = password;
        return true;
    }

  private:
    bool keepAlive();

  private:
    std::string host_;
    std::string port_;
    std::string user_;
    std::string password_;
    std::string db_;

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
