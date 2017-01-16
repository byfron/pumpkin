#include "World.hpp"
#include <entities/Entity2D.hpp>
#include <utils/VertexUtils.hpp>

World::World() {
}

void World::init() {

	m_tilemap.init();

//	Entity::Ptr ent = std::make_shared<Entity2D>();
//	ent->init();
//	m_entities.push_back(ent);

}

void World::update(float delta) {

	m_tilemap.update(delta);
		
	// for (auto entity : m_entities) {
	// 	entity->update(delta);
	// }
}
