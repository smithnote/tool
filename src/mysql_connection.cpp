// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#include "mysql_connection.h"


namespace tool {

bool MysqlConnection::connect() {
    conn = mysql_init(NULL);
    if (!conn) {
        return false;
    }
    if (mysql_real_connect(conn, host_.c_str(), user_.c_str(), password_.c_str(),
                db_.c_str(), atoi(port_.c_str()), NULL, 0) == NULL) {
        std::cerr << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

bool MysqlConnection::disconnect() {
    mysql_close(conn);
    return true;
}

bool MysqlConnection::isAlive() {
    return mysql_ping(conn) == 0;
}

};
