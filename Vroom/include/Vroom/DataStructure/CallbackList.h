#pragma once

#include <list>

namespace vrm
{

/**
 * @brief Callback list class. Used to store a list of callbacks and trigger them all at once.
 * 
 * @tparam CallbackType Type of the callback.
 */
template<typename CallbackType>
class CallbackList
{
public:
    CallbackList() = default;

    /**
     * @brief Add a callback to the list.
     * 
     * @param cb Callback to add.
     */
    inline void addCallback(const CallbackType& cb)
    {
        m_Callbacks.push_back(cb);
    }

    /**
     * @brief Add a callback to the list.
     * 
     * @tparam Args Types of the arguments.
     * @param args Arguments to forward to the callback.
     */
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
