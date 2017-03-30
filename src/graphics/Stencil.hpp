//
// Created by rubio on 27/03/17.
//

#ifndef VOYAGE_STENCIL_HPP
#define VOYAGE_STENCIL_HPP

#include <utils/VertexUtils.hpp>
#include <graphics/GraphicsEngine.hpp>
#include <common/math.hpp>
#include <vector>

namespace pumpkin {
	class Stencil {

	public:
		Stencil();
		void clearStencil();
		void craftStencil(const std::vector<Vec2f> & polygon,
				  const Vec2f & player_pos);
		void updateDynamicVertexBuffer();

	protected:

		RenderState m_craftStencilState;
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;
		uint8_t m_viewId;
		Shader::Ptr m_shaderColorBlack;

		std::vector<Vec3f> m_vertices;
		std::vector<uint16_t> m_indices;
	};
}

#endif //VOYAGE_STENCIL_HPP
