//
// Created by rubio on 27/03/17.
//

#include "Stencil.hpp"
#include "DebugManager.hpp"
#include <clip2tri/clip2tri.h>

using namespace c2t;

#define MAX_VERTEX_COUNT 512

namespace pumpkin {

	namespace {

		int makeTriangleStrip(const std::vector<Vec2f> & polygon,
							  const Vec2f & player_pos,
							  std::vector<PosColorVertex> & triangles,
							  std::vector<uint16_t> & indices) {

			if (polygon.size() == 0) {
				assert(false);
				return 0;
			}

			int vertex_count = 0;

			std::vector< std::vector<Point> > inputPolygons;
			std::vector<Point> outputTriangles;
			std::vector<Point> boundingPolygon;

			std::vector<Point> poly;
			for (auto p : polygon) {
				poly.push_back(Point(p(0), p(1)));
			}

			inputPolygons.push_back(poly);

			clip2tri clip2tri;
			clip2tri.triangulate(inputPolygons, outputTriangles, boundingPolygon);


			for (size_t i = 0; i < outputTriangles.size(); i+=3) {

				triangles[i] = {outputTriangles[i].x, outputTriangles[i].y, -0.5f, 0x00000000};
				triangles[i+1] = {outputTriangles[i+1].x, outputTriangles[i+1].y, -0.5f, 0x00000000};
				triangles[i+2] = {outputTriangles[i+2].x, outputTriangles[i+2].y, -0.5f, 0x00000000};
				// std::vector<Vec2f> poly;
				// poly.push_back(Vec2f(outputTriangles[i].x,
				// 					 outputTriangles[i].y));
				// poly.push_back(Vec2f(outputTriangles[i+1].x,
				// 					 outputTriangles[i+1].y));
				// poly.push_back(Vec2f(outputTriangles[i+2].x,
				// 					 outputTriangles[i+2].y));
				// pumpkin::DebugManager::push_polygon(poly);

				indices[i] = uint16_t(i);
				indices[i+1] = uint16_t(i+1);
				indices[i+2] = uint16_t(i+2);

			}

			vertex_count = outputTriangles.size();

			return vertex_count;
		}
	}

	Stencil::Stencil() {

		m_vbh.idx = bgfx::invalidHandle;
		m_ibh.idx = bgfx::invalidHandle;
        m_dvbh.idx = bgfx::invalidHandle;
        m_dibh.idx = bgfx::invalidHandle;

		m_vertex_count = 0;
		m_current_buffer = 0;

		// TODO. Using the stencil for this is a mess. We would have to add attachements
		// to the FBO so that the post-processing shader works properly.
		// Just create a FBO with the visibility mask and pass it to the post-processing
		// or to whatever intermediate view that handles this stuff


		m_viewId = RENDER_PASS_VISIBILITY;

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
		m_shaderColorBlack->init();

		m_vertices[0].reserve(MAX_VERTEX_COUNT);
		m_indices[0].reserve(MAX_VERTEX_COUNT);
		m_vertices[1].reserve(MAX_VERTEX_COUNT);
		m_indices[1].reserve(MAX_VERTEX_COUNT);

	}

	void Stencil::updateDynamicVertexBuffer() {

		const bgfx::Memory* mem;

		// dynamic buffer version may be more efficient
		// if (!bgfx::isValid(m_dvbh) )
		// {
		// 	m_dvbh = bgfx::createDynamicVertexBuffer(MAX_VERTEX_COUNT, PosColorVertex::ms_decl);
		// }
		// 	mem = bgfx::makeRef(&m_vertices[0], sizeof(PosColorVertex) * m_vertex_count);
		// bgfx::updateDynamicVertexBuffer(m_dvbh, 0, mem);

		// if (!bgfx::isValid(m_dibh) )
		// {
		// 	m_dibh = bgfx::createDynamicIndexBuffer(MAX_VERTEX_COUNT);
		// }
		// mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * m_vertex_count);
		// bgfx::updateDynamicIndexBuffer(m_dibh, 0, mem);


		if (bgfx::isValid(m_vbh) )
		{
			bgfx::destroyVertexBuffer(m_vbh);
		}

		mem = bgfx::makeRef(&m_vertices[m_current_buffer][0], sizeof(PosColorVertex) * m_vertex_count);
		m_vbh = bgfx::createVertexBuffer(mem, PosColorVertex::ms_decl);

		if (bgfx::isValid(m_ibh) )
		{
			bgfx::destroyIndexBuffer(m_ibh);
		}

		mem = bgfx::makeRef(&m_indices[m_current_buffer][0], sizeof(uint16_t) * m_vertex_count);
		m_ibh = bgfx::createIndexBuffer(mem);

	}

	void Stencil::clearStencil() {

		bgfx::setViewClear(m_viewId,
				   BGFX_CLEAR_STENCIL,
				   (uint16_t) 0,   //rgba
				   1.0f,           //depth
				   (uint8_t) 1);   //stencil
	}

	void Stencil::craftStencil(const std::vector<Vec2f> & polygon,
		const Vec2f & player_pos) {

		if (polygon.size() > 0) {

			//paint polygon in stencil buffer
			m_vertex_count = makeTriangleStrip(polygon, player_pos,
			m_vertices[m_current_buffer], m_indices[m_current_buffer]);

			updateDynamicVertexBuffer();

			// swap vertex buffers
			m_current_buffer = m_current_buffer?0:1;

			Eigen::MatrixXf transform = Eigen::MatrixXf::Identity(4,4);
			bgfx::setTransform(transform.data());

			//bgfx::setStencil(m_craftStencilState.m_fstencil, m_craftStencilState.m_bstencil);
			bgfx::setState(m_craftStencilState.m_state, m_craftStencilState.m_blendFactorRgba);

			bgfx::setIndexBuffer(m_ibh);
			bgfx::setVertexBuffer(m_vbh);

			//render polygon to stencil buffer
			bgfx::submit(m_viewId,
						 m_shaderColorBlack->getHandle());
		}

		//clearStencil();
	}

}
