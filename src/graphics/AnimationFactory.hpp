#pragma once
#include "Animation.hpp"
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <animation.pb.h>
#include <graphics.pb.h>

namespace pumpkin {
	
class AnimationFactory {

public:

	//TODO: we can parse outside and pass the cfg object!
	// TODO: Bring this to an parent factory
	AnimationFactory(std::string config_file) : m_config_file(config_file)  {

		int fileDescriptor = open(m_config_file.c_str(), O_RDONLY);
		google::protobuf::io::FileInputStream fileInput(fileDescriptor);
		fileInput.SetCloseOnDelete( true );

		if (!google::protobuf::TextFormat::Parse(&fileInput, &m_animation_list_cfg))
		{
			std::cout << "Failed to parse file!" << std::endl;
		}

		//TODO: check atlas sanity (sizes make sense)
	}

	//TODO: change all to this style
	bool generate(Animation* anim) {

		voyage::GraphicsObjectCfg graphics_cfg = m_animation_list_cfg.graphics_properties();
		anim->loadTextureAtlas(graphics_cfg.atlas_id());

		// Add all animation frames
		for (int i = 0; i < m_animation_list_cfg.animation().size(); i++) {

			voyage::AnimationCfg anim_cfg = m_animation_list_cfg.animation(i);
			
			for (int i = 0; i < anim_cfg.frames().size(); i++) {

				voyage::AtlasFrameCfg frame = anim_cfg.frames(i);
				
				anim->addFrame(anim_cfg.animation_id(),
					       Animation::AtlasFrame(float(frame.x())/4.0,
								     float(frame.y())/4.0,
								     frame.flipped()));
			}

		}


		// graphics initialisation
		anim->loadShader(graphics_cfg.shader_id());
		anim->m_width = graphics_cfg.width();
		anim->m_height = graphics_cfg.height();
		
		anim->switchToAnim(m_animation_list_cfg.animation(0).animation_id());
		anim->updateAtlasOffset();
		return true;
	}	

private:

std::string m_config_file;
voyage::AnimationListCfg m_animation_list_cfg;

};

}
