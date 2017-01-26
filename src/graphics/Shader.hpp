#pragma once

#include "ResourceManager.hpp"
#include <common/common.h>
#include <common/bgfx_utils.h>
#include <string>
#include <memory>

namespace pumpkin {

class ShaderFactory;	
class Shader : public Resource{
public:

	friend class ShaderFactory;
	typedef std::shared_ptr<Shader> Ptr;

	Shader(const std::string & shader_cfg_file);
	Shader(std::string fs,
	       std::string vs) : m_vs_shader(vs), m_fs_shader(fs) {
	}
	
	~Shader() {
		bgfx::destroyProgram(m_program);
	}

	void init() {
		m_program = loadProgram(m_vs_shader.c_str(), m_fs_shader.c_str());
	}

	bgfx::ProgramHandle & getHandle() {
		return m_program;
	}
	
protected:

	std::string m_vs_shader;
	std::string m_fs_shader;
	bgfx::ProgramHandle m_program;
};

}
