#pragma once
#include "TextureAtlas.hpp"
#include <utils/Configuration.hpp>
#include <texture_atlas.pb.h>


namespace pumpkin {

class TextureAtlasFactory {

public:

	typedef Configuration<voyage::AtlasCfg> Config;

	TextureAtlasFactory(std::string config_file) : m_atlas_cfg(Config(config_file))  {}

	void generate(TextureAtlas* atlas) {

		atlas->m_id = m_atlas_cfg.config().resource_id();
		atlas->m_atlas_file = std::string(TEXTURE_FILE_PATH) +
			m_atlas_cfg.config().atlas_file();
		atlas->m_grid_width = m_atlas_cfg.config().grid_width();
		atlas->m_grid_height = m_atlas_cfg.config().grid_height();
	}

private:

	Config m_atlas_cfg;
};

}
