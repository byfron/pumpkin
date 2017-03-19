#pragma once

#include "GraphicsObject.hpp"
#include <utils/Configuration.hpp>
#include <graphics.pb.h>

namespace pumpkin {

class GraphicsObjectFactory {

public:
	GraphicsObjectFactory(const GraphicsObject::Config & config) :
		m_config(config) {}

	bool generate(GraphicsObject *go) {
		go->loadTexture(m_config.atlas_id());
		go->loadShader(m_config.shader_id());
	}

private:
	std::string m_config_file;
	GraphicsObject::Config m_config;
};

}
