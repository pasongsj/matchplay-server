#pragma once

#include <thread>

namespace ThreadUtil
{
    template <typename Callable, typename... Args>
    std::thread CreateDetachedThread(Callable&& func, Args&&... args)
    {
        std::thread t(std::forward<Callable>(func), std::forward<Args>(args)...);
        t.detach();
        return t;
    }
}
