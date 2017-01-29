#include "TileMap.hpp"
//#include "TileMapLayerFactory.hpp"

namespace pumpkin {

TileMap::TileMap() {

}
	
bool TileMap::load(const Configuration<voyage::TileMapCfg> & conf) {

//	TileMapLayerFactory factory(conf);
//	factory.generate(this);	
}

void TileMap::update(float delta) {

	// Go through the layers (atlas)
	for (auto layer : m_tilemap_layers) {
		layer->update(delta);
	}       
}

}
