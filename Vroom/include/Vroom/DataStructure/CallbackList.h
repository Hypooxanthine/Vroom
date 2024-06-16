#pragma once

#include <list>
#include <functional>

namespace vrm
{

template<typename CallbackType>
class CallbackList
{
public:
    CallbackList() = default;

    inline void addCallback(const CallbackType& cb)
    {
        m_Callbacks.push_back(cb);
    }

    template <typename... Args>
    void triggerAll(Args&&... args)
    {
        for (const auto& cb : m_Callbacks)
            cb(std::forward<Args>(args)...);
    }

private:
    std::list<CallbackType> m_Callbacks;
};

} // namespace vrm
