#ifndef ENTITY2D_H_HEADER_GUARD
#define ENTITY2D_H_HEADER_GUARD

#include <engine/ResourceManager.hpp>
#include <utils/VertexUtils.hpp>

class Entity2D : public Resource {

public:

	Entity2D(const std::string & texture,
		 const std::string & shader,
		 float w, float h);
	
	~Entity2D();
	void init();
	void update(float d);
	
	Eigen::Affine3f getTowardsCameraRotation();
	
	void setPosition(const Eigen::Vector3f & p) {
		m_position = p;
	}

protected:

	
	uint16_t m_indices[6];
	bgfx::UniformHandle s_texColor;
	bgfx::TextureHandle m_textureColor;
	bgfx::ProgramHandle m_program;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
//	float m_transform[16];

	
	float m_height;
	float m_width;
	float m_pos_x;
	float m_pos_y;
	float m_sprite_width;

	bgfx::UniformHandle u_flip;
	bgfx::UniformHandle u_texOffset;
	bgfx::UniformHandle u_lightPosRadius;

	int m_flipped;
	float m_atlas_offset[4];
	Eigen::Vector3f m_position;

	Eigen::MatrixXf m_transform;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
};


#endif
