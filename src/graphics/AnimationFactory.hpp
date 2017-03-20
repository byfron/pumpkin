#pragma once
#include "Animation.hpp"
#include <utils/Configuration.hpp>
#include <iostream>
#include <animation.pb.h>
#include <graphics.pb.h>

namespace pumpkin {

class AnimationFactory {

public:

	AnimationFactory(const Animation::Config & config) : m_animation_list_cfg(config) {}

	//TODO: change all to this style
	bool generate(Animation* anim) {

		//voyage::SpriteObjectCfg sprite_cfg = m_animation_list_cfg.config().graphics_properties();
		//anim->loadTexture(sprite_cfg.atlas_id());

		// Add all animation frames
		for (int i = 0; i < m_animation_list_cfg.animation().size(); i++) {

			voyage::AnimationCfg anim_cfg = m_animation_list_cfg.animation(i);

			for (int i = 0; i < anim_cfg.frames().size(); i++) {

				voyage::AtlasFrameCfg frame = anim_cfg.frames(i);

				anim->addFrame(anim_cfg.animation_id(),
					       Animation::AtlasFrame(
							   float(frame.x()), //14
							   float(frame.y()), //5
							   frame.flipped()));
			}

		}


		// graphics initialisation
		//anim->loadShader(sprite_cfg.shader_id());
		//anim->m_width = sprite_cfg.width();
		//anim->m_height = sprite_cfg.height();

		anim->switchToAnim(m_animation_list_cfg.animation(0).animation_id());
		anim->updateAtlasOffset();
		return true;
	}

private:

	Animation::Config m_animation_list_cfg;

};

}
