#pragma once

#include <Eigen/Dense>
#include "GraphicsEngine.hpp"
#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include <common/bgfx_utils.h>
#include <common/common.h>
#include <common/math.hpp>

namespace pumpkin {

enum class ParticleType {
	LINE_PARTICLE,
	CIRCLE_PARTICLE
};

class ParticleGraphicsObject {

public:
	ParticleGraphicsObject() {
//		bgfx::allocTransientVertexBuffer(&tvb, 4, PosTexCoord0Vertex::ms_decl);
	}

	void setPosition(const Eigen::Vector3f & p) {
		m_transform.block(0,3,1,3) = p;
	}

	void setRotation(const Eigen::MatrixXf & rot) {
		m_transform.block(0,0,3,3) = rot;
	}

	void setTransform(const Eigen::MatrixXf &t) {
		m_transform = t;
	}

	Eigen::Matrix3f getRotation() const {
		return m_transform.topLeftCorner(3,3);
	}

	Vec3f getLocation() const {
		return m_transform.col(3).head(3);
	}

	void update(float delta) {
		// whatever
	}

	Eigen::MatrixXf m_transform = Eigen::MatrixXf::Identity(4,4);
	float m_size = 1.0;
	//color
};


// Renders several particles in a single VBO
class ParticleRenderer {

public:

	ParticleRenderer(uint16_t type);

	// loads resources once available
	void init();

	void render(uint8_t view);
	uint16_t id() const { return m_particle_type; }

	void pushParticle(const ParticleGraphicsObject & p) {
		m_particles.push_back(p);
	}

private:

	std::vector<ParticleGraphicsObject> m_particles;

	uint16_t m_vertices_per_particle = 4;
	uint16_t m_indices_per_particle = 4;
	uint16_t m_particle_type;
	bgfx::UniformHandle s_texColor;
	ParticleType m_type;
//	bgfx::TransientVertexBuffer tvb;
//	bgfx::TransientIndexBuffer tib;
	TextureAtlas::Ptr   m_texture;
	Shader::Ptr         m_shader;


	// bgfx::DynamicVertexBufferHandle tvb;
	// bgfx::DynamicIndexBufferHandle tib;

};

}
