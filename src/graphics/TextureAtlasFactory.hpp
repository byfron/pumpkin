#pragma once
#include "TextureAtlas.hpp"
#include <texture_atlas.pb.h>

namespace pumpkin {

class TextureAtlasFactory {

public:

	TextureAtlasFactory(const voyage::AtlasCfg & config) : m_atlas_cfg(config)  {}

	void generate(TextureAtlas* atlas) {

		atlas->m_id = m_atlas_cfg.resource_id();
		atlas->m_atlas_color = std::string(TEXTURE_FILE_PATH) +
			m_atlas_cfg.color_file();

		if (m_atlas_cfg.has_normal_file()) {
			atlas->m_atlas_normal = std::string(TEXTURE_FILE_PATH) +
				m_atlas_cfg.normal_file();
			atlas->m_has_normalmap = true;
		}

		atlas->m_grid_width = m_atlas_cfg.grid_width();
		atlas->m_grid_height = m_atlas_cfg.grid_height();
		atlas->m_sprite_width = m_atlas_cfg.sprite_width();
		atlas->m_sprite_height = m_atlas_cfg.sprite_height();
	}

private:

	voyage::AtlasCfg m_atlas_cfg;
};

}
