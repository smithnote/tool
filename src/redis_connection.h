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
                    const std::string &user = "", const std::string &password = "",
                    const std::string &db = "0")
        : redis(NULL), host_(host), port_(port),
          user_(user), password_(password), db_(db) {}
    ~RedisConnection() {}
    
    virtual bool connect() override;

    virtual bool disconnect() override;

    virtual bool isAlive() override;

  public:
    redisContext *redis;
    
    std::string host_;
    std::string port_;
    std::string user_;
    std::string password_;
    std::string db_;
};


}

#endif 
