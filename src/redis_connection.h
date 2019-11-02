// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#ifndef TOOL_REDIS_CONNECTION_H_
#define TOOL_REDIS_CONNECTION_H_

#include <hiredis/hiredis.h>
#include "connection_pool.h"

namespace tool {

class RedisConnection: public Connection {
  public:
    RedisConnection(const std::string &host, const std::string &port,
                    const std::string &user, const std::string &password,
                    const std::string &db)
        : Connection(host, port, user, password, db) {}
    ~RedisConnection() {}
    
    virtual bool connect();

    virtual bool disconnect();

    virtual bool isAlive();

  public:
    redisContext *redis;
};


}

#endif 
