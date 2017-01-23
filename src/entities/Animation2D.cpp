#include "Animation2D.hpp"

Animation2D::Animation2D(const std::string & texture,
			 const std::string & shader,
			 float width, float height) : Entity2D(texture, shader, width, height),
						      m_active_anim(AnimationId::IDLE),
						      m_active_frame(0),
						      m_enabled(true),
						      m_time_since_last_anim(0),
						      m_frame_period(0.2)
{

}

void Animation2D::addFrame(AnimationId id, const AnimationFrame & frame) {
	m_animation[id].push_back(frame);
}
