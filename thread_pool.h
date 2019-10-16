// Copyright (c) 2019 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2019-10-15

#ifndef TOOL_THREAD_POOL_H_
#define TOOL_THREAD_POOL_H_

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace tool {

class PoolTask {
  public:
    PoolTask() {}
    ~PoolTask() {}

    virtual bool run();
};


class ThreadPool {
  public:
    ThreadPool() {}
    ~ThreadPool() {}

    bool start(const size_t pool_size);

    bool stop();

    bool pushTask(const std::shared_ptr<PoolTask> &task);

  private:
    bool threadWorker();

    bool getTask(std::shared_ptr<PoolTask> &task);

  private:
    bool init_;
    bool runing_;
    std::mutex task_queue_mutex_;
    std::condition_variable task_queue_cond_;
    std::vector<std::shared_ptr<std::thread>> thread_pool_;
    std::vector<std::shared_ptr<PoolTask>> task_queue_;
};

};
#endif 
