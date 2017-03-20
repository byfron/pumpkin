 #include "GraphicsObject.hpp"

namespace pumpkin {

bool loadShader(MeshState *pass, uint32_t id) {
	Shader::Ptr shader = ResourceManager::getResource<Shader>(id);
	if (!shader) {
		//log error
		std::cout << "error loading shader cfg" << std::endl;
		return false;
	}

	shader->init();
	pass->m_shader = shader;
	return true;
}

bool loadTexture(MeshState *pass, uint32_t id) {
	TextureAtlas::Ptr texture = ResourceManager::getResource<TextureAtlas>(id);
	if (!texture) {
		//log error
		std::cout << "error loading texture atlas cfg" << std::endl;
		return false;
	}

	texture->init();
	pass->m_texture = texture;
	return true;
}

}
