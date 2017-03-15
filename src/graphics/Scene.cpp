#include "Scene.hpp"
#include <utils/FbxLoader.hpp>

namespace pumpkin {

SceneObject::SceneObject() {

}

void SceneObject::loadFromFbxFile(std::string file) {
	FbxLoader loader(file);
	m_meshes = loader.loadMeshes();
}

void SceneObject::init() {

	for (auto mesh : m_meshes) {
		addMeshToPool(mesh);
	}

	loadShader(0);
	m_shader->init();

	initialiseBuffers();
	createUniforms();
}

void SceneObject::update(float d) {

	// Set render states.
	bgfx::setState(0
				   | BGFX_STATE_DEFAULT);

	bgfx::setTransform(m_transform.data());
	bgfx::setVertexBuffer(m_vbh);
	bgfx::setIndexBuffer(m_ibh);
//	bgfx::setTexture(0, u_texColor,  m_scene_textures[]);

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_shader->getHandle());
}

}
