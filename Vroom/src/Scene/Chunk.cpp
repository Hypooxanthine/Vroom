#include "Vroom/Scene/Chunk.h"

namespace Vroom
{

	Chunk::Chunk()
	{
	}

	Chunk::Chunk(Chunk&& other)
		: m_Entities(std::move(other.m_Entities))
	{
	}

	void Chunk::addEntity(const Entity& e)
	{
		m_Entities.insert(e);
	}

	void Chunk::removeEntity(const Entity& e)
	{
		m_Entities.erase(e);
	}

	void Chunk::fillEntities(EntitySet& set) const
	{
		for (auto entity : m_Entities)
			// Won't add an entity if it's already in.
			set.insert(entity);
	}

}