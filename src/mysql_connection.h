// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#ifndef TOOL_MYSQL_CONNECTION_H_
#define TOOL_MYSQL_CONNECTION_H_

#include <mysql/mysql.h>
#include <iostream>
#include "connection_pool.h"

namespace tool {

class MysqlConnection : public Connection {
  public:
    MysqlConnection(const std::string &host, const std::string &port,
                    const std::string &user, const std::string &password,
                    const std::string &db = "")
        : conn(NULL), host_(host), port_(port), user_(user), password_(password), db_(db) {}
    virtual ~MysqlConnection() {
        disconnect();
    }
    
    virtual bool connect(const int timeout = 30) override;

    virtual bool disconnect() override;

    virtual bool isAlive() override;

  public:
    MYSQL *conn;
    
    std::string host_;
    std::string port_;
    std::string user_;
    std::string password_;
    std::string db_;
};

};

#endif 
