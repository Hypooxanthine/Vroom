#pragma once

#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <list>

#include "Vroom/Scene/Chunk.h"

namespace Vroom
{
	struct ChunkCoords
	{
		int x = 0, y = 0;

		friend bool operator==(const ChunkCoords& left, const ChunkCoords& right)
		{
			return left.x == right.x && left.y == right.y;
		}
	};
}

template<>
struct std::hash<Vroom::ChunkCoords>
{
	std::size_t operator()(const Vroom::ChunkCoords& coords) const noexcept
	{
		std::size_t h1 = std::hash<int>{}(coords.x);
		std::size_t h2 = std::hash<int>{}(coords.y);
		return h1 ^ (h2 << 1);
	}
};

namespace Vroom
{
	using ChunkPtrSet = std::unordered_set<Chunk*>;
	using ConstChunkPtrSet = std::unordered_set<const Chunk*>;

	class ChunkManager
	{
	public:
		ChunkManager(float chunkSizeX, float chunkSizeY,
			int chunksXstart, int chunksXend,
			int chunksYstart, int chunksYend);

		inline sf::Vector2f getChunkSize() const { return { m_ChunkSizeX, m_ChunkSizeY }; }
		inline sf::Vector2i getChunksNumber() const { return { m_ChunkEndX - m_ChunkStartX, m_ChunkEndY - m_ChunkStartY }; }

		void addEntityBySprite(const Entity& e);

		template<typename... Rects>
		void addEntity(const Entity& e, Rects&&... rects)
		{
			addEntity(e, { std::forward<Rects>(rects)... });
		}

		void addEntity(const Entity& e, std::initializer_list<sf::FloatRect> rects);
		void removeEntity(const Entity& e);
		void refreshEntity(const Entity& e);

		void getChunksContainingField(float startX, float endX, float startY, float endY, ChunkPtrSet& list);
		void getChunksContainingField(float startX, float endX, float startY, float endY, ConstChunkPtrSet& list) const;
		ChunkPtrSet getChunksContainingField(float startX, float endX, float startY, float endY);
		ConstChunkPtrSet getChunksContainingField(float startX, float endX, float startY, float endY) const;

		void clear();

		EntitySet getEntitiesInField(float startX, float endX, float startY, float endY) const;

	private:
		float m_ChunkSizeX, m_ChunkSizeY;
		int m_ChunkStartX, m_ChunkEndX, m_ChunkStartY, m_ChunkEndY;
		std::unordered_map<ChunkCoords, std::unique_ptr<Chunk>> m_Chunks;
		std::unordered_map<Entity, ChunkPtrSet> m_EntitiesInChunks;
	};

}