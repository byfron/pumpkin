#include "Animation.hpp"
#include "AnimationFactory.hpp"

namespace pumpkin {

	Animation::Animation(const Animation::Config & config) :
		GraphicsObject(config.graphics_properties()),
		m_frame_period(0.04),
		m_enabled(true) {

		uint64_t state = 0 |
			BGFX_STATE_DEFAULT |
			BGFX_STATE_PT_TRISTRIP |
			BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
								  BGFX_STATE_BLEND_INV_SRC_ALPHA);

		m_mesh_state_vector[0].m_state = state;

		addMesh(VertexUtils::constructVPlane(0.5, 0.5));

		AnimationFactory factory(config);
		factory.generate(this);
	}
}
