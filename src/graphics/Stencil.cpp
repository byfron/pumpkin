//
// Created by rubio on 27/03/17.
//

#include "Stencil.hpp"

namespace pumpkin {

	Stencil::Stencil() {

		m_viewId = RENDER_PASS_MAKE_STENCIL;

		m_craftStencilState = {
			BGFX_STATE_RGB_WRITE
			| BGFX_STATE_DEPTH_WRITE
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA
			, UINT32_MAX
			, BGFX_STENCIL_TEST_ALWAYS         // pass always
			| BGFX_STENCIL_FUNC_REF(1)         // value = 1
			| BGFX_STENCIL_FUNC_RMASK(0xff)
			| BGFX_STENCIL_OP_FAIL_S_REPLACE
			| BGFX_STENCIL_OP_FAIL_Z_REPLACE
			| BGFX_STENCIL_OP_PASS_Z_REPLACE   // store the value
			, BGFX_STENCIL_NONE
		};

		//initialise shader
		m_shaderColorBlack = ResourceManager::getResource<Shader>(6);
	}

	void Stencil::updateDynamicVertexBuffer(const std::vector<Vec2f> & triangles) {

		// fill vertices with triangles

	}

	void Stencil::clearStencil() {

		bgfx::setViewClear(m_viewId,
						   BGFX_CLEAR_STENCIL,
						   (uint16_t)0,   //rgba
						   0.0f, //depth
						   (uint8_t)0);  //stencil
	}

	void Stencil::craftStencil(const std::vector<Vec2f> & polygon) {


		//paint polygon in stencil buffer
		std::vector<Vec2f> triangles;
		Triangulate::Process(polygon, triangles);
		updateDynamicVertexBuffer(triangles);

		bgfx::setStencil(m_craftStencilState.m_fstencil, m_craftStencilState.m_bstencil);
		bgfx::setState(m_craftStencilState.m_state, m_craftStencilState.m_blendFactorRgba);

		bgfx::setIndexBuffer(m_dibh);
		bgfx::setVertexBuffer(m_dvbh);

		// render polygon to stencil buffer
		bgfx::submit(m_viewId,
					 m_shaderColorBlack->getHandle());

		//pumpkin::DebugManager::push_polygon(m_polygon);
	}

}
