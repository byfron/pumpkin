#pragma once

#include "ResourceManager.hpp"
#include <common/common.h>
#include <common/bgfx_utils.h>
#include <iostream> // CHANGE THIS WITH PROPER LOG
#include <string>
#include <vector>
#include <assert.h>
#include <memory>

namespace pumpkin {

class TextureAtlasFactory;
	
class TextureAtlas : public Resource {
public:

	typedef std::shared_ptr<TextureAtlas> Ptr;	

	TextureAtlas(const std::string & atlas_cfg_file);

	~TextureAtlas() {       	
		bgfx::destroyTexture(m_textureColor);
		bgfx::destroyTexture(m_textureNormal);
	}

	void init() {
		m_textureColor = loadTexture(m_atlas_color.c_str());
		m_textureNormal = loadTexture(m_atlas_normal.c_str());
	}

	bgfx::TextureHandle & getColorHandle() {
		return m_textureColor;
	}

	bgfx::TextureHandle & getNormalHandle() {
		return m_textureColor;
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
	
	// TODO load all atlases from a config file of atlases?
	// Load them when we need them better I guess.
	// Make TextureAtlasFactory
	//grid size
	
};

}
