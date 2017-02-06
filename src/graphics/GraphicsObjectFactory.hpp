#pragma once

#include "GraphicsObject.hpp"
#include <utils/Configuration.hpp>
#include <graphics.pb.h>

namespace pumpkin {

class GraphicsObjectFactory {

public:
	typedef Configuration<voyage::GraphicsObjectCfg> Config;

	GraphicsObjectFactory(const std::string & config_file) :
		m_config(Config(config_file)) {}

	bool generate(GraphicsObject *go) {
		go->loadTextureAtlas(m_config.config().atlas_id());
		go->loadShader(m_config.config().shader_id());
		go->m_width = m_config.config().width();
		go->m_height = m_config.config().height();
	}

private:
	std::string m_config_file;
	Config m_config;
};

}
