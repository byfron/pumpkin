#include "GraphicsObject.hpp"
#include "GraphicsEngine.hpp"
#include "GraphicsObjectFactory.hpp"
#include <utils/MeshFactory.hpp>
#include <iostream>
#include <common/debugdraw/debugdraw.h>

namespace pumpkin {

// When we build Animation we do not call directly the constructor of
// GraphicObject because we cannot inherit settings with protocol buffers.
// A possible solution would be to pass to the constructors the
// protobuf structures rather than the config files!!!
GraphicsObject::GraphicsObject() :
		m_transform(Eigen::MatrixXf(4,4)){
}

GraphicsObject::GraphicsObject(const std::string & config_file) :
	m_transform(Eigen::MatrixXf(4,4)) {
}

GraphicsObject::~GraphicsObject() {
	destroyUniforms();
}

void GraphicsObject::destroyUniforms() {
}

void GraphicsObject::init() {

	m_shader->init();

	initialiseBuffers();
	createUniforms();

	// debugging
	ddInit();
}

bool GraphicsObject::loadShader(uint32_t id) {
	m_shader = ResourceManager::getResource<Shader>(id);
	std::cout << "Loading shader" << std::endl;
	if (!m_shader) {
		//log error
		std::cout << "error loading shader cfg" << std::endl;
		return false;
	}
	return true;
}

void GraphicsObject::initialiseBuffers() {

		// size of a single sprite. TODO: This depends on the grid!!
	int16_t tsize = 0x7fff/4;

	static PosNormalTangentTexcoordVertex s_playerVertices[] =
	{
		// Horizonally aligned
		{  0.0f,      m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, tsize, tsize },
		{  0.0f,     -m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, 0, tsize },
		{ -m_height,  m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, tsize, 0 },
		{ -m_height, -m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, 0, 0 }

	};

	static const uint16_t s_playerTriStrip[] =
	{
		0, 1, 2,
		3
	};

	// TODO: have this vertex buffer static? same for all!!
	// Create static vertex buffer.
	m_vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_playerVertices, sizeof(s_playerVertices) )
		, PosNormalTangentTexcoordVertex::ms_decl
		);

	// Create static index buffer.
	m_ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_playerTriStrip, sizeof(s_playerTriStrip) )
		);
}

void GraphicsObject::createUniforms() {

}

void GraphicsObject::update(float d) {

	// Set render states.
	bgfx::setState(0
		       | BGFX_STATE_DEFAULT
		       | BGFX_STATE_PT_TRISTRIP
		       | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
					       BGFX_STATE_BLEND_INV_SRC_ALPHA )
		);

//	bgfx::setTransform(m_transform.data());
	bgfx::setVertexBuffer(m_vbh);
	bgfx::setIndexBuffer(m_ibh);
//	bgfx::setTexture(0, s_texColor,  m_texture_atlas->getColorHandle());


	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_shader->getHandle());

}

}
