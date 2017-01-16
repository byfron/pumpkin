#ifndef ENTITY2D_H_HEADER_GUARD
#define ENTITY2D_H_HEADER_GUARD

#include <engine/ResourceManager.hpp>
#include <utils/VertexUtils.hpp>
#include <Eigen/Geometry>

class Entity2D : public Resource {

public:

	Entity2D();
	~Entity2D();
	void init();
	void update(float d);

	virtual void getAtlasOffset(float *offset) {
		offset[0] = 0;
		offset[1] = 0;
	}
	
	Eigen::Affine3f getTowardsCameraRotation();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

protected:

	Eigen::Matrix4f m_transform;
	
	uint16_t m_indices[6];
	bgfx::UniformHandle s_texColor;
	bgfx::TextureHandle m_textureColor;
	bgfx::ProgramHandle m_program;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
//	float m_transform[16];

	Eigen::Vector3f m_pos;
	
	float m_height;
	float m_width;
	float m_pos_x;
	float m_pos_y;

	bgfx::UniformHandle u_texOffset;
	
	
};


#endif
