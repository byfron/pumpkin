//
// Created by rubio on 27/03/17.
//

#ifndef VOYAGE_STENCIL_HPP
#define VOYAGE_STENCIL_HPP

#include <utils/VertexUtils.hpp>
#include <utils/Triangulation.hpp>
#include <graphics/GraphicsEngine.hpp>
#include <common/math.hpp>
#include <vector>

namespace pumpkin {
    class Stencil {

    public:
		Stencil();
		void clearStencil();
        void craftStencil(const std::vector<Vec2f> &);
		void updateDynamicVertexBuffer(const std::vector<Vec2f> & triangles);

	protected:

		RenderState m_craftStencilState;
		bgfx::DynamicVertexBufferHandle m_dvbh;
		bgfx::DynamicIndexBufferHandle m_dibh;
		uint8_t m_viewId;
		Shader::Ptr m_shaderColorBlack;
	};
}

#endif //VOYAGE_STENCIL_HPP
