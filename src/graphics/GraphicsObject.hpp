#ifndef GRAPHICSOBJECT_H_HEADER_GUARD
#define GRAPHICSOBJECT_H_HEADER_GUARD

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "GraphicsEngine.hpp"
#include <common/math.hpp>
#include <utils/VertexUtils.hpp>
#include <utils/MeshFactory.hpp>
#include <string>
#include <Eigen/Dense>
#include <common/debugdraw/debugdraw.h>
#include <graphics.pb.h>

namespace pumpkin {

class GraphicsObject {

public:

	typedef voyage::GraphicsObjectCfg Config;

	friend class GraphicsObjectFactory;

	GraphicsObject(const Config & config);

	~GraphicsObject() {
		for (auto mesh : m_mesh_vector)
			mesh.unload();
	}

	virtual void init() {
		// debugging
		ddInit();
	}

	void meshSubmit() {
		for (int i = 0; i < m_mesh_vector.size(); i++) {
			m_mesh_vector[i].submit(m_mesh_state_vector,
						(const float*)m_transform.data(),
						(uint16_t) 1);
		}
	}
	
	virtual void update(float d) {};
	
	bool loadShader(uint32_t id, uint8_t pass = 0) {

		assert(pass <= m_mesh_state_vector.size());
		
		Shader::Ptr shader = ResourceManager::getResource<Shader>(id);
		if (!shader) {
			//log error
			std::cout << "error loading shader cfg" << std::endl;
			return false;
		}

		shader->init();
		m_mesh_state_vector[pass].m_shader = shader;
		return true;
	}

	bool loadTexture(uint32_t id, uint8_t pass = 0) {
		TextureAtlas::Ptr texture = ResourceManager::getResource<TextureAtlas>(id);
		if (!texture) {
			//log error
			std::cout << "error loading texture atlas cfg" << std::endl;
			return false;
		}

		texture->init();
		m_mesh_state_vector[pass].m_texture = texture;
		return true;	
	}

	void setTransform(const Eigen::MatrixXf & transform) {
		m_transform = transform;
	}

protected:

	std::vector<MeshState> m_mesh_state_vector;
	std::vector<Mesh> m_mesh_vector;
	Eigen::MatrixXf m_transform;
};

}
#endif
