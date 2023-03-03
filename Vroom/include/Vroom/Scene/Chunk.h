#pragma once

#include <unordered_set>

#include <entt/entt.hpp>
#include "Vroom/Scene/Entity.h"

namespace Vroom
{
	using EntitySet = std::unordered_set<Entity>;

	class Chunk
	{
	public:
		using iterator = EntitySet::iterator;
		using const_iterator = EntitySet::const_iterator;

	public:
		Chunk();
		Chunk(const Chunk&) = delete;
		Chunk(Chunk&& other);

		void addEntity(const Entity& e);
		void removeEntity(const Entity& e);
		inline void clear() { m_Entities.clear(); }

		void fillEntities(EntitySet& set) const;

		iterator begin() { return m_Entities.begin(); }
		iterator end() { return m_Entities.end(); }
		const_iterator begin() const { return m_Entities.begin(); }
		const_iterator end() const { return m_Entities.end(); }

	private:
		EntitySet m_Entities;
	};

}