#pragma once

#include "Shader.hpp"
#include <utils/Configuration.hpp>
#include <shader.pb.h>

namespace pumpkin {

class ShaderFactory {
public:

	typedef Configuration<voyage::ShaderCfg> Config;

	ShaderFactory(std::string config_file) : m_shader_cfg(Config(config_file))  {}
	void generate(Shader* shader) {

		shader->m_id = m_shader_cfg.config().resource_id();
		shader->m_vs_shader = m_shader_cfg.config().vertex_shader();
		shader->m_fs_shader = m_shader_cfg.config().fragment_shader();
	}
private:


private:

	//ShaderProperties - inputs for the shader
	Config m_shader_cfg;

};
}
