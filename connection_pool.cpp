// Copyright (c) 2019 360.cn. All rights reserved.
// Author：shichanghui@360.cn
// Time：2019-10-16

#include "connection_pool.h"


namespace tool {

template<class Conn>
bool ConnectionPool<Conn>::initPool() {
    if (init_) {
        return true;
    }
    for (size_t i = 0; i < pool_size_; ++i) {
        conn_pool_.push(std::make_shared<Connection>(Conn()));
    }
    init_ = true;
    keep_alive_thread_ = std::make_shared<std::thread>(&ConnectionPool<Conn>::keepAlive,
                                                       this);
    return true;
}

template<class Conn>
bool ConnectionPool<Conn>::getConnection(std::shared_ptr<Conn> &conn) {
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

template<class Conn>
bool ConnectionPool<Conn>::returnConnection(std::shared_ptr<Conn> &conn) {
    if (!init_) {
        return false;
    }
    pool_mutex_.lock();
    conn_pool_.push(conn);
    pool_mutex_.unlock();
    pool_cond_.notify_one();
    return true;
}

template<class Conn>
bool ConnectionPool<Conn>::keepAlive() {
    while (running_) {
        std::shared_ptr<Conn> connection;
        if (!getConnection(connection)) {
            continue;
        }
        if (connection && !connection->isAlive()) {
            connection->connect;
        }
        returnConnection(connection);
        // TODO: 释放的时候会等待，后期加notify
        sleep(keep_interval_);
    }
    return true;
}


};
