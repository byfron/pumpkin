#include "ParticleObject.hpp"
#include "DebugManager.hpp"
#include <utils/VertexUtils.hpp>

namespace pumpkin {

	struct QuadParticle {

	public:

		QuadParticle(float scale) {
			m_vertices = {
				Vec3f(-1.0,-1.0, 0.0) * scale,
				Vec3f(-1.0, 1.0, 0.0) * scale,
				Vec3f( 1.0, 1.0, 0.0) * scale,
				Vec3f( 1.0,-1.0, 0.0) * scale
			};
		}

		Vec3f getVertex(int i) {
			assert(i < 4);
			return m_vertices[i];
		}

		std::vector<Vec3f> m_vertices;
	};


	inline uint32_t toAbgr(const float r, const float g,
						   const float b, const float a)
	{
		return 0
			| (uint8_t(r*255.0f)<< 0)
			| (uint8_t(g*255.0f)<< 8)
			| (uint8_t(b*255.0f)<<16)
			| (uint8_t(a*255.0f)<<24)
			;
	}

	ParticleRenderer::ParticleRenderer(uint16_t type) : m_particle_type(type) {
	}

	void ParticleRenderer::init() {

		m_shader = ResourceManager::getResource<Shader>(0); //load default shader
			m_shader->init();
	}

	void ParticleRenderer::render(uint8_t _view) {

	if (m_particles.size() > 0) {

		std::cout << "rendering " << m_particles.size() << " particles" << std::endl;
		int num_particles = m_particles.size();


		// TODO try dynamic vertex buffer

		bgfx::TransientVertexBuffer tvb;
		bgfx::TransientIndexBuffer tib;

		const uint32_t numVertices = bgfx::getAvailTransientVertexBuffer(
			num_particles*4,
			PosColorVertex::ms_decl);

		const uint32_t numIndices = bgfx::getAvailTransientIndexBuffer(num_particles*4);
		const uint32_t max = bx::uint32_min(numVertices/4, numIndices/4);

		//fill VBO and paint. why a transient?
		if (max > 0) {
			if (not bgfx::allocTransientBuffers( &tvb
												 ,PosColorVertex::ms_decl
												 ,max*4
												 ,&tib
												 ,max*4)) {
				return;
			}
		}

		PosColorVertex* vertices = (PosColorVertex*)tvb.data;
		uint16_t* indices = (uint16_t*)tib.data;

		uint32_t color = toAbgr(0.0, 1.0, 0.0, 1.0);

		std::vector<PosColorVertex> vertex_data(num_particles*4);
		std::vector<uint16_t> index_data(num_particles*4);


		int i = 0;
		while(not m_particles.empty()) {

			ParticleGraphicsObject particle = m_particles.back();

			float size = particle.m_size;
			Eigen::Matrix3f R = particle.getRotation();

			QuadParticle qpart(0.1);

			uint16_t *index = &indices[i*4];
//			uint16_t *index = &index_data[i*4];
			PosColorVertex* vertex = (PosColorVertex*) &tvb.data[i*4];
//			PosColorVertex* vertex = (PosColorVertex*) &vertex_data[i*4];

			std::vector<Vec2f> poly;

			for (int j = 0; j < 4; j++) {

				Vec3f loc = particle.getLocation() + qpart.getVertex(j);
				poly.push_back(loc.head(2));

				//vertices
				vertex->m_x = loc(0);
				vertex->m_y = loc(1);
				vertex->m_z = loc(2) - 1.4;
				vertex->m_abgr = color;
				++vertex;

//				std::cout << loc << std::endl;
			}

			index[0] = i*4 + 3;
			index[1] = i*4 + 2;
			index[2] = i*4 + 1;
			index[3] = i*4 + 0;

			DebugManager::push_polygon(poly);

			m_particles.pop_back();
			i++;
		}

		// std::cout << "built" << std::endl;

		// if (bgfx::isValid(tvb) )
		// {
		// 	bgfx::destroyDynamicVertexBuffer(tvb);
		// }

		// if (bgfx::isValid(tib) )
		// {
		// 	bgfx::destroyDynamicIndexBuffer(tib);
		// }


		// tvb = bgfx::createDynamicVertexBuffer(
		// 	bgfx::makeRef(&vertex_data[0], sizeof(PosColorVertex) *num_particles*4),
		// 	PosColorVertex::ms_decl);

		// tib = bgfx::createDynamicIndexBuffer(
		// 	bgfx::makeRef(&index_data[0], sizeof(uint16_t) * num_particles*4));


		// Eigen::MatrixXf ident_t = Eigen::MatrixXf::Identity(4,4);
	// 	bgfx::setTransform(ident_t.data());

// 		// bgfx::setState(0
// 		// 				| BGFX_STATE_RGB_WRITE
// 		// 				| BGFX_STATE_ALPHA_WRITE
// 		// 				| BGFX_STATE_DEPTH_TEST_LESS
// 		// 				| BGFX_STATE_CULL_CW
// 		// 				| BGFX_STATE_BLEND_NORMAL
// 		// 				);

		uint64_t state = 0 |
			BGFX_STATE_DEFAULT |
			BGFX_STATE_PT_TRISTRIP;
	// |
	// 		BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
	// 							  BGFX_STATE_BLEND_INV_SRC_ALPHA);

		bgfx::setState(state);

// 		// 			   // | BGFX_STATE_RGB_WRITE
// 		// 			   // | BGFX_STATE_ALPHA_WRITE
// 		// 			   // | BGFX_STATE_DEPTH_TEST_LESS
// 		// 			   // | BGFX_STATE_CULL_CW
// 		// 			   // | BGFX_STATE_BLEND_NORMAL
//			);
		bgfx::setVertexBuffer(&tvb);
		bgfx::setIndexBuffer(&tib);

		bgfx::submit(_view, m_shader->getHandle());

		std::cout << "DONE" << std::endl;
	}
	}
}
