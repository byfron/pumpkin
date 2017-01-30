#pragma once
#include "Animation.hpp"
#include <utils/Configuration.hpp>
#include <iostream>
#include <animation.pb.h>
#include <graphics.pb.h>

namespace pumpkin {

class AnimationFactory {

public:

	typedef Configuration<voyage::AnimationListCfg> Config;

	AnimationFactory(std::string config_file) : m_animation_list_cfg(Config(config_file)) {}

	//TODO: change all to this style
	bool generate(Animation* anim) {

		voyage::GraphicsObjectCfg graphics_cfg = m_animation_list_cfg.config().graphics_properties();
		anim->loadTextureAtlas(graphics_cfg.atlas_id());

		// Add all animation frames
		for (int i = 0; i < m_animation_list_cfg.config().animation().size(); i++) {

			voyage::AnimationCfg anim_cfg = m_animation_list_cfg.config().animation(i);

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

		anim->switchToAnim(m_animation_list_cfg.config().animation(0).animation_id());
		anim->updateAtlasOffset();
		return true;
	}

private:

Config m_animation_list_cfg;

};

}
