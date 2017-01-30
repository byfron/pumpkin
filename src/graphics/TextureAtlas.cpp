#include "TextureAtlas.hpp"
#include "TextureAtlasFactory.hpp"

namespace pumpkin {
TextureAtlas::TextureAtlas(const voyage::AtlasCfg &cfg) {

	// generate atlas frames
	TextureAtlasFactory factory(cfg);
	factory.generate(this);
}

}
