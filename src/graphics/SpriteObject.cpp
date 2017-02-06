#include "SpriteObject.hpp"
#include "GraphicsEngine.hpp"
#include "SpriteObjectFactory.hpp"
#include <utils/MeshFactory.hpp>
#include <iostream>
#include <common/debugdraw/debugdraw.h>

namespace pumpkin {

// When we build Animation we do not call directly the constructor of
// GraphicObject because we cannot inherit settings with protocol buffers.
// A possible solution would be to pass to the constructors the
// protobuf structures rather than the config files!!!
SpriteObject::SpriteObject() :
	GraphicsObject("cfg"),
	m_flipped(0) {

	m_transform = Eigen::MatrixXf(4,4);
	m_atlas_offset[0] = 0.0;
	m_atlas_offset[1] = 0.0;
	m_atlas_offset[2] = 1.0/4.0;
}

SpriteObject::SpriteObject(const std::string & config_file) :
	GraphicsObject(config_file),
	m_flipped(0) {

	m_transform = Eigen::MatrixXf(4,4);
	SpriteObjectFactory factory(config_file);
	factory.generate(this);

	// sprite texture width: this comes from the grid of the atlas!
	// we pack it in the z coord of the atlas offset (vec4)
	// PUT IT IN FACTORY
	m_atlas_offset[0] = 0.0;
	m_atlas_offset[1] = 0.0;
	m_atlas_offset[2] = 1.0/4.0;
}

SpriteObject::~SpriteObject() {
	destroyUniforms();
}

void SpriteObject::destroyUniforms() {

	bgfx::destroyUniform(u_texOffset);
	bgfx::destroyUniform(u_flip);
	bgfx::destroyUniform(u_lightPosRadius);

	GraphicsObject::destroyUniforms();
}

void SpriteObject::init() {

	m_texture_atlas->init();
	GraphicsObject::init();
}


bool SpriteObject::loadTextureAtlas(uint32_t id) {
	m_texture_atlas = ResourceManager::getResource<TextureAtlas>(id);
	if (!m_texture_atlas) {
		//log error
		std::cout << "error loading texture atlas cfg" << std::endl;
		return false;
	}
	return true;
}

void SpriteObject::initialiseBuffers() {
	GraphicsObject::initialiseBuffers();
}

void SpriteObject::createUniforms() {

	u_texOffset = bgfx::createUniform("packed_input",  bgfx::UniformType::Vec4);
	u_flip = bgfx::createUniform("flip", bgfx::UniformType::Int1);
	u_lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, 1);
}

void SpriteObject::update(float d) {

	// Set render states.
	bgfx::setState(0
		       | BGFX_STATE_DEFAULT
		       | BGFX_STATE_PT_TRISTRIP
		       | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
					       BGFX_STATE_BLEND_INV_SRC_ALPHA )
		);


	float lightPosRadius[4] = { 4.0, 4.0, 1.0, 2.0};
	bgfx::setUniform(u_lightPosRadius, lightPosRadius, 1);

	Eigen::Vector3f at = GraphicsEngine::camera().getAt();

	Eigen::Vector3f axis = Eigen::Vector3f(0.0, 1.0, 0.0);

	bgfx::setTransform(m_transform.data());
	bgfx::setVertexBuffer(m_vbh);
	bgfx::setIndexBuffer(m_ibh);
	bgfx::setTexture(0, s_texColor,  m_texture_atlas->getColorHandle());

	bgfx::setUniform(u_texOffset, m_atlas_offset, 1);
	bgfx::setUniform(u_flip, &m_flipped, 1);

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_shader->getHandle());

	//TODO refactor to debug helper
	if (GraphicsEngine::debugEnabled()) {
		ddBegin(0);

		float normal[4] = {
			0.0, 0.0, -1.0, 0.0
		};

		float center[4] = {
			-0.25, 0.0, 0.0, 1.0
		};

		ddPush();
		ddSetTransform(m_transform.data());
		ddMoveTo(center);
		ddLineTo(center[0] + normal[0],
			 center[1] + normal[1],
			 center[2] + normal[2]);
		float size = 0.5;
		ddDrawQuad(normal, center, size);

		ddPop();

		ddEnd();
	}
}

}
