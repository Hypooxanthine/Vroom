#pragma once

#include <unordered_map>

#include "Vroom/Core/Assert.h"

/**
 * @brief Associative container with constant time lookup in both ways.
 * T1 elements AND T2 elements have to be unique.
 * @tparam T1 First key. Needs a hash and a == operator.
 * @tparam T2 Second key. Needs a hash and a == operator.
 */
template<typename T1, typename T2>
class TwoWaysUnorderedMap
{
public:
	using value_type = std::pair<T1, T2>;

private:
	// using value_type12 = std::pair<T1, T2*>;
	// using value_type21 = std::pair<T2, T1*>;

public:
	/**
	 * @brief Creates an empty TwoWaysUnorderedMap.
	 */
	TwoWaysUnorderedMap()
	{}

	/**
	 * @brief Creates a TwoWaysUnorderedMap from an initializer list.
	 * @param init List of pairs of elements.
	 */
	TwoWaysUnorderedMap(std::initializer_list<value_type> init)
	{
		insert(init);
	}

	/**
	 * @brief Insert a single pair of elements.
	 * @param element The element to insert. Left and right values must have never been inserted into the container before.
	 */
	void insert(const value_type& element)
	{
		VRM_ASSERT(!m_T1ToT2.contains(element.first));
		VRM_ASSERT(!m_T2ToT1.contains(element.second));

		m_T1ToT2.insert({ element.first, element.second });
		m_T2ToT1.insert({ element.second, element.first });
	}

	/**
	 * @brief Insert a list of elements.
	 * @param list List of pairs of elements.
	 */
	void insert(std::initializer_list<value_type> list)
	{
		for (const auto& element : list)
		{
			insert(element);
		}
	}

	/**
	 * @brief Check if container contains a specific key.
	 * @param left Left key to check.
	 * @return True if contains the key. False otherwise.
	 */
	bool containsLeft(const T1& left) const
	{
		return m_T1ToT2.contains(left);
	}

	/**
	 * @brief Check if container contains a specific key.
	 * @param right Right key to check.
	 * @return Trus if contains the key. False otherwise.
	 */
	bool containsRight(const T2& right) const
	{
		return m_T2ToT1.contains(right);
	}

	/**
	 * @brief Gets the right key from a left key.
	 * @param left Left key. Must exist in the container.
	 * @return The right key.
	 */
	const T2& getRight(const T1& left) const
	{
		return m_T1ToT2.at(left);
	}

	/**
	 * @brief Gets the left key from the right key.
	 * @param right Right key. Must exist in the container.
	 * @return The left key.
	 */
	const T1& getLeft(const T2& right) const
	{
		return m_T2ToT1.at(right);
	}

private:
	std::unordered_map<T1, T2> m_T1ToT2;
	std::unordered_map<T2, T1> m_T2ToT1;
};