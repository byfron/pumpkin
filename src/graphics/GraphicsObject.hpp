#ifndef GRAPHICSOBJECT_H_HEADER_GUARD
#define GRAPHICSOBJECT_H_HEADER_GUARD

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include <utils/VertexUtils.hpp>
#include <string>
#include <Eigen/Dense>

namespace pumpkin {

class GraphicsObjectFactory;
class AnimationFactory;	
	
class GraphicsObject {

public:

	friend class GraphicsObjectFactory;
	friend class AnimationFactory;

	GraphicsObject();
	GraphicsObject(const std::string & config_file);
	
	~GraphicsObject();
	void init();
	
	void update(float d);

	void destroyUniforms();
	void initialiseBuffers();
	void createUniforms();
	bool loadTextureAtlas(uint32_t);
	bool loadShader(uint32_t);
	       
	static Eigen::MatrixXf getTowardsCameraRotation(Vec3f);
	
	void setPosition(const Eigen::Vector3f & p) {
		m_position = p;
	}

	void setRotation(const Eigen::MatrixXf & rot) {
		m_rotation = rot;
	}

protected:

	
	uint16_t m_indices[6];
	bgfx::UniformHandle s_texColor;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	
	float m_height;
	float m_width;
	float m_pos_x;
	float m_pos_y;
	float m_scale;
	float m_sprite_width;

	bgfx::UniformHandle u_flip;
	bgfx::UniformHandle u_texOffset;
	bgfx::UniformHandle u_lightPosRadius;

	int m_flipped;
	float m_atlas_offset[4];
	Eigen::Vector3f m_position;
	Eigen::MatrixXf m_rotation;
	Eigen::MatrixXf m_transform;

	std::string m_texture;
	std::string m_vertex_shader;
	std::string m_fragment_shader;

	TextureAtlas::Ptr m_texture_atlas;
	Shader::Ptr m_shader;
};

}
#endif
