#include "TextureAtlas.hpp"
#include "TextureAtlasFactory.hpp"

namespace pumpkin {
TextureAtlas::TextureAtlas(const std::string & atlas_cfg_file) {

	// generate atlas frames
	TextureAtlasFactory factory(atlas_cfg_file);
	factory.generate(this);
}

}
