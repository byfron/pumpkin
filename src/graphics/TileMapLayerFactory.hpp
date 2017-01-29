#pragma once
#include "TileMapLayer.hpp"
#include <tilemap.pb.h>
#include <utils/Configuration.hpp>

namespace pumpkin {
	
class TileMapLayerFactory {

public:
	TileMapLayerFactory(const Configuration<voyage::TileMapCfg> & conf) {
		m_tilemap_cfg = conf.config();
	}

	void generate(TileMap* tilemap) {

		tilemap->clear();
		
		// Add all layers
		for (int i = 0; i < m_tilemap_cfg.layers().size(); i++) {
						
			voyage::TileMapLayerCfg layer_cfg = m_tilemap_cfg.layers(i);

			TileMapLayerProperties properties(
				m_tilemap_cfg.size_x(),
				m_tilemap_cfg.size_y(),
				layer_cfg.data_file(),
				layer_cfg.atlas_id(),
				layer_cfg.shader_id());

			// Create Layer
			TileMapLayer::Ptr layer = std::make_shared<TileMapLayer>(properties);

			// Fill layer
//			layer->addMeshObject(TileProperties(i,j, 0 , 0.0f));


			// TEST TO SHOW SOMEHING
			float scale = 0.5;
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					layer->addTile(i, j, scale);

			layer->addWall(0, 0, 0.5, 0.5);
			layer->addWall(0, 1, 0.5, 1.0);

			layer->addWall(5, 6, 0.5, 0.5);
			layer->addWall(6, 6, 0.5, 0.25);

			layer->addWall(4, 6, 0.5, 0.5);

			
			// Initialise internals
			layer->init();
			
			tilemap->addLayer(layer);
		}

		
	}
	
private:

std::string m_config_file;

	//NOTE this namespace should not be called voyage
voyage::TileMapCfg m_tilemap_cfg;

};

}
