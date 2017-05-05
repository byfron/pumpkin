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
		m_shader = ResourceManager::getResource<Shader>(0); //load default shader
			m_shader->init();
	}
	
	
	void ParticleRenderer::render(uint8_t _view,
								  const std::vector<ParticleGraphicsObject> & particles) {

	if (particles.size() > 0) {


		// TODO try dynamic vertex buffer
		
		bgfx::TransientVertexBuffer tvb;
		bgfx::TransientIndexBuffer tib;
		
		const uint32_t numVertices = bgfx::getAvailTransientVertexBuffer(
			particles.size()*4,
			PosColorVertex::ms_decl);
		
		const uint32_t numIndices = bgfx::getAvailTransientIndexBuffer(particles.size()*4);
		
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
			
		for (int i = 0; i < max; i++) {
		
			float size = particles[i].m_size;
			Eigen::Matrix3f R = particles[i].getRotation();
			
			QuadParticle qpart(0.1);
			
			uint16_t *index = &indices[i*4];
			PosColorVertex* vertex = (PosColorVertex*) &tvb.data[i*4];
			
			std::vector<Vec2f> poly;
			
			for (int j = 0; j < 4; j++) {
				
//				Vec3f loc = particles[i].getLocation() + R * qpart.getVertex(j);
				Vec3f loc = qpart.getVertex(j);
				
				poly.push_back(loc.head(2));				
				
				//vertices
				vertex->m_x = loc(0);
				vertex->m_y = loc(1);
				vertex->m_z = loc(2) + 0.1;
				vertex->m_abgr = color;
				++vertex;
				
//				std::cout << loc << std::endl;
			}
			
			index[0] = i*4 + 0;
			index[1] = i*4 + 1;
			index[2] = i*4 + 2;
			index[3] = i*4 + 3;
			
			DebugManager::push_polygon(poly);
		}
		
		// bgfx::setState(0
		// 				| BGFX_STATE_RGB_WRITE
		// 				| BGFX_STATE_ALPHA_WRITE
		// 				| BGFX_STATE_DEPTH_TEST_LESS
		// 				| BGFX_STATE_CULL_CW
		// 				| BGFX_STATE_BLEND_NORMAL
		// 				);
		
		bgfx::setState(0 |
					   BGFX_STATE_DEFAULT |
					   BGFX_STATE_PT_TRISTRIP);
		
		// 			   // | BGFX_STATE_RGB_WRITE
		// 			   // | BGFX_STATE_ALPHA_WRITE
		// 			   // | BGFX_STATE_DEPTH_TEST_LESS
		// 			   // | BGFX_STATE_CULL_CW
		// 			   // | BGFX_STATE_BLEND_NORMAL
//			);
		bgfx::setVertexBuffer(&tvb);
		bgfx::setIndexBuffer(&tib);
		
		bgfx::submit(_view, m_shader->getHandle());
	}
	}
}
