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

	///////// DEBUG /////////////
//	DebugUtils::paintHighlightedTiles()
	float scale = 0.5;
	Aabb aabb = {
		{0.0, 0.0, 0.0},
		{scale, scale, -0.01}
	};

	ddBegin(0);			
	for (auto ht : m_debug_highlight_tiles) {
		ddSetTranslate(ht(0)*scale, ht(1)*scale, 0.0f);
		ddDraw(aabb);
	}
	ddEnd();
	/////////////
	
	// Go through the layers (atlas)
	for (auto layer : m_tilemap_layers) {
		layer->update(delta);
	}       
}

}
