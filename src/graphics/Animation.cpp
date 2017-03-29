#include "Animation.hpp"
#include "AnimationFactory.hpp"

namespace pumpkin {

	Animation::Animation(const Animation::Config & config) :
		m_frame_period(0.04),
		m_enabled(true) {

		uint64_t state = 0 |
			BGFX_STATE_DEFAULT |
			BGFX_STATE_PT_TRISTRIP |
			BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
			 		      BGFX_STATE_BLEND_INV_SRC_ALPHA);

		std::vector<MeshState> passes;
		MeshState pass0;
		pass0.m_state = state;
		loadTexture(&pass0, config.graphics_properties().atlas_id());
		loadShader(&pass0, config.graphics_properties().shader_id());
		passes.push_back(pass0);
		addPrefab(passes, VertexUtils::constructVPlane(0.5, 0.5));
		m_pass0 = &(m_prefabs[0].m_passes[0]);

		AnimationFactory factory(config);
		factory.generate(this);
		updateAtlasOffset(m_pass0);
	}
}
