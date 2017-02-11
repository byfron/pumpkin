#pragma once

#include "SpriteObject.hpp"
#include "TextureAtlas.hpp"
#include <map>

namespace pumpkin {

class AnimationFactory;

class Animation : public SpriteObject {

public:

	// Load from data. TODO: refactor this to TextureAtlas?
	struct AtlasFrame {
		AtlasFrame(float x, float y, bool f) : atlas_offset_x(x),
						       atlas_offset_y(y),
						       flipped(f) {}
		float atlas_offset_x;
		float atlas_offset_y;
		bool flipped;

	};

	Animation(std::string anim_cfg);

	void updateAtlasOffset() {
		assert(isValid());
		m_atlas_offset[0] = m_animation[m_active_anim][m_active_frame].atlas_offset_x;
		m_atlas_offset[1] = m_animation[m_active_anim][m_active_frame].atlas_offset_y;
		m_flipped = m_animation[m_active_anim][m_active_frame].flipped;

		std::cout << "offset:" << m_atlas_offset[0] << "," <<  m_atlas_offset[1] << std::endl;
	}

	void init() {
		SpriteObject::init();
	}

	bool isValid() {
		return m_animation.count(m_active_anim) &&
			m_animation[m_active_anim].size() > m_active_frame;
	}

	void update(float dt) {
		if (isTimeToSwitchFrames(dt) && m_enabled) { frameForward(); }
		SpriteObject::update(dt);
	}

	void addFrame(uint32_t anim_id, const AtlasFrame & frame) {
			m_animation[anim_id].push_back(frame);
	}

	void switchToAnim(uint32_t id) {
		m_enabled = true;
		if (id != m_active_anim) {

			m_active_anim = id;
			m_active_frame = 0;
		}
	}

	bool isTimeToSwitchFrames(float dt) {

		m_time_since_last_anim += dt;

		if (m_time_since_last_anim >= m_frame_period) {
			m_time_since_last_anim = 0;
			return true;
		}
		return false;
	}

	void frameForward() {
		if (m_active_frame >= m_animation[m_active_anim].size() - 1) {
			m_active_frame = 0;
		}
		else m_active_frame++;
		updateAtlasOffset();
	}

	void stop() {
		m_enabled = false;
	}

protected:

	float m_time_since_last_anim;
	float m_frame_period;
	int16_t m_active_anim;
	int16_t m_active_frame;
	bool m_enabled;

	std::map<uint32_t, std::vector<AtlasFrame> > m_animation;

};

}
