#pragma once
#include "TextureAtlas.hpp"
#include <texture_atlas.pb.h>

namespace pumpkin {

class TextureAtlasFactory {

public:

	TextureAtlasFactory(const voyage::AtlasCfg & config) : m_atlas_cfg(config)  {}

	void generate(TextureAtlas* atlas) {

		atlas->m_id = m_atlas_cfg.resource_id();
		atlas->m_atlas_file[0] = std::string(TEXTURE_FILE_PATH) +
			m_atlas_cfg.color_file();
		std::cout << "loading " << atlas->m_atlas_file[0] << std::endl;
		atlas->m_num_textures = 1;

		// if (m_atlas_cfg.has_normal_file()) {
		// 	atlas->m_atlas_file[1] = std::string(TEXTURE_FILE_PATH) +
		// 		m_atlas_cfg.normal_file();
		// 	atlas->m_num_textures++;
		// }

		atlas->m_grid_width = m_atlas_cfg.grid_width();
		atlas->m_grid_height = m_atlas_cfg.grid_height();
		atlas->m_sprite_width = m_atlas_cfg.sprite_width();
		atlas->m_sprite_height = m_atlas_cfg.sprite_height();

		atlas->m_texture_handle[0] = loadTexture(atlas->m_atlas_file[0].c_str());
		
		atlas->m_flags[0] = UINT32_MAX;
		atlas->m_stage[0] = 0;
		atlas->u_sampler[0] = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
		atlas->u_texOffset = bgfx::createUniform("packed_input",  bgfx::UniformType::Vec4);
		atlas->u_flip = bgfx::createUniform("flip", bgfx::UniformType::Int1);
	}

private:

	voyage::AtlasCfg m_atlas_cfg;
};

}
