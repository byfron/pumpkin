#include "TileMap.hpp"
#include "TileMapLayerFactory.hpp"

namespace pumpkin {
	
TileMap::TileMap(const std::string & config_file) {

	TileMapLayerFactory factory(config_file);
	factory.generate(this);	
}

void TileMap::update(float delta) {

	// Go through the layers (atlas)
	for (auto layer : m_tilemap_layers) {
		layer->update(delta);
	}       
}

}
