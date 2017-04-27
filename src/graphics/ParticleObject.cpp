#include "ParticleObject.hpp"
#include <utils/VertexUtils.hpp>

namespace pumpkin {

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
	
	void ParticleRenderer::render(uint8_t _view,
								  const std::vector<ParticleGraphicsObject> & particles) {

	if (particles.size() > 0) {

		std::cout << uint32_t(particles.size() * m_vertices_per_particle) << std::endl;
	

		const uint32_t numVertices =
			bgfx::getAvailTransientVertexBuffer(uint32_t(particles.size() * m_vertices_per_particle),
												PosColorTexCoordVertex::ms_decl);
		const uint32_t numIndices  =
			bgfx::getAvailTransientIndexBuffer(uint32_t(particles.size() * m_indices_per_particle));
		const uint32_t max = std::min(numVertices/m_vertices_per_particle,
									  numIndices/m_indices_per_particle);

		//fill VBO and paint. why a transient?
		bgfx::allocTransientBuffers(&tvb
									, PosColorTexCoordVertex::ms_decl
									, max * m_vertices_per_particle
									, &tib
									, max * m_indices_per_particle);

		PosColorTexCoordVertex* vertices = (PosColorTexCoordVertex*)tvb.data;
		uint16_t* indices = (uint16_t*)tib.data;

		uint32_t color = toAbgr(1.0, 0.0, 0.0, 0.5);
		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Int1);
	
		// Only for line particles
		Vec3f line = Vec3f(0.0, 1.0, 0.0);
	
		for (int i = 0; i < particles.size(); i++) {
		
			float size = particles[i].m_size;
			Eigen::Matrix3f R = particles[i].getRotation();
			Vec3f loc = particles[i].getLocation() - R * line * size/2.0;

			//vertices
			PosColorTexCoordVertex* vertex = (PosColorTexCoordVertex*) &tvb.data[i*m_vertices_per_particle];
			vertex->m_x = loc(0);
			vertex->m_y = loc(1);
			vertex->m_z = loc(2);	
			vertex->m_abgr = color;
			vertex->m_u = 0.0;
			vertex->m_v = 0.0;
			vertex->m_blend = 1.0;
			++vertex;

			loc = particles[i].getLocation() + R * line * size/2.0;
		
			vertex->m_x = loc(0);
			vertex->m_y = loc(1);
			vertex->m_z = loc(2);	
			vertex->m_abgr = color;
			vertex->m_u = 0.0;
			vertex->m_v = 1.0;
			vertex->m_blend = 1.0;
			++vertex;

			//indices
			uint16_t *index = &indices[i];
			index[0] = i*m_vertices_per_particle;
			index[1] = i*m_vertices_per_particle + 1;
		}

		bgfx::setState(0
					   | BGFX_STATE_RGB_WRITE
					   | BGFX_STATE_ALPHA_WRITE
					   | BGFX_STATE_DEPTH_TEST_LESS
					   | BGFX_STATE_CULL_CW
					   | BGFX_STATE_BLEND_NORMAL
			);
		bgfx::setVertexBuffer(&tvb);
		bgfx::setIndexBuffer(&tib);
		bgfx::setTexture(0, s_texColor, m_texture->getColorHandle());
		bgfx::submit(_view, m_shader->getHandle());
	}
}

}
