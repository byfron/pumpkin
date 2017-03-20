#pragma once

#include "GraphicsObject.hpp"
#include <utils/Configuration.hpp>
#include <graphics.pb.h>

namespace pumpkin {

class GraphicsObjectFactory {

public:
	// GraphicsObjectFactory(const GraphicsObject::Config & config) :
	// 	m_config(config) {}

	bool generate(Prefab *prefab) {
		prefab->loadTexture(m_config.atlas_id());
		prefab->loadShader(m_config.shader_id());
	}

private:
	std::string m_config_file;
	GraphicsObject::Config m_config;
};

}
