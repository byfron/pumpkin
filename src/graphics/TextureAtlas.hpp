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
	}

	void init() {
		std::cout << "initialising texure: " << m_atlas_file << std::endl;
		m_textureColor = loadTexture(m_atlas_file.c_str());
	}

	bgfx::TextureHandle & getHandle() {
		return m_textureColor;
	}

	// const AtlasFrame & getFrame(uint32_t id) const {
	// 	assert(id < m_frames.size());
	// 	return m_framfaes[id];
	// }
	
protected:

	friend TextureAtlasFactory;	
	bgfx::TextureHandle m_textureColor;
	std::string m_atlas_file;

	int m_grid_width;
	int m_grid_height;
	
	// TODO load all atlases from a config file of atlases?
	// Load them when we need them better I guess.
	// Make TextureAtlasFactory
	//grid size
	
};

}