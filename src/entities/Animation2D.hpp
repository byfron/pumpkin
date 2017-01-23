#pragma once

#include "Entity2D.hpp"
#include <map>
#include <iostream>

enum class AnimationId {
	DOWN,
	TOP,
	RIGHT,
	LEFT,
	IDLE
};

class Animation2D : public Entity2D {

public:	

	Animation2D(const std::string & texture,
		    const std::string & shader,
		    float height, float width);
	
	// Load from data
	struct AnimationFrame {
		AnimationFrame(float x, float y, bool f) : atlas_offset_x(x),
							   atlas_offset_y(y),
							   flipped(f) {}
		float atlas_offset_x;
		float atlas_offset_y;
		bool flipped;		
	};

	void updateAtlasOffset() {
		assert(isValid());
		m_atlas_offset[0] = m_animation[m_active_anim][m_active_frame].atlas_offset_x;
		m_atlas_offset[1] = m_animation[m_active_anim][m_active_frame].atlas_offset_y;
		m_flipped = m_animation[m_active_anim][m_active_frame].flipped;
	}

	bool isValid() {
		return m_animation.count(m_active_anim) &&
			m_animation[m_active_anim].size() > m_active_frame;
	}       

	void update(float dt) {	      	       
		if (isTimeToSwitchFrames(dt) && m_enabled) { frameForward(); }
		Entity2D::update(dt);
	}
	
	void addFrame(AnimationId id, const AnimationFrame & frame);

	void switchToAnim(AnimationId id) {
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

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
protected:

	float m_time_since_last_anim;
	float m_frame_period;
	
	AnimationId m_active_anim;
	int16_t m_active_frame;
	bool m_enabled;
	std::map<AnimationId, std::vector<AnimationFrame> > m_animation;
	
};
