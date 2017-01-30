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
		m_flipped(0),
		m_transform(Eigen::MatrixXf(4,4)){

	m_atlas_offset[0] = 0.0;
	m_atlas_offset[1] = 0.0;
	m_atlas_offset[2] = 1.0/4.0;
}

GraphicsObject::GraphicsObject(const std::string & config_file) :
	m_flipped(0),
	m_transform(Eigen::MatrixXf(4,4)) {

	GraphicsObjectFactory factory(config_file);
	factory.generate(this);

	// sprite texture width: this comes from the grid of the atlas!
	// we pack it in the z coord of the atlas offset (vec4)
	// PUT IT IN FACTORY
	m_atlas_offset[0] = 0.0;
	m_atlas_offset[1] = 0.0;
	m_atlas_offset[2] = 1.0/4.0;
}

GraphicsObject::~GraphicsObject() {
	destroyUniforms();
}

void GraphicsObject::destroyUniforms() {

	bgfx::destroyUniform(u_texOffset);
	bgfx::destroyUniform(u_flip);
	bgfx::destroyUniform(u_lightPosRadius);
}

void GraphicsObject::init() {

	m_texture_atlas->init();
	m_shader->init();
	initialiseBuffers();
	createUniforms();

	// debugging
	ddInit();
}


bool GraphicsObject::loadTextureAtlas(uint32_t id) {
	m_texture_atlas = ResourceManager::getResource<TextureAtlas>(id);
	if (!m_texture_atlas) {
		//log error
		std::cout << "error loading texture atlas cfg" << std::endl;
		return false;
	}
	return true;
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

	// TODO: use the modern mesh object

	// MeshObject<PosNormalTexCoordVertex> mesh = MeshFactory<PosNormalTexCoordVertex>::
	// 	construct(MeshType::PLANE_MESH,
	// 		  MeshProperties(0, 0, m_scale, m_width, m_height));


	// // TODO: have this vertex buffer static?
	// m_vbh = bgfx::createVertexBuffer(
	// 	// Static data can be passed with bgfx::makeRef
	// 	bgfx::makeRef(&mesh.m_vertex_pool[0],
	// 		      sizeof(PosNormalTexCoordVertex) *
	// 		      mesh.m_vertex_pool.size() )
	// 	, PosNormalTexCoordVertex::ms_decl
	// 	);

	// // Create static index buffer.
	// m_ibh = bgfx::createIndexBuffer(
	// 	// Static data can be passed with bgfx::makeRef
	// 	bgfx::makeRef(&mesh.m_index_pool[0],
	// 		      sizeof(uint16_t) * mesh.m_index_pool.size())
	// 	);
}

void GraphicsObject::createUniforms() {

	u_texOffset = bgfx::createUniform("packed_input",  bgfx::UniformType::Vec4);
	u_flip = bgfx::createUniform("flip", bgfx::UniformType::Int1);
	u_lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, 1);
}

Eigen::Affine3f GraphicsObject::getTowardsCameraRotation() {

	Eigen::Vector3f axis = Eigen::Vector3f(0.0, 0.0, 1.0);
	Eigen::Vector3f norm = Eigen::Vector3f(1.0, 0.0, 0.0);
	Eigen::Vector3f eye = GraphicsEngine::camera().getEye();
	Eigen::Vector3f dir2D = GraphicsEngine::camera().getDirection();

	float pitch = GraphicsEngine::camera().getPitch();
	Eigen::Vector3f dir = eye - m_position;

	//rotate it so that the normal and the projection on XY are aligned
	eye(2) = 0;
	dir2D(2) = 0;
	dir2D.normalize();
	dir.normalize();

	float dot = norm(0)*dir2D(0) + norm(1) * dir2D(1);
	float det = norm(0)*dir2D(1) - norm(1) * dir2D(0);
	float angle = atan2f(det, dot);

	Eigen::Affine3f rot = Eigen::Affine3f(
		Eigen::AngleAxisf(angle, Eigen::Vector3f(0.0, 0.0, 1.0)));

	Eigen::Affine3f rot_up = Eigen::Affine3f(
	 	Eigen::AngleAxisf(1.2 + M_PI/2, Eigen::Vector3f(0.0, -1.0, 0.0)));

	return rot * rot_up;
}

void GraphicsObject::update(float d) {

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

	Eigen::Affine3f translation(Eigen::Translation3f(m_position(0),
							 m_position(1),
							 m_position(2)));
	Eigen::Affine3f rot = getTowardsCameraRotation();

	Eigen::Vector3f axis = Eigen::Vector3f(0.0, 1.0, 0.0);

	m_transform = (translation * rot).matrix();
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
