#include "Vroom/Scene/ChunkManager.h"

#include "Vroom/Utility/Math.h"

namespace Vroom
{

	ChunkManager::ChunkManager(float chunkSizeX, float chunkSizeY,
		int chunksXstart, int chunksXend,
		int chunksYstart, int chunksYend)
		: m_ChunkSizeX(chunkSizeX), m_ChunkSizeY(chunkSizeY),
		m_ChunkStartX(chunksXstart), m_ChunkEndX(chunksXend),
		m_ChunkStartY(chunksYstart), m_ChunkEndY(chunksYend)
	{
		LOG_TRACE("Adding chunks in range ({}, {}) - ({}, {}).", chunksXstart, chunksYstart, chunksXend, chunksYend);

		for (int chunkX = chunksXstart; chunkX <= chunksXend; chunkX++)
		{
			for (int chunkY = chunksYstart; chunkY <= chunksYend; chunkY++)
			{
				m_Chunks.emplace(ChunkCoords{ chunkX, chunkY }, std::make_unique<Chunk>());
			}
		}
	}

	void ChunkManager::addEntityBySprite(const Entity& e)
	{
		if (e.containsComponent<SpriteComponent>() && e.containsComponent<TransformComponent>())
		{
			auto& sprite = e.getComponent<SpriteComponent>();
			auto& transform = e.getComponent<TransformComponent>();

			sf::Vector2f scale = { transform.getScale().x * sprite.getScale().x, transform.getScale().y * sprite.getScale().y };
			sf::Vector2f translation = transform.getTranslation() + sprite.getTranslation();

			sf::FloatRect rawBox = sprite.getRawBoundingBox();
			sf::FloatRect box = { rawBox.left + translation.x, rawBox.top + translation.y, rawBox.width * scale.x, rawBox.height * scale.y };

			addEntity(e, box);
		}
	}

	void ChunkManager::addEntity(const Entity& e, std::initializer_list<sf::FloatRect> rects)
	{
		if (m_EntitiesInChunks.contains(e)) return;

		ChunkPtrSet relatedChunks;

		// We add to relatedChunks all the chunks containing all the rects.
		for (auto& rect : rects)
			getChunksContainingField(rect.left, rect.left + rect.width, rect.top, rect.top + rect.height, relatedChunks);

		m_EntitiesInChunks[e] = ChunkPtrSet();

		// Then we add the entity to each of these chunks
		for (auto* chunk : relatedChunks)
		{
			chunk->addEntity(e);
			m_EntitiesInChunks.at(e).insert(chunk);
		}
	}

	void ChunkManager::removeEntity(const Entity& e)
	{
		for (auto* chunk : m_EntitiesInChunks.at(e))
		{
			chunk->removeEntity(e);
		}

		m_EntitiesInChunks.erase(e);
	}

	void ChunkManager::refreshEntity(const Entity& e)
	{
		if (!e.containsComponent<SpriteComponent>() || !m_EntitiesInChunks.contains(e)) return;
		removeEntity(e);
		// /!\ For future other render components /!\ 
		addEntityBySprite(e);
	}

	void ChunkManager::getChunksContainingField(float startX, float endX, float startY, float endY, ChunkPtrSet& list)
	{
		int chunkXstart = int(startX / m_ChunkSizeX);
		int chunkXend = int(endX / m_ChunkSizeX);
		int chunkYstart = int(startY / m_ChunkSizeY);
		int chunkYend = int(endY / m_ChunkSizeY);

		Math::Clamp(chunkXstart, m_ChunkStartX, m_ChunkEndX);
		Math::Clamp(chunkXend, m_ChunkStartX, m_ChunkEndX);
		Math::Clamp(chunkYstart, m_ChunkStartY, m_ChunkEndY);
		Math::Clamp(chunkYend, m_ChunkStartY, m_ChunkEndY);

		for (int chunkX = chunkXstart; chunkX <= chunkXend; chunkX++)
		{
			for (int chunkY = chunkYstart; chunkY <= chunkYend; chunkY++)
			{
				list.insert(m_Chunks.at(ChunkCoords{ chunkX, chunkY }).get());
			}
		}
	}

	void ChunkManager::getChunksContainingField(float startX, float endX, float startY, float endY, ConstChunkPtrSet& list) const
	{
		int chunkXstart = int(startX / m_ChunkSizeX);
		int chunkXend = int(endX / m_ChunkSizeX);
		int chunkYstart = int(startY / m_ChunkSizeY);
		int chunkYend = int(endY / m_ChunkSizeY);

		Math::Clamp(chunkXstart, m_ChunkStartX, m_ChunkEndX);
		Math::Clamp(chunkXend, m_ChunkStartX, m_ChunkEndX);
		Math::Clamp(chunkYstart, m_ChunkStartY, m_ChunkEndY);
		Math::Clamp(chunkYend, m_ChunkStartY, m_ChunkEndY);

		for (int chunkX = chunkXstart; chunkX <= chunkXend; chunkX++)
		{
			for (int chunkY = chunkYstart; chunkY <= chunkYend; chunkY++)
			{
				list.insert(m_Chunks.at(ChunkCoords{ chunkX, chunkY }).get());
			}
		}
	}

	ChunkPtrSet ChunkManager::getChunksContainingField(float startX, float endX, float startY, float endY)
	{
		ChunkPtrSet out;

		getChunksContainingField(startX, endX, startY, endY, out);

		return out;
	}

	ConstChunkPtrSet ChunkManager::getChunksContainingField(float startX, float endX, float startY, float endY) const
	{
		ConstChunkPtrSet out;

		getChunksContainingField(startX, endX, startY, endY, out);

		return out;
	}

	void ChunkManager::clear()
	{
		for (auto& [coord, chunk] : m_Chunks)
			chunk->clear();
	}

	EntitySet ChunkManager::getEntitiesInField(float startX, float endX, float startY, float endY) const
	{
		EntitySet out;

		const auto chunks = std::move(getChunksContainingField(startX, endX, startY, endY));
		for (const auto& chunk : chunks)
			chunk->fillEntities(out);

		return out;
	}

}