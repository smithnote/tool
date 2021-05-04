// Copyright (c) 2021 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2021-03-12
// thread-safe inotify cpp warpper

#ifndef TOOL_INOTIFY_H_
#define TOOL_INOTIFY_H_

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <iostream>
#include <map>
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <memory>
#include <functional>

namespace tool {

class FileWatcher {
  public:
    FileWatcher() : init_(false) {}
    FileWatcher(const FileWatcher&) = delete;
    FileWatcher& operator=(const FileWatcher&) = delete;
    ~FileWatcher() {
        if (init_.load(std::memory_order_acquire)) {
            init_ = false;
            write(stop_pipe_[1], "a", 1);
            watch_thread_->join();
            close(stop_pipe_[0]);
            close(stop_pipe_[1]);
            close(epoll_fd_);
            close(notify_fd_);
        }
    }

    bool start();
    
    bool add(const std::string &fpath, uint32_t masks,
             std::shared_ptr<std::function<void()>> func);

    bool remove(const std::string &fpath);

    inline size_t count() const {
        std::lock_guard<std::recursive_mutex> locker(recursive_mtx_);
        return path2func_map_.size();
    }

    inline bool isInit() const {
        return init_;
    }

    inline bool isWatched(const std::string &fpath) const {
        std::lock_guard<std::recursive_mutex> locker(recursive_mtx_);
        return path2func_map_.count(fpath);
    }

  private:
    int epoll_fd_;
    int stop_pipe_[2];
    int notify_fd_;
    std::atomic<bool> init_;
    std::map<int, std::string> fd2path_map_;
    std::map<std::string, int> path2fd_map_;
    std::map<std::string, std::shared_ptr<std::function<void()>>> path2func_map_;
    mutable std::recursive_mutex recursive_mtx_;
    std::shared_ptr<std::thread> watch_thread_;
};

};

#endif
