#pragma once

#include <list>
#include <vector>
#include <unordered_map>

namespace vrm
{

/**
 * @brief Callback list class. Used to store a list of callbacks and trigger them all at once.
 * 
 * @tparam CallbackType Type of the callback.
 */
template<typename CallbackType, typename BinderType>
class CallbackList
{
public:
    CallbackList() = default;

    /**
     * @brief Add a callback to the list.
     * 
     * @param cb Callback to add.
     */
    inline void addCallback(const CallbackType& cb, BinderType* emitter)
    {
        m_Callbacks.push_back(cb);
        m_Emitters[emitter].push_back(--m_Callbacks.end());
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

    void unbindAllFromEmitter(BinderType* emitter)
    {
        if (!m_Emitters.contains(emitter))
            return;

        for (const auto& it : m_Emitters[emitter])
            m_Callbacks.erase(it);

        m_Emitters.erase(emitter);
    }

private:
    std::list<CallbackType> m_Callbacks;
    std::unordered_map<BinderType*, std::vector<typename std::list<CallbackType>::iterator>> m_Emitters;
};

} // namespace vrm
