#pragma once

#include "Shader.hpp"
#include <shader.pb.h>

namespace pumpkin {

class ShaderFactory {
public:

	ShaderFactory(const voyage::ShaderCfg & cfg) : m_shader_cfg(cfg)  {}
	void generate(Shader* shader) {

		shader->m_id = m_shader_cfg.resource_id();
		shader->m_vs_shader = m_shader_cfg.vertex_shader();
		shader->m_fs_shader = m_shader_cfg.fragment_shader();
	}
private:


private:

	//ShaderProperties - inputs for the shader
	voyage::ShaderCfg m_shader_cfg;

};
}
