#ifndef GRAPHICSOBJECT_H_HEADER_GUARD
#define GRAPHICSOBJECT_H_HEADER_GUARD

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include <common/math.hpp>
#include <utils/VertexUtils.hpp>
#include <utils/MeshFactory.hpp>
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
	GraphicsObject() : m_transform(Eigen::MatrixXf::Identity(4,4)) {

		// This should come from the config file,
		// or with a tailored constructor using the VertexUtils
		int16_t tsize = 0x7fff/4;
		m_width = 0.5;
		m_height = 0.5;

		m_mesh = VertexUtils::constructBulletLine(MeshProperties(0,0,1.0,0.1,0.1));

		addMeshToPool(m_mesh);
		loadShader(0);
	}

	GraphicsObject(const std::string & config_file) : m_transform(Eigen::MatrixXf(4,4)) {
		// MeshFactory<T> factory(config_file);
		// factory.generate(&m_mesh);
		// TODO put this back
		m_width = 0.5;
		m_height = 0.5;
		int16_t tsize = 0x7fff/4;
		std::vector<AtlasFrame> frames;
		AtlasFrame af;
		af.top_left = AtlasFrame::Corner(0,0);
		af.bottom_right = AtlasFrame::Corner(tsize,tsize);
		frames.push_back(af);

		m_mesh = VertexUtils::constructVPlane(MeshProperties(0,0,1.0,0.5,0.5,frames));
		addMeshToPool(m_mesh);
	}

	~GraphicsObject() {
		bgfx::destroyDynamicIndexBuffer(m_ibh);
		bgfx::destroyDynamicVertexBuffer(m_vbh);
	}

	virtual void init() {

		m_shader->init();

		initialiseBuffers();
		createUniforms();

		// debugging
		ddInit();
	}

	virtual void update(float d) {

		//Set render states.
		bgfx::setState(0
			       | BGFX_STATE_DEFAULT
			       | m_mesh.getType()
			       // | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
			       // 			       BGFX_STATE_BLEND_INV_SRC_ALPHA )
			);

		bgfx::setTransform(m_transform.data());
		bgfx::setVertexBuffer(m_vbh);
		bgfx::setIndexBuffer(m_ibh);


		// ddBegin(0);

		// float normal[4] = {
		// 	0.0, 0.0, -1.0, 0.0
		// };

		// float center[4] = {
		// 	-0.25, 0.0, 0.0, 1.0
		// };

		// ddPush();
		// //	ddSetTransform(m_transform.data());
		// ddMoveTo(m_transform.block(0,3,3,1).data());
		// // ddLineTo(center[0] + normal[0],
		// // 	 center[1] + normal[1],
		// // 	 center[2] + normal[2]);
		// float size = 0.5;
		// ddDrawQuad(normal, m_transform.block(0,3,3,1).data(), size);

		// ddPop();

		// Submit primitive for rendering to view 0.
		bgfx::submit(0, m_shader->getHandle());

	}

	virtual void createUniforms() {}
	virtual void destroyUniforms() {};

	void addMeshToPool(const Mesh<T> & mesh) {
		for (auto v : mesh.m_vertex_pool) {
			std::cout << v.m_x << "," << v.m_y << "," << v.m_z << "," << std::endl;
			m_vertexPool.push_back(v);
		}

		for (auto i : mesh.m_index_pool) {
			m_indices.push_back(i);
		}
	}

	virtual void initialiseBuffers() {

		assert(m_vertexPool.size() > 0);

		const bgfx::Memory* mem;
		mem = bgfx::makeRef(&m_vertexPool[0], sizeof(T)*m_vertexPool.size());
		m_vbh = bgfx::createDynamicVertexBuffer(mem, T::ms_decl);
		mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * m_indices.size() );
		m_ibh = bgfx::createDynamicIndexBuffer(mem);


		// m_vbh = bgfx::createVertexBuffer(
		// 	bgfx::makeRef(&m_vertexPool[0], sizeof(T)*m_vertexPool.size() )
		// 	, T::ms_decl
		// 	);

		// m_ibh = bgfx::createIndexBuffer(
		// 	bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * m_indices.size() )
		// 	);
	}

	bool loadShader(uint32_t id) {
		m_shader = ResourceManager::getResource<Shader>(id);
		if (!m_shader) {
			//log error
			std::cout << "error loading shader cfg" << std::endl;
			return false;
		}
		std::cout << "Loading shader" << std::endl;
		return true;
	}

	void setTransform(const Eigen::MatrixXf & transform) {
		m_transform = transform;
	}

protected:

	// bgfx::VertexBufferHandle m_vbh;
	// bgfx::IndexBufferHandle m_ibh;
	bgfx::DynamicVertexBufferHandle m_vbh;
	bgfx::DynamicIndexBufferHandle m_ibh;

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
