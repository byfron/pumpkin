#include "TextureAtlas.hpp"
#include "TextureAtlasFactory.hpp"

namespace pumpkin {
TextureAtlas::TextureAtlas(const voyage::AtlasCfg &cfg) {

	m_offset[0] = 0.0f;
	m_offset[1] = 0.0f;
	
	// generate atlas frames
	TextureAtlasFactory factory(cfg);
	factory.generate(this);
}

}
