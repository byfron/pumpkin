#include "Animation.hpp"
#include "AnimationFactory.hpp"

namespace pumpkin {

	Animation::Animation(const Animation::Config & config) :
		GraphicsObject(config.graphics_properties()),
		m_frame_period(0.04),
		m_enabled(true) {	

	AnimationFactory factory(config);
	factory.generate(this);
}

}
