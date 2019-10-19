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
    auto pool = std::make_shared<ConnectionPool<RedisConnection>>();
    pool->setHost("localhost");
    pool->setPort("6379");
    ASSERT_EQ(pool->initPool(), true);
    std::shared_ptr<RedisConnection> conn;
    ASSERT_EQ(pool->getConnection(conn), true);
    pool->returnConnection(conn);
}

TEST_F(ConnectionPoolTest, mysql_pool_test) {
    using namespace tool;
    auto pool = std::make_shared<ConnectionPool<MysqlConnection>>();
    pool->setHost("127.0.0.1");
    pool->setPort("3306");
    pool->setPort("root");
    pool->setPassword("smithgo");
    pool->setPoolSize(10);
    pool->setKeepInterval(15);
    ASSERT_EQ(pool->initPool(), true);
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
