#ifndef TILEMAP_H_HEADER_GUARD
#define TILEMAP_H_HEADER_GUARD

#include "TileMapChunk.hpp"
#include <vector>

class TileMap {
public:
	TileMap();
	void init();
	void update(float delta);

	typedef std::shared_ptr<TileMap> Ptr;
private:

	std::vector<TileMapChunk::Ptr> m_chunks;
	
};


#endif
