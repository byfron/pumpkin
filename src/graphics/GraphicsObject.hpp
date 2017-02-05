#ifndef GRAPHICSOBJECT_H_HEADER_GUARD
#define GRAPHICSOBJECT_H_HEADER_GUARD

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include <common/math.hpp>
#include <utils/VertexUtils.hpp>
#include <string>
#include <Eigen/Dense>
#include <common/debugdraw/debugdraw.h>

namespace pumpkin {

class GraphicsObjectFactory;

template <typename T = PosNormalTangentTexcoordVertex>
class GraphicsObject {

public:

	friend class GraphicsObjectFactory;

	
// When we build Animation we do not call directly the constructor of
// GraphicObject because we cannot inherit settings with protocol buffers.
// A possible solution would be to pass to the constructors the
// protobuf structures rather than the config files!!!
	GraphicsObject() : m_transform(Eigen::MatrixXf(4,4)) {
	}
	
	GraphicsObject(const std::string & config_file) : m_transform(Eigen::MatrixXf(4,4)) {
	}
	
	~GraphicsObject() {
	}
	
	virtual void init() {

		m_shader->init();

		initialiseBuffers();
		createUniforms();

		// debugging
		ddInit();
	}
		
	virtual void update(float d) {
		
		// Set render states.
		bgfx::setState(0
			       | BGFX_STATE_DEFAULT
			       | BGFX_STATE_PT_TRISTRIP
			       | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
						       BGFX_STATE_BLEND_INV_SRC_ALPHA )
			);

		bgfx::setTransform(m_transform.data());
		bgfx::setVertexBuffer(m_vbh);
		bgfx::setIndexBuffer(m_ibh);

		// Submit primitive for rendering to view 0.
		bgfx::submit(0, m_shader->getHandle());
	}
	
	virtual void createUniforms() {}
	virtual void destroyUniforms() {}; 	

	void addMeshToPool(const Mesh<T> & mesh) {
		int starting_idx = m_vertexPool.size();
		for (auto v : mesh.m_vertex_pool) {
			m_vertexPool.push_back(v);
		}
		
		for (auto i : mesh.m_index_pool) {
			m_indices.push_back(i + starting_idx);
		}
	}

	virtual void initialiseBuffers() {

		// This can also come from config
		int16_t tsize = 0x7fff/4;		
		std::vector<AtlasFrame> frames;
		AtlasFrame af;
		af.top_left = AtlasFrame::Corner(0,0);
		af.bottom_right = AtlasFrame::Corner(tsize,tsize);
		frames.push_back(af);
		
		m_mesh = VertexUtils::constructVPlane(MeshProperties(0,0,1.0,0.5,0.5,frames));
		addMeshToPool(m_mesh);
	
		// Create static vertex buffer.
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(&m_vertexPool[0], sizeof(T)*m_vertexPool.size() )
			, T::ms_decl
			);

		// Create static index buffer.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * m_indices.size() )
			);
	}
	
	bool loadShader(uint32_t id) {
		m_shader = ResourceManager::getResource<Shader>(id);
		std::cout << "Loading shader" << std::endl;
		if (!m_shader) {
			//log error
			std::cout << "error loading shader cfg" << std::endl;
			return false;
		}
		return true;
	}

	void setTransform(const Eigen::MatrixXf & transform) {
		m_transform = transform;
	}

protected:
	
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	
	float m_height;
	float m_width;
	float m_scale;

	Mesh<T> m_mesh;
	std::vector<T> m_vertexPool;
	std::vector<uint16_t> m_indices;
	
	Eigen::MatrixXf m_transform;

	std::string m_vertex_shader;
	std::string m_fragment_shader;

	Shader::Ptr m_shader;
};

}
#endif
