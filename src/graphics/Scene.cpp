#include "Scene.hpp"

namespace pumpkin {

Scene::Scene(const Scene::Config & config) :
	GraphicsObject(config.graphics_properties()) {

}

void Scene::loadFromFbxNode(FbxNode * node) {
	m_mesh_vector = loadMeshes(node);
}
	
void Scene::update(float d) {

	float view[16];
	Eigen::AngleAxis<float> aa(3.1416, Vec3f(0.0,1.0,0.0));
	Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
	rot.block(0,0,3,3) = aa.matrix();
	m_transform = rot;
        meshSubmit();
	
	//bgfx::setVertexBuffer(m_vbh);
	//bgfx::setIndexBuffer(m_ibh);
	//bgfx::setTexture(0, u_texColor,  m_texture->getColorHandle());

	// Submit primitive for rendering to view 0.
	//bgfx::submit(0, m_shader->getHandle());
}

}
