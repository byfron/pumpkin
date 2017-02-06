#pragma once

#include "SpriteObject.hpp"
#include <utils/Configuration.hpp>
#include <graphics.pb.h>

namespace pumpkin {

class SpriteObjectFactory {

public:
	typedef Configuration<voyage::SpriteObjectCfg> Config;

	SpriteObjectFactory(const std::string & config_file) :
		m_config(Config(config_file)) {}

	bool generate(SpriteObject *go) {
		go->loadTextureAtlas(m_config.config().atlas_id());
		go->loadShader(m_config.config().shader_id());
		go->m_width = m_config.config().width();
		go->m_height = m_config.config().height();
	}

private:
	Config m_config;
};

}
