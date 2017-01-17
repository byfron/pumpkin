#pragma once

#include "Entity2D.hpp"
#include <map>
#include <iostream>

enum class AnimationId {
	IDLE,
	LEFT,
	RIGHT,
	TOP,
	DOWN
};

class Animation2D : public Entity2D {

public:	
	
	// Load from data
	struct AnimationFrame {
		AnimationFrame(float x, float y) : atlas_offset_x(x),
						   atlas_offset_y(y) {}
		float atlas_offset_x;
		float atlas_offset_y;
	};

	void getAtlasOffset(float *offset) override {
		assert(isValid());
		offset[0] = m_animation[m_active_anim][m_active_frame].atlas_offset_x;
		offset[1] = m_animation[m_active_anim][m_active_frame].atlas_offset_y;
	}

	bool isValid() {
		return m_animation.count(m_active_anim) &&
			m_animation[m_active_anim].size() > m_active_frame;
	}
       	
	Animation2D();

	void update(float dt) {
	      		
		if (isTimeToSwitchFrames(dt)) { frameForward(); }

		Entity2D::update(dt);
	}
	
	void addFrame(AnimationId id, const AnimationFrame & frame);

	void switchToAnim(AnimationId id) {
		m_active_anim = id;
		m_active_frame = 0;
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
		if (m_active_frame == m_animation[m_active_anim].size() - 1) {
			m_active_frame = 0;
			return;
		}
		m_active_frame++;
	}
	
protected:

	float m_time_since_last_anim;
	float m_frame_period;
	
	AnimationId m_active_anim;
	int16_t m_active_frame; 
	std::map<AnimationId, std::vector<AnimationFrame> > m_animation;
	
};
