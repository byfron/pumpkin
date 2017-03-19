#pragma once

#include "ResourceManager.hpp"
#include "common/defines.hpp"
#include <common/common.h>
#include <common/bgfx_utils.h>
#include <common/math.hpp>
#include <iostream> // CHANGE THIS WITH PROPER LOG
#include <string>
#include <vector>
#include <assert.h>
#include <memory>
#include <texture_atlas.pb.h>

namespace pumpkin {

class Mesh;
	
struct AtlasFrame {
	typedef Eigen::Matrix<int16_t, 2, 1> Corner;
	Corner top_left;
	Corner bottom_right;
};

class TextureAtlasFactory;

class TextureAtlas : public Resource {
public:

	typedef std::shared_ptr<TextureAtlas> Ptr;

	TextureAtlas(const voyage::AtlasCfg &);

	static uint32_t type() { return TEXTURE_ATLAS_RESOURCE_TYPE; }

	~TextureAtlas() {
		for (int i = 0; i < m_num_textures; i++) {
			bgfx::destroyTexture(m_texture[i]);
			bgfx::destroyUniform(u_sampler[i]);
		}

		bgfx::destroyUniform(u_texOffset);
		bgfx::destroyUniform(u_flip);
	}

	void init() {

		// load texture (0)
		m_texture[0] = loadTexture(m_atlas_file[0].c_str());
		
		// create uniforms
		u_sampler[0]  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
		u_texOffset = bgfx::createUniform("packed_input",  bgfx::UniformType::Vec4);
		u_flip = bgfx::createUniform("flip", bgfx::UniformType::Int1);
	}

	bgfx::TextureHandle & getColorHandle() {
		return m_texture[0];
	}


	// gets atlas frames in texture coordinates
	std::vector<AtlasFrame> getAtlasFrames(const std::vector<Vec2i> & coords) {

		uint16_t texel_height = 0x7fff / (m_sprite_height * m_grid_height);
		uint16_t frame_height = 0x7fff /  m_grid_height;
		uint16_t texel_width  = 0x7fff / (m_sprite_width * m_grid_width);
		uint16_t frame_width  = 0x7fff /  m_grid_width;
		std::vector<AtlasFrame> out;

		for (auto coord: coords) {
			AtlasFrame af;
			af.top_left = AtlasFrame::Corner(frame_width*coord(0) + texel_width/2.0,
							 frame_height*coord(1) + texel_height/2.0);

			af.bottom_right = AtlasFrame::Corner(frame_width*(coord(0)+1)-texel_width/2.0,
							     frame_height*(coord(1)+1)-texel_height/2.0);
			out.push_back(af);
		}

		return out;
	}

	void updateAtlasFrame(float offset0, float offset1, bool flipped) {
		m_offset[0] = offset0;
		m_offset[1] = offset1;
		m_flipped = flipped;		       		
	}
	
protected:

	friend TextureAtlasFactory;
	friend Mesh;
	
	int m_num_textures;
	
	// We only support 4 textures per mesh
	bgfx::TextureHandle m_texture[4];
	uint32_t m_flags[4];
	uint8_t  m_stage[4];
	bgfx::UniformHandle u_sampler[4];
	std::string m_atlas_file[4];
	
	// Atlas properties
	int m_grid_width;
	int m_grid_height;
	int m_sprite_width;
	int m_sprite_height;	
	bgfx::UniformHandle u_flip;
	bgfx::UniformHandle u_texOffset;
	bool m_flipped;
	float m_offset[2];


};

}
