#pragma once

#include <vector>
#include <memory>

#include "Vroom/Core/Layer.h"

namespace vrm
{

class Layer;

/**
 * @brief Layer stack class. Layers are stored in a stack, where the top layer is the last one added.
 * Layer are rendered in the order they are added, and updated in the reverse order.
 * 
 */
class LayerStack
{
public:
    using Container = std::vector<std::unique_ptr<Layer>>;
public:

    /**
     * @brief Iterator class for the layer stack.
     * 
     */
    class Iterator
    {
    public:
        Iterator(Container::iterator it)
            : m_Iterator(it)
        {
        }

        Iterator& operator++()
        {
            ++m_Iterator;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator it = *this;
            ++m_Iterator;
            return it;
        }

        Iterator& operator--()
        {
            --m_Iterator;
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator it = *this;
            --m_Iterator;
            return it;
        }

        Layer& operator*() { return **m_Iterator; }
        Layer* operator->() { return m_Iterator->get(); }

        bool operator==(const Iterator& other) const { return m_Iterator == other.m_Iterator; }
        bool operator!=(const Iterator& other) const { return m_Iterator != other.m_Iterator; }

    private:
        Container::iterator m_Iterator;
    };

    /**
     * @brief Reverse iterator class for the layer stack.
     * 
     */
    class ReverseIterator
    {
    public:
        ReverseIterator(Container::reverse_iterator it)
            : m_Iterator(it)
        {
        }

        ReverseIterator& operator++()
        {
            ++m_Iterator;
            return *this;
        }

        ReverseIterator operator++(int)
        {
            ReverseIterator it = *this;
            ++m_Iterator;
            return it;
        }

        ReverseIterator& operator--()
        {
            --m_Iterator;
            return *this;
        }

        ReverseIterator operator--(int)
        {
            ReverseIterator it = *this;
            --m_Iterator;
            return it;
        }

        Layer& operator*() { return **m_Iterator; }
        Layer* operator->() { return m_Iterator->get(); }

        bool operator==(const ReverseIterator& other) const { return m_Iterator == other.m_Iterator; }
        bool operator!=(const ReverseIterator& other) const { return m_Iterator != other.m_Iterator; }

    private:
        Container::reverse_iterator m_Iterator;
    };

public:
    LayerStack() = default;
    ~LayerStack() = default;

    /**
     * @brief Push a layer to the layer stack.
     * 
     * @param layer Layer to push.
     * @return Layer& Reference to the pushed layer.
     */
    Layer& push(std::unique_ptr<Layer>&& layer)
    {
        m_Layers.emplace_back(std::move(layer));
        return top();
    }

    /**
     * @brief Pop the top layer from the layer stack.
     * 
     */
    void pop()
    {
        m_Layers.pop_back();
    }

    /**
     * @brief Get the top layer.
     * 
     * @return Layer& Reference to the top layer.
     */
    Layer& top()
    {
        return *m_Layers.back();
    }

    /**
     * @brief Get the top layer.
     * 
     * @return const Layer& Reference to the top layer.
     */
    const Layer& top() const
    {
        return *m_Layers.back();
    }

    /**
     * @brief Clear the layer stack.
     * 
     */
    void clear()
    {
        m_Layers.clear();
    }

    // Bottom to top iteration

    /**
     * @brief Get the begin iterator.
     * 
     * @return Iterator Begin iterator.
     */
    Iterator begin() { return Iterator(m_Layers.begin()); }

    /**
     * @brief Get the end iterator.
     * 
     * @return Iterator End iterator.
     */
    Iterator end() { return Iterator(m_Layers.end()); }

    // Top to bottom iteration

    /**
     * @brief Get the reverse begin iterator.
     * 
     * @return ReverseIterator Reverse begin iterator.
     */
    ReverseIterator rbegin() { return ReverseIterator(m_Layers.rbegin()); }

    /**
     * @brief Get the reverse end iterator.
     * 
     * @return ReverseIterator Reverse end iterator.
     */
    ReverseIterator rend() { return ReverseIterator(m_Layers.rend()); }

private:
    Container m_Layers;
};

} // namespace vrm