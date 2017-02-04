#ifndef TILEMAP_H_HEADER_GUARD
#define TILEMAP_H_HEADER_GUARD

#include "TileMapLayer.hpp"
#include <common/debugdraw/debugdraw.h>
#include <utils/Configuration.hpp>
#include <tilemap.pb.h>
#include <memory>
#include <vector>

namespace pumpkin {
	
class TileMap {
public:
	typedef Eigen::Matrix<uint16_t, Eigen::Dynamic, Eigen::Dynamic> LayerData;
	
	TileMap();

	bool load(const Configuration<voyage::TileMapCfg> &);
		
	void update(float delta);

	void clear() {
		m_tilemap_layers.clear();
	}
	
	void addLayer(TileMapLayer::Ptr layer) {
		m_tilemap_layers.push_back(layer);
	}

	void reset_debug() { m_debug_highlight_tiles.clear(); }
	void highlightTile(Vec2i tile) { m_debug_highlight_tiles.push_back(tile); }
	
	typedef std::shared_ptr<TileMap> Ptr;
private:

 	// each layer is created with one specific texture atlas
 	std::vector<TileMapLayer::Ptr> m_tilemap_layers;       
	std::vector<Vec2i> m_debug_highlight_tiles;
};

}

#endif
