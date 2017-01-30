#pragma once

#include "ResourceManager.hpp"
#include <common/common.h>
#pragma once

#include "common/common.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <common/bgfx_utils.h>


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

	static uint32_t type() { return SHADER_RESOURCE_TYPE; }

	void init();

	bgfx::ProgramHandle & getHandle() {
		return m_program;
	}

protected:

	std::string m_vs_shader;
	std::string m_fs_shader;
	bgfx::ProgramHandle m_program;
};

}
