#pragma once

#include "Shader.hpp"
#include <shader.pb.h>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace pumpkin {

class ShaderFactory {
public:
	ShaderFactory(std::string config_file) : m_config_file(config_file)  {

		int fileDescriptor = open(m_config_file.c_str(), O_RDONLY);
		google::protobuf::io::FileInputStream fileInput(fileDescriptor);
		fileInput.SetCloseOnDelete( true );

		if (!google::protobuf::TextFormat::Parse(&fileInput,
							 &m_shader_cfg))
		{
			std::cout << "Failed to parse file!" << std::endl;
		}

		//TODO: check atlas sanity (sizes make sense)
	}

	void generate(Shader* shader) {

		shader->m_id = m_shader_cfg.resource_id();
		shader->m_vs_shader = m_shader_cfg.vertex_shader();
		shader->m_fs_shader = m_shader_cfg.fragment_shader();
	}
private:


private:

	//ShaderProperties - inputs for the shader

	std::string m_config_file;
	voyage::ShaderCfg m_shader_cfg;

};
}
