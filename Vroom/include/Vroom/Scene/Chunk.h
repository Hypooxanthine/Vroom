#pragma once

#include <unordered_set>
#include <map>

#include <entt/entt.hpp>
#include "Vroom/Scene/Entity.h"

namespace Vroom
{
	//using EntitySet = std::unordered_set<Entity>;
	using EntityLayerSet = std::unordered_set<Entity>;
	using EntitySet = std::map<int, EntityLayerSet>;

	class Chunk
	{
	public:
		Chunk();
		Chunk(const Chunk&) = delete;
		Chunk(Chunk&& other) noexcept;

		void addEntity(const Entity& e);
		void removeEntity(const Entity& e);
		inline void clear() { m_Entities.clear(); }

		void fillEntities(EntitySet& set) const;

	private:
		EntitySet m_Entities;
		std::unordered_map<int, int> m_EntitiesPerLayer;
		std::unordered_map<Entity, int> m_EntityLayers;
	};

}