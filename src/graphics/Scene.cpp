#include "Scene.hpp"

namespace pumpkin {

Scene::Scene(const Scene::Config & config)
{
}

void Scene::loadFromFbxNode(FbxNode * node) {

	uint64_t state = 0 | BGFX_STATE_DEFAULT;


	std::vector<Mesh> meshes = loadMeshes(node);
	std::vector<TextureAtlas::Ptr> textures = loadTextures(node);

	assert(meshes.size() == textures.size());

	//create a single pass for each mesh
	for (int i = 0; i < meshes.size(); i++) {
		std::vector<MeshState> passes;
		MeshState pass0;
		pass0.m_state = state;
		loadShader(&pass0, 5);
 		pass0.m_texture = textures[i];
		passes.push_back(pass0);
		addPrefab(passes, meshes[i]);
	}
}

void Scene::update(float d) {

	float view[16];
	Eigen::AngleAxis<float> aa(3.1416, Vec3f(0.0,1.0,0.0));
	Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
	rot.block(0,0,3,3) = aa.matrix();
}

void Scene::render(float d) {
	meshSubmit();
}

}
