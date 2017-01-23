#pragma once
#include "Animation2D.hpp"
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <graphics.pb.h>

class AnimationFactory {

public:
	AnimationFactory(std::string config_file) : m_config_file(config_file)  {

		int fileDescriptor = open(m_config_file.c_str(), O_RDONLY);
		google::protobuf::io::FileInputStream fileInput(fileDescriptor);
		fileInput.SetCloseOnDelete( true );

		if (!google::protobuf::TextFormat::Parse(&fileInput, &m_graphics_cfg))
		{
			std::cout << "Failed to parse file!" << std::endl;
		}

		//TODO: check atlas sanity (sizes make sense)
	}

	void generate(Animation2D* anim) {

		// Add all animation frames
		for (int i = 0; i < m_graphics_cfg.animation().size(); i++) {
			voyage::AnimationCfg anim_cfg = m_graphics_cfg.animation(i);
			std::cout << "add anim:" << i << std::endl;
			for (int i = 0; i < anim_cfg.frames().size(); i++) {
				voyage::AtlasFrameCfg f = anim_cfg.frames(i);

				
				getAnimFrame(anim_cfg.atlas_id(),
					     (uint32_t)f.x(),
					     (uint32_t)f.y(),
					     f.flipped());


				std::cout << "adding frame in " << anim_cfg.animation_id() << std::endl;
				
				anim->addFrame((AnimationId)anim_cfg.animation_id(),
					       getAnimFrame(anim_cfg.atlas_id(),
							    (uint32_t)f.x(),
							    (uint32_t)f.y(),
							    f.flipped()));
			}
		}

		anim->init();
		anim->switchToAnim(AnimationId(1));
		anim->updateAtlasOffset();
	}
	
	Resource::Ptr generate() {

		// Note to self: Use ptr<>(new) instead of make_shared to avoid
		// misalignment issues when dynamically allocating Eigen containers
		Resource::Ptr entity = std::shared_ptr<Animation2D>(new Animation2D("", "", 0.5,0.5));
		entity->init();
		std::shared_ptr<Animation2D> anim = std::dynamic_pointer_cast<Animation2D>(entity);
	       
		// Add all animation frames
		for (int i = 0; i < m_graphics_cfg.animation().size(); i++) {
			voyage::AnimationCfg anim_cfg = m_graphics_cfg.animation(i);
			for (int i = 0; i < anim_cfg.frames().size(); i++) {
				voyage::AtlasFrameCfg f = anim_cfg.frames(i);

				getAnimFrame(anim_cfg.atlas_id(),
					     (uint32_t)f.x(),
					     (uint32_t)f.y(),
					     f.flipped());
					    
				anim->addFrame((AnimationId)anim_cfg.animation_id(),
					       getAnimFrame(anim_cfg.atlas_id(),
							    (uint32_t)f.x(),
							    (uint32_t)f.y(),
							    f.flipped()));
			}
		}

		anim->switchToAnim(AnimationId(1));
		anim->updateAtlasOffset();				

		return entity;
	}

	Animation2D::AnimationFrame getAnimFrame(uint32_t atlas_id,
						 uint32_t x, uint32_t y,
						 bool flipped) {
		float xcoord = float(x)/m_graphics_cfg.atlas(atlas_id).grid_width();
	        float ycoord = float(y)/m_graphics_cfg.atlas(atlas_id).grid_height();
		return Animation2D::AnimationFrame(xcoord, ycoord, flipped);	
	}

private:

std::string m_config_file;
voyage::GraphicsCfg m_graphics_cfg;

};
