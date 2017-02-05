#ifndef GRAPHICSOBJECT_H_HEADER_GUARD
#define GRAPHICSOBJECT_H_HEADER_GUARD

#include "Shader.hpp"
#include <common/math.hpp>
#include <utils/VertexUtils.hpp>
#include <string>
#include <Eigen/Dense>

namespace pumpkin {

class GraphicsObjectFactory;
	
class GraphicsObject {

public:

	friend class GraphicsObjectFactory;

	GraphicsObject();
	GraphicsObject(const std::string & config_file);
	
	~GraphicsObject();
	void init();
	
	virtual void update(float d);

	void destroyUniforms();
	void initialiseBuffers();
	void createUniforms();
	bool loadShader(uint32_t);

	void setTransform(const Eigen::MatrixXf & transform) {
		m_transform = transform;
	}

protected:
	
	uint16_t m_indices[6];
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	
	float m_height;
	float m_width;
	float m_scale;

	Eigen::MatrixXf m_transform;

	std::string m_vertex_shader;
	std::string m_fragment_shader;

	Shader::Ptr m_shader;
};

}
#endif
