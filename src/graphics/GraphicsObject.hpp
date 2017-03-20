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

	//GraphicsObject() : m_transform(Eigen::MatrixXf::Identity(4,4)) {}
	GraphicsObject(const Config & config);

	~GraphicsObject() {
		for (auto mesh : m_mesh_vector)
			mesh.unload();
	}

	virtual void init() {
		// debugging
		ddInit();
	}

	void addMesh(const Mesh & mesh) {
		m_mesh_vector.push_back(mesh);
	}

	void meshSubmit() {
		for (int i = 0; i < m_mesh_vector.size(); i++) {			
			m_mesh_vector[i].submit(m_mesh_state_vector,
						(const float*)m_transform.data(),
						(uint16_t) 1);
		}


	// 	uint32_t cached = bgfx::setTransform((const float*)m_transform.data(),
// 						     1);
// 		bgfx::setTransform(cached, 1);




// 		// Set render states.
// 		bgfx::setState(m_mesh_state_vector[0].m_state);	

// //		bgfx::setTransform(m_transform.data());
// 		bgfx::setVertexBuffer(m_mesh_vector[0].m_groups[0].m_dvbh);
// 		bgfx::setIndexBuffer(m_mesh_vector[0].m_groups[0].m_dibh);
		
// 		bgfx::setTexture(m_mesh_state_vector[0].m_texture->m_stage[0],
// 				 m_mesh_state_vector[0].m_texture->u_sampler[0],
// 				 m_mesh_state_vector[0].m_texture->m_texture_handle[0]);

// 		// if (m_texture_atlas->hasNormalMap()) {
// 		// 	bgfx::setTexture(0, m_mesh_state_vector[0].m_texture->u_texNormal,
// 		// 			 m_mesh_state_vector[0].m_texture->getNormalHandle());
// 		// }

// 		m_mesh_state_vector[0].m_texture->setUniforms();

// 		// Submit primitive for rendering to view 0.
// 		bgfx::submit(m_mesh_state_vector[0].m_viewId,
// 			     m_mesh_state_vector[0].m_shader->getHandle(),
// 			     0, true);
	}
	
	virtual void update(float d) {
		
		//	meshSubmit();

	};
	
	bool loadShader(uint32_t id, uint8_t pass = 0) {

		assert(pass <= m_mesh_state_vector.size());
		
		Shader::Ptr shader = ResourceManager::getResource<Shader>(id);
		if (!shader) {
			//log error
			std::cout << "error loading shader cfg" << std::endl;
			return false;
		}

		std::cout << "Loaded shader" << id << std::endl;
		
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

		std::cout << "Loaded texture" << id << std::endl;

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
