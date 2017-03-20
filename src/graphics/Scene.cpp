#include "Scene.hpp"

namespace pumpkin {

Scene::Scene(const Scene::Config & config)
{
}

void Scene::loadFromFbxNode(FbxNode * node) {

	uint64_t state = 0 | BGFX_STATE_DEFAULT;


	std::vector<Mesh> meshes = loadMeshes(node);
	std::vector<TextureAtlas> textures = loadTextures(node);

	assert(meshes.size() == textures.size());

	//create a single pass for each mesh
	for (int i = 0; i < meshes.size(); i++) {

		std::vector<MeshState> passes;
		MeshState pass0;
		pass0.m_state = state;
		loadShader(&pass0, 5);
		loadTexture(&pass0, 5);
//		pass0.m_texture = std::make_shared<TextureAtlas>(textures[i]);
		passes.push_back(pass0);

		addPrefab(passes, meshes[i]);
	}
}

void Scene::update(float d) {

	float view[16];
	Eigen::AngleAxis<float> aa(3.1416, Vec3f(0.0,1.0,0.0));
	Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
	rot.block(0,0,3,3) = aa.matrix();
//	m_transform = rot;
	meshSubmit();

	//bgfx::setVertexBuffer(m_vbh);
	//bgfx::setIndexBuffer(m_ibh);
	//bgfx::setTexture(0, u_texColor,  m_texture->getColorHandle());

	// Submit primitive for rendering to view 0.
	//bgfx::submit(0, m_shader->getHandle());
}

}
