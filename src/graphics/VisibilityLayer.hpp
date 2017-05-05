/
// Created by rubio on 24/03/17.
//

#ifndef VOYAGE_VISIBILITYLAYER_H
#define VOYAGE_VISIBILITYLAYER_H

#include "GraphicsObject.hpp"
#include "DebugManager.hpp"
#include <utils/Triangulation.hpp>

namespace pumpkin {

    class VisibilityLayer {

    public:

        VisibilityLayer() {

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
		}

        void update(float delta) override {

			std::vector<Vec2f> triangles;
			Triangulate::Process(m_polygon, triangles);
			updateDynamicVertexBuffer(triangles);

			bgfx::setState(m_craftStencilState);

			bgfx::setIndexBuffer(m_dibh);
			bgfx::setVertexBuffer(m_dvbh);

			bgfx::submit(m_viewId,
						 m_shader->getHandle());

            pumpkin::DebugManager::push_polygon(m_polygon);

        }

		void updateDynamicVertexBuffer(const std::vector<Vec2f> & triangles) {

		}

		protected:

		std::vector<Vec2f> m_polygon;
		RenderState m_craftStencilState;
		bgfx::DynamicVertexBufferHandle m_dvbh;
		bgfx::DynamicIndexBufferHandle m_dibh;
		View m_viewId;
		Shader::Ptr m_shader;
    };
}

#endif //VOYAGE_VISIBILITYLAYER_H
