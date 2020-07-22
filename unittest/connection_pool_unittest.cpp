// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#include <iostream>
#include <memory>
#include <gtest/gtest.h>

#include "connection_pool.h"
#include "redis_connection.h"
#include "mysql_connection.h"


class ConnectionPoolTest: public testing::Test {
  public:
    virtual void SetUp() {}
    virtual void TearDown() {}

};


TEST_F(ConnectionPoolTest, redis_pool_test) {
    using namespace tool;
    std::string host("localhost");
    std::string port("6379");
    auto pool = std::make_shared<ConnectionPool<RedisConnection>>();
    ASSERT_EQ(pool->initPool(host, port), true);
    std::shared_ptr<RedisConnection> conn;
    ASSERT_EQ(pool->getConnection(conn), true);
    pool->returnConnection(conn);
}

TEST_F(ConnectionPoolTest, mysql_pool_test) {
    using namespace tool;
    size_t poolsize(10);
    size_t keepinterval(15);
    std::string host("127.0.0.1");
    std::string port("3306");
    std::string user("root");
    std::string password("smithgo");
    auto pool = std::make_shared<ConnectionPool<MysqlConnection>>();
    ASSERT_EQ(pool->initPool(host, port, user, password), true);
    std::shared_ptr<MysqlConnection> conn;
    ASSERT_EQ(pool->getConnection(conn), true);
    ASSERT_NE(conn, nullptr);
    ASSERT_EQ(conn->conn != NULL, true);
    std::string sql = "show databases;";
    ASSERT_EQ(mysql_query(conn->conn, sql.c_str()), 0);
    MYSQL_RES *res = mysql_use_result(conn->conn);
    MYSQL_ROW row;
    while (row = mysql_fetch_row(res)) {
        const std::string &db = row[0];
    }
    mysql_free_result(res);
    pool->returnConnection(conn);
};

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
