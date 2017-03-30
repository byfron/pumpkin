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

		bgfx::DynamicVertexBufferHandle m_dvbh;
		bgfx::DynamicIndexBufferHandle m_dibh;

		uint8_t m_viewId;
		Shader::Ptr m_shaderColorBlack;

		// We need to support double-buffering of the
		// vertex buffers given that we need to ensure memory stays consistent
		// within two bgfx::frame calls. Otherwise mesh flickers when updating.
		std::vector<PosColorVertex> m_vertices[2];
		std::vector<uint16_t> m_indices[2];

		int m_vertex_count;
		int m_current_buffer;
	};
}

#endif //VOYAGE_STENCIL_HPP
