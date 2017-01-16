#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include <entities/Entity.hpp>
#include "TileMap.hpp"
#include <vector>

class World {

public:
	World();
	void init();
	void update(float delta);

private:

	TileMap m_tilemap;
//	std::vector<Entity::Ptr> m_entities;
};

#endif
