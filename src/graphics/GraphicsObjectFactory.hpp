#pragma once

#include "GraphicsObject.hpp"
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <graphics.pb.h>

namespace pumpkin {

class GraphicsObjectFactory {

public:

	// TODO: Bring this to an parent factory
	GraphicsObjectFactory(std::string config_file) : m_config_file(config_file)  {

		int fileDescriptor = open(m_config_file.c_str(), O_RDONLY);
		google::protobuf::io::FileInputStream fileInput(fileDescriptor);
		fileInput.SetCloseOnDelete( true );

		if (!google::protobuf::TextFormat::Parse(&fileInput, &m_graphics_cfg))
		{
			std::cout << "Failed to parse file!" << std::endl;
		}

		//TODO: check atlas sanity (sizes make sense)
	}

bool generate(GraphicsObject *go) {	
	go->loadTextureAtlas(m_graphics_cfg.atlas_id());
	go->loadShader(m_graphics_cfg.shader_id());
	go->m_width = m_graphics_cfg.width();
	go->m_height = m_graphics_cfg.height();
}

private:
	std::string m_config_file;
	voyage::GraphicsObjectCfg m_graphics_cfg;
};

}
