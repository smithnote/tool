// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#ifndef TOOL_MYSQL_CONNECTION_H_
#define TOOL_MYSQL_CONNECTION_H_

#include <mysql/mysql.h>
#include "connection_pool.h"

namespace tool {

class MysqlConnection : public Connection {
  public:
    MysqlConnection(const std::string &host, const std::string &port,
                    const std::string &user, const std::string &password,
                    const std::string &db)
        : Connection(host, port, user, password, db), conn(NULL) {}
    ~MysqlConnection() {
        disconnect();
    }
    
    virtual bool connect();

    virtual bool disconnect();

    virtual bool isAlive();

  public:
    MYSQL *conn;

};

};

#endif 
