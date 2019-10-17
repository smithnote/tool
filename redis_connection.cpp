// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-16

#include "redis_connection.h"

namespace tool {

bool RedisConnection::connect() {
    struct timeval timeout = { 1, 500000};  // 1.5s
    redis = redisConnectWithTimeout(host_.c_str(), atoi(port_.c_str()), timeout);
    if (redis == NULL || redis->err) {
        return false;
    }
    return true;
}

bool RedisConnection::disconnect() {
    if (redis) {
        redisFree(redis);
    }
    return true;
}

bool RedisConnection::isAlive() {
    if (redis) {
        redisReply *reply  = (redisReply*)redisCommand(redis, "ping a");
        freeReplyObject(reply);
        return reply != NULL;
    }
    return false;
}

}
