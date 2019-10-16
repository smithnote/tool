// Copyright (c) 2019 360.cn. All rights reserved.
// Author：shichanghui@360.cn
// Time：2019-10-15

#include "thread_pool.h"

namespace tool {

bool PoolTask::run() {
    std::cerr << "thread " << std::this_thread::get_id() << " do work" << std::endl;
    return true;
}


bool ThreadPool::start(const size_t pool_size) {
    runing_ = true;
    for (size_t i = 0; i < pool_size; ++i) {
        auto worker = std::make_shared<std::thread>(
                std::thread(&ThreadPool::threadWorker, this));
        thread_pool_.push_back(worker);
    }
    return true;
}

bool ThreadPool::stop() {
    task_queue_mutex_.lock();
    runing_ = false;
    task_queue_mutex_.unlock();
    task_queue_cond_.notify_all();
    for (auto &woker: thread_pool_) {
        woker->join();
    }
    return true;
}

bool ThreadPool::pushTask(const std::shared_ptr<PoolTask> &task) {
    if (!runing_) {
        return false;
    }
    task_queue_mutex_.lock();
    task_queue_.push_back(task);
    task_queue_mutex_.unlock();
    task_queue_cond_.notify_one();
    return true;
}

bool ThreadPool::threadWorker() {
    while (runing_) {
        std::shared_ptr<PoolTask> task;
        if (!getTask(task) || task == nullptr) {
            continue;
        }
        task->run();
    }
    return true;
}

bool ThreadPool::getTask(std::shared_ptr<PoolTask> &task) {
    std::unique_lock<std::mutex> locker(task_queue_mutex_);
    while (task_queue_.empty() && runing_) {
        task_queue_cond_.wait(locker);
    }
    if (runing_) {
        task = task_queue_.front();
        task_queue_.pop_back();
    } else {
        task = nullptr;
    }
    task_queue_mutex_.unlock();
    return true;
}
}
