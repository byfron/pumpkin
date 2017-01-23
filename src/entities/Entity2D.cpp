#include "Entity2D.hpp"
#include <engine/Engine.hpp>
#include <iostream>
#include <common/debugdraw/debugdraw.h>

Entity2D::Entity2D(const std::string & texture,
		   const std::string & shader,
		   float w, float h) : m_height(h),
				       m_width(w) {

	PosNormalTangentTexcoordVertex::init();

	m_flipped = 0;
	m_atlas_offset[0] = 0;
	m_atlas_offset[1] = 0;
	// sprite texture width: this comes from the grid of the atlas!
	// we pack it in the z coord of the atlas offset (vec4)
	m_atlas_offset[2] = 1.0/4.0;
	
	m_transform = Eigen::MatrixXf(4,4);
}

Entity2D::~Entity2D() {
	bgfx::destroyUniform(u_texOffset);
	bgfx::destroyUniform(u_flip);
	bgfx::destroyUniform(u_lightPosRadius);
}

void Entity2D::init() {

	//TODO get from proto cfg?
	m_textureColor = loadTexture("textures/character.ktx");
	m_program  = loadProgram("vs_character", "fs_character");

	// size of a single sprite. TODO: This depends on the grid!!
	int16_t tsize = 0x7fff/4;

	static PosNormalTexCoordVertex s_playerVertices[] =
	{
		// Horizonally aligned	
		{  0.0f,      m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f), tsize, tsize },
		{  0.0f,     -m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f),  0, tsize },
		{ -m_height,  m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f), tsize, 0 },
		{ -m_height, -m_width/2.0f, 0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, 0 }

	};
	
	static const uint16_t s_playerTriStrip[] =
	{
		0, 1, 2,
		3
	};
	
	// Create static vertex buffer.
	m_vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_playerVertices, sizeof(s_playerVertices) )
		, PosNormalTexCoordVertex::ms_decl
		);
		
	// Create static index buffer.
	m_ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_playerTriStrip, sizeof(s_playerTriStrip) )
		);

	u_texOffset = bgfx::createUniform("packed_input",  bgfx::UniformType::Vec4);
	u_flip = bgfx::createUniform("flip", bgfx::UniformType::Int1);
	u_lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, 1);
	
	// debugging
	ddInit();

}

Eigen::Affine3f Entity2D::getTowardsCameraRotation() {
       
	Eigen::Vector3f axis = Eigen::Vector3f(0.0, 0.0, 1.0);
	Eigen::Vector3f norm = Eigen::Vector3f(1.0, 0.0, 0.0);
	Eigen::Vector3f eye = Engine::camera().getEye();
	Eigen::Vector3f dir2D = Engine::camera().getDirection();

	float pitch = Engine::camera().getPitch();
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

void Entity2D::update(float d) {

	// Set render states.
	bgfx::setState(0
		       | BGFX_STATE_DEFAULT
		       | BGFX_STATE_PT_TRISTRIP
		       | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
					       BGFX_STATE_BLEND_INV_SRC_ALPHA )
		);	


	float lightPosRadius[4] = { 4.0, 4.0, 1.0, 2.0};
	bgfx::setUniform(u_lightPosRadius, lightPosRadius, 1);
	
	Eigen::Vector3f at = Engine::camera().getAt();

//	at(2) = 0;
//	m_pos = at;	

	Eigen::Affine3f translation(Eigen::Translation3f(m_position(0),
							 m_position(1),
							 m_position(2)));
	Eigen::Affine3f rot = getTowardsCameraRotation();

	Eigen::Vector3f axis = Eigen::Vector3f(0.0, 1.0, 0.0);
	
	m_transform = (translation * rot).matrix();
	bgfx::setTransform(m_transform.data());

	bgfx::setVertexBuffer(m_vbh);		
	bgfx::setIndexBuffer(m_ibh);
	bgfx::setTexture(0, s_texColor,  m_textureColor);

	bgfx::setUniform(u_texOffset, m_atlas_offset, 1);
	bgfx::setUniform(u_flip, &m_flipped, 1);

	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_program);


	//TODO refactor to debug helper
	if (Engine::debugEnabled()) {
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
 
