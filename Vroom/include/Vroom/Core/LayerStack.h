#pragma once

#include <vector>
#include <memory>

#include "Vroom/Core/Layer.h"

namespace vrm
{

class Layer;

class LayerStack
{
public:
    using Container = std::vector<std::unique_ptr<Layer>>;
public:
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

    Layer& push(std::unique_ptr<Layer>&& layer)
    {
        m_Layers.emplace_back(std::move(layer));
        return top();
    }

    void pop()
    {
        m_Layers.pop_back();
    }

    Layer& top()
    {
        return *m_Layers.back();
    }

    const Layer& top() const
    {
        return *m_Layers.back();
    }

    void clear()
    {
        m_Layers.clear();
    }

    // Bottom to top iteration
    Iterator begin() { return Iterator(m_Layers.begin()); }
    Iterator end() { return Iterator(m_Layers.end()); }

    // Top to bottom iteration
    ReverseIterator rbegin() { return ReverseIterator(m_Layers.rbegin()); }
    ReverseIterator rend() { return ReverseIterator(m_Layers.rend()); }

private:
    Container m_Layers;
};

} // namespace vrm