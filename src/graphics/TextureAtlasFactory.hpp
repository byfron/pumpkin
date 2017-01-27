#pragma once
#include "TextureAtlas.hpp"
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <texture_atlas.pb.h>
#include <animation.pb.h>

namespace pumpkin {

class TextureAtlasFactory {

public:
	// TODO: Bring this to an parent factory
	TextureAtlasFactory(std::string config_file) : m_config_file(config_file)  {

		int fileDescriptor = open(m_config_file.c_str(), O_RDONLY);
		google::protobuf::io::FileInputStream fileInput(fileDescriptor);
		fileInput.SetCloseOnDelete( true );

		if (!google::protobuf::TextFormat::Parse(&fileInput,
							 &m_atlas_cfg))
		{
			std::cout << "Failed to parse file!" << std::endl;
		}

		//TODO: check atlas sanity (sizes make sense)
	}

	void generate(TextureAtlas* atlas) {

		atlas->m_id = m_atlas_cfg.resource_id();
		atlas->m_atlas_file = std::string(TEXTURE_FILE_PATH) + m_atlas_cfg.atlas_file();
		atlas->m_grid_width = m_atlas_cfg.grid_width();
		atlas->m_grid_height = m_atlas_cfg.grid_height();

	}
private:


	std::string m_config_file;
	voyage::AtlasCfg m_atlas_cfg;

};

}
