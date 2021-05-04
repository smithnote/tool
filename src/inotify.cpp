// Copyright (c) 2021 smithemail@163.com. All rights reserved.
// Author：smithemail@163.com
// Time：2021-03-19

#include "inotify.h"

namespace tool {

bool FileWatcher::start() {
    epoll_fd_ = epoll_create(1);
    notify_fd_ = inotify_init();
    int res = pipe(stop_pipe_);
    if (epoll_fd_ == -1 || notify_fd_ == -1 || res == -1) {
        std::cerr << "epoll or inotify or pipe error:" << strerror(errno);
        return false;
    }
    auto add_epoll = [&](int fd) {
        struct epoll_event pevent;
        pevent.data.fd = fd;
        pevent.events = EPOLLIN | EPOLLET;
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &pevent);
    };
    add_epoll(notify_fd_);
    add_epoll(stop_pipe_[0]);
    init_.store(true, std::memory_order_release);
    watch_thread_ = std::make_shared<std::thread>(([&]() {
        constexpr uint32_t UNDEAL = IN_IGNORED|IN_UNMOUNT|IN_Q_OVERFLOW;
        struct epoll_event events[10];
        while (true) {
            int nfds = epoll_wait(epoll_fd_, events, 10, -1);
            if (nfds < 0 && errno == EINTR) {
                continue;
            }
            if (nfds < 0) {
                return false;
            }
            for (int i = 0; i < nfds; ++i) {
                int fd = events[i].data.fd;
                if (fd == stop_pipe_[0]) {
                    return true;
                }
                unsigned int avail;
                ioctl(fd, FIONREAD, &avail);
                char *buffer = (char*)calloc(avail, sizeof(char));
                int len = read(fd, buffer, avail);
                int offset = 0;
                while (offset < len) {
                    struct inotify_event *event = (inotify_event*)(buffer + offset);
                    offset = offset + sizeof(inotify_event) + event->len;
                    if (event->mask & IN_IGNORED) {
                        // need to continue cause the bug
                        // https://man7.org/linux/man-pages/man7/inotify.7.html#BUGS
                        continue;
                    }
                    std::lock_guard<std::recursive_mutex> locker(recursive_mtx_);
                    if (!fd2path_map_.count(event->wd)) {
                        continue;
                    }
                    const std::string &fpath = fd2path_map_[event->wd];
                    if (!path2func_map_.count(fpath)) {
                        remove(fpath);
                        continue;
                    }
                    // cb may oprate file_watcher
                    auto cb = path2func_map_[fpath];
                    // only call callback once
                    remove(fpath);
                    (*(cb))();
                }
                free(buffer);
            }
        }
    }));
    return true;
}

bool FileWatcher::add(const std::string &fpath, uint32_t masks,
                      std::shared_ptr<std::function<void()>> func) {
    if (!init_.load(std::memory_order_acquire)) {
        std::cerr << "file_watcher not init";
        return false;
    }
    std::lock_guard<std::recursive_mutex> locker(recursive_mtx_);
    int wd = inotify_add_watch(notify_fd_, fpath.c_str(), masks);
    if (wd == -1) {
        std::cerr << "fail inotify_add_watch add path:[" << fpath
                   << "], error:" << strerror(errno);
        return false;
    }
    fd2path_map_[wd] = fpath;
    path2fd_map_[fpath] = wd;
    path2func_map_[fpath] = func;
    return true;
}

bool FileWatcher::remove(const std::string &fpath) {
    std::lock_guard<std::recursive_mutex> locker(recursive_mtx_);
    if (path2func_map_.count(fpath)) {
        path2func_map_.erase(fpath);
    }
    if (path2fd_map_.count(fpath)) {
        int wd = path2fd_map_[fpath];
        path2fd_map_.erase(fpath);
        if (inotify_rm_watch(notify_fd_, wd)) {
            std::cerr << "fail remove notify path:" << fpath
                       << ", cause:"  << strerror(errno);
        }
        if (fd2path_map_.count(wd)) {
            fd2path_map_.erase(wd);
        }
    }
    return true;
}
};
