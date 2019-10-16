// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#include "mysql_connection.h"


namespace tool {

bool MysqlConnection::connect() {
    mysql_init(conn);
    if (!conn) {
        return false;
    }
    mysql_real_connect(conn, host_.c_str(), user_.c_str(), password_.c_str(),
                       db_.c_str(), atoi(port_.c_str()), NULL, 0);
    return true;
}

bool MysqlConnection::disconnect() {
    mysql_close(conn);
    return true;
}

bool MysqlConnection::isAlive() {
    return true;
}

};
