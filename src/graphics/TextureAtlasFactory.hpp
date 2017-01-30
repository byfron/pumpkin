#pragma once
#include "TextureAtlas.hpp"
#include <texture_atlas.pb.h>

namespace pumpkin {

class TextureAtlasFactory {

public:

	TextureAtlasFactory(const voyage::AtlasCfg & config) : m_atlas_cfg(config)  {}

	void generate(TextureAtlas* atlas) {

		atlas->m_id = m_atlas_cfg.resource_id();
		atlas->m_atlas_file = std::string(TEXTURE_FILE_PATH) +
			m_atlas_cfg.atlas_file();
		atlas->m_grid_width = m_atlas_cfg.grid_width();
		atlas->m_grid_height = m_atlas_cfg.grid_height();
	}

private:

	voyage::AtlasCfg m_atlas_cfg;
};

}
