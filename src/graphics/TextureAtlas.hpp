#pragma once

#include "ResourceManager.hpp"
#include "common/defines.hpp"
#include <common/common.h>
#include <common/bgfx_utils.h>
#include <iostream> // CHANGE THIS WITH PROPER LOG
#include <string>
#include <vector>
#include <assert.h>
#include <memory>
#include <texture_atlas.pb.h>

namespace pumpkin {

class TextureAtlasFactory;

class TextureAtlas : public Resource {
public:

	typedef std::shared_ptr<TextureAtlas> Ptr;

	TextureAtlas(const voyage::AtlasCfg &);

	static uint32_t type() { return TEXTURE_ATLAS_RESOURCE_TYPE; }

	~TextureAtlas() {
		bgfx::destroyTexture(m_textureColor);
		bgfx::destroyTexture(m_textureNormal);
	}

	void init() {
		m_textureColor = loadTexture(m_atlas_color.c_str());
		if (m_has_normalmap)
			m_textureNormal = loadTexture(m_atlas_normal.c_str());
	}

	bgfx::TextureHandle & getColorHandle() {
		return m_textureColor;
	}

	bgfx::TextureHandle & getNormalHandle() {
		return m_textureColor;
	}

	uint16_t getSpriteWidthCoord() const {
		uint16_t texel_size = 0x7fff / (32 * m_grid_height);
		return 0x7fff/m_grid_width - texel_size/2;
	}

	uint16_t getSpriteHeightCoord() const {
		uint16_t texel_size = 0x7fff / (32 * m_grid_height);
		return 0x7fff/m_grid_height - texel_size/2;
	}

	bool hasNormalMap() const {
		return m_has_normalmap;
	}
	
	// const AtlasFrame & getFrame(uint32_t id) const {
	// 	assert(id < m_frames.size());
	// 	return m_framfaes[id];
	// }

protected:

	friend TextureAtlasFactory;
	bgfx::TextureHandle m_textureColor;
	bgfx::TextureHandle m_textureNormal;
	std::string m_atlas_color;
	std::string m_atlas_normal;

	int m_grid_width;
	int m_grid_height;

	bool m_has_normalmap;

	// TODO load all atlases from a config file of atlases?
	// Load them when we need them better I guess.
	// Make TextureAtlasFactory
	//grid size

};

}
