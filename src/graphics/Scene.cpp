#include "Scene.hpp"

namespace pumpkin {

SceneObject::SceneObject() {

}

void SceneObject::loadFromFbxNode(FbxNode * node) {
	m_meshes = loadMeshes(node);
}

void SceneObject::destroyUniforms() {
	bgfx::destroyUniform(u_texColor);
}

void SceneObject::createUniforms() {       
	u_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
}
	
void SceneObject::init() {

	for (auto mesh : m_meshes) {
		addMeshToPool(mesh);		
	}

	loadShader(5);
	m_shader->init();

	m_texture = ResourceManager::getResource<TextureAtlas>(5);	
	if (!m_texture) {
		//log error
		std::cout << "error loading texture atlas cfg" << std::endl;
	}
	m_texture->init();
	
	initialiseBuffers();
	createUniforms();
}

void SceneObject::update(float d) {

	// Set render states.
	bgfx::setState(0
		       | BGFX_STATE_DEFAULT);

	float view[16];

	Eigen::AngleAxis<float> aa(3.1416, Vec3f(0.0,1.0,0.0));
	Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
	rot.block(0,0,3,3) = aa.matrix();
	bgfx::setTransform(rot.data());
	
	bgfx::setVertexBuffer(m_vbh);
	bgfx::setIndexBuffer(m_ibh);
	bgfx::setTexture(0, u_texColor,  m_texture->getColorHandle());

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_shader->getHandle());
}

}
