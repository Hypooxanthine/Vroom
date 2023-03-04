#include "Vroom/Scene/Chunk.h"

namespace Vroom
{

	Chunk::Chunk()
	{
		// Default layer initialization
		m_Entities[0] = EntityLayerSet();
		m_EntitiesPerLayer[0] = 0;
	}

	Chunk::Chunk(Chunk&& other) noexcept
		: m_Entities(std::move(other.m_Entities))
	{
	}

	void Chunk::addEntity(const Entity& e)
	{
		int layer = 0;

		if (e.containsComponent<LayerComponent>())
		{
			layer = e.getComponent<LayerComponent>().getLayer();

			// Layer 0 is created in constructor, so we only check for entities that have a LayerComponent
			if (!m_Entities.contains(layer))
			{
				m_Entities[layer] = { e };
				m_EntitiesPerLayer[layer] = 0;
			}
			else
				m_Entities.at(layer).insert(e);
		}
		else
			m_Entities.at(layer).insert(e);

		m_EntityLayers[e] = layer;
		m_EntitiesPerLayer[layer]++;
	}

	void Chunk::removeEntity(const Entity& e)
	{
		const int& layer = m_EntityLayers.at(e);
		auto& layerSet = m_Entities.at(layer);
		layerSet.erase(e);
		if (layerSet.size() == 0) m_Entities.erase(layer);
		m_EntitiesPerLayer[layer]--;
		m_EntityLayers.erase(e);
	}

	void Chunk::fillEntities(EntitySet& set) const
	{
		for (auto& [layer, entitySet] : m_Entities)
		{
			// If receiving set does not contain a layer, we need to insert it.
			if (!set.contains(layer))
				set[layer] = EntityLayerSet();

			for (auto& entity : entitySet)
				// Won't add an entity if it's already in.
				set.at(layer).insert(entity);
		}
	}

}