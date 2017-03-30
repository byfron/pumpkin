//
// Created by rubio on 27/03/17.
//

#include "Stencil.hpp"
#include "DebugManager.hpp"
#include <clip2tri/clip2tri.h>

using namespace c2t;

namespace pumpkin {

	namespace {

		void makeTriangleStrip(const std::vector<Vec2f> & polygon,
				       const Vec2f & player_pos,
				       std::vector<Vec3f> & triangles,
				       std::vector<uint16_t> & indices) {


			indices.clear();
			triangles.clear();
			uint16_t vertex_count = 0;

			if (polygon.size() == 0) return;

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

				triangles.push_back(Vec3f(outputTriangles[i].x,
										 outputTriangles[i].y,
										 0.0f));

				triangles.push_back(Vec3f(outputTriangles[i+1].x,
										 outputTriangles[i+1].y,
										 0.0f));

				triangles.push_back(Vec3f(outputTriangles[i+2].x,
										 outputTriangles[i+2].y,
										 0.0f));

				std::vector<Vec2f> poly;
				poly.push_back(Vec2f(outputTriangles[i].x,
									 outputTriangles[i].y));
				poly.push_back(Vec2f(outputTriangles[i+1].x,
									 outputTriangles[i+1].y));
				poly.push_back(Vec2f(outputTriangles[i+2].x,
									 outputTriangles[i+2].y));

				pumpkin::DebugManager::push_polygon(poly);

				indices.push_back(i);
				indices.push_back(i+1);
				indices.push_back(i+2);
			}


			return;
		}


		// 	// sort points by angle
		// 	typedef std::pair<Vec2f, float> AnglePoly;
		// 	std::vector<AnglePoly> sorted_poly;
		// 	for (size_t i = 0; i < polygon.size(); i++) {

		// 		Vec2f vec = polygon[i] - player_pos;
		// 		vec.normalize();
		// 		float d = vec.dot(Vec2f(0.0, 1.0));
		// 		sorted_poly.push_back(AnglePoly(polygon[i], d));
		// 	}

		// 	std::sort(sorted_poly.begin(), sorted_poly.end(),
		// 		  [](auto &left, auto &right) {
		// 			  return left.second < right.second;
		// 		  });

		// 	for (size_t i = 0; i < sorted_poly.size()-1; i++) {

		// 		Vec2f point1 = sorted_poly[i].first;
		// 		Vec2f point2 = sorted_poly[i+1].first;

		// 		indices.push_back(vertex_count);
		// 		indices.push_back(vertex_count+1);
		// 		indices.push_back(vertex_count+2);
		// 		vertex_count += 3;
		// 		triangles.push_back(Vec3f(point1(0), point1(1), 0.0f));
		// 		triangles.push_back(Vec3f(player_pos(0), player_pos(1), 0.0f));
		// 		triangles.push_back(Vec3f(point2(0), point2(1), 0.0f));
		// 	}
		// }


	}

	Stencil::Stencil() {

        m_vbh.idx = bgfx::invalidHandle;
        m_ibh.idx = bgfx::invalidHandle;

	m_viewId = RENDER_PASS_GEOMETRY;//RENDER_PASS_MAKE_STENCIL;

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



        // static PosColorVertex s_vertices[] =
        //         {
        //                 {-2.0f, -5.0f,  0.0f, 0x00000000 },
        //                 {-2.0f,  5.0f,  0.0f, 0x00000000 },
        //                 { 2.0f, -5.0f,  0.0f, 0x00000000 },
        //                 { 2.0f,  5.0f,  0.0f, 0x00000000 },

        //         };

        // static const uint16_t s_triList[] =
        //         {
        //                 0, 1, 2, 3
        //         };

        // m_dvbh =  bgfx::createDynamicVertexBuffer(bgfx::makeRef(s_vertices, sizeof(s_vertices) ),
        //                                           PosColorVertex::ms_decl);

        // m_dibh = bgfx::createDynamicIndexBuffer(bgfx::makeRef(s_triList, sizeof(s_triList) ) );
	}

	void Stencil::updateDynamicVertexBuffer() {

		// fill vertices with triangles

		// static PosColorVertex s_vertices[] =
		// 	{
		// 		{-15.0f, -5.0f,  0.0f, 0x00000000 },
		// 		{-15.0f,  5.0f,  0.0f, 0x00000000 },
		// 		{ 15.0f,  5.0f,  0.0f, 0x00000000 },
		// 		{ 15.0f, -5.0f,  0.0f, 0x00000000 },
		// 	};

		// static const uint16_t s_triList[] =
		// 	{
		// 		0, 1, 2, 3// 0
		// 	};


		const bgfx::Memory* mem;

		if (bgfx::isValid(m_vbh) )
		{
			bgfx::destroyVertexBuffer(m_vbh);
		}

		mem = bgfx::makeRef(&m_vertices[0], sizeof(PosColorVertex) * m_vertices.size());
		m_vbh = bgfx::createVertexBuffer(mem, PosColorVertex::ms_decl);

		if (bgfx::isValid(m_ibh) )
		{
			bgfx::destroyIndexBuffer(m_ibh);
		}

		mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * m_indices.size());
		m_ibh = bgfx::createIndexBuffer(mem);

	}

	void Stencil::clearStencil() {

		bgfx::setViewClear(m_viewId,
				   BGFX_CLEAR_STENCIL,
				   (uint16_t) 0,   //rgba
				   0.0f,           //depth
				   (uint8_t) 0);   //stencil
	}

	void Stencil::craftStencil(const std::vector<Vec2f> & polygon,
				   const Vec2f & player_pos) {


		//paint polygon in stencil buffer
		makeTriangleStrip(polygon, player_pos, m_vertices, m_indices);
		updateDynamicVertexBuffer();

		// Eigen::MatrixXf transform = Eigen::MatrixXf::Identity(4,4);
		// bgfx::setTransform(transform.data());

		// bgfx::setStencil(m_craftStencilState.m_fstencil, m_craftStencilState.m_bstencil);
		// bgfx::setState(m_craftStencilState.m_state, m_craftStencilState.m_blendFactorRgba);

		// bgfx::setIndexBuffer(m_ibh);
		// bgfx::setVertexBuffer(m_vbh);

		// // render polygon to stencil buffer
		// bgfx::submit(m_viewId,
		// 	     m_shaderColorBlack->getHandle());

		//clearStencil();


//        pumpkin::DebugManager::push_polygon(m_polygon);
	}

}
