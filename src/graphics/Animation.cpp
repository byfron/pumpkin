#include "Animation.hpp"
#include "AnimationFactory.hpp"

namespace pumpkin {
	
Animation::Animation(std::string anim_cfg) :
	m_enabled(true) {
	AnimationFactory factory(anim_cfg);		
	factory.generate(this);	
}

}

