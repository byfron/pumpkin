#include "Entity2D.hpp"
#include <engine/Engine.hpp>
#include <iostream>
#include <common/debugdraw/debugdraw.h>

Entity2D::Entity2D() : m_height(0.5), m_width(0.5) {
}

Entity2D::~Entity2D() {
	bgfx::destroyUniform(u_texOffset);

}

void Entity2D::init() {

	m_textureColor = loadTexture("textures/character.ktx");
	m_program  = loadProgram("vs_character", "fs_character");

	// size of a single sprite
	int16_t tsize = 0x7fff/4;
	
	static PosTexCoordVertex s_playerVertices[] =
	{
		// Horizonally aligned	
		{  0.0f,      m_width/2.0f, 0.0f, tsize, tsize },
		{  0.0f,     -m_width/2.0f, 0.0f, 0, tsize },
		{ -m_height,  m_width/2.0f, 0.0f, tsize, 0 },
		{ -m_height, -m_width/2.0f, 0.0f, 0, 0 }

	};

	m_pos << 2.0, 2.0, 0.0;
	
	static const uint16_t s_playerTriStrip[] =
	{
		0, 1, 2,
		3
	};
	
	// Create static vertex buffer.
	m_vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_playerVertices, sizeof(s_playerVertices) )
		, PosTexCoordVertex::ms_decl
		);
		
	// Create static index buffer.
	m_ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_playerTriStrip, sizeof(s_playerTriStrip) )
		);

	u_texOffset = bgfx::createUniform("tex0_offset",  bgfx::UniformType::Vec4);
	
	// debugging
	ddInit();

}

Eigen::Affine3f Entity2D::getTowardsCameraRotation() {
       
	Eigen::Vector3f axis = Eigen::Vector3f(0.0, 0.0, 1.0);
	Eigen::Vector3f norm = Eigen::Vector3f(1.0, 0.0, 0.0);
	Eigen::Vector3f eye = Engine::camera().getEye();
	Eigen::Vector3f dir2D = Engine::camera().getDirection();

	float pitch = Engine::camera().getPitch();
	Eigen::Vector3f dir = eye - m_pos;
	
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
		       | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA , BGFX_STATE_BLEND_INV_SRC_ALPHA )
		);	

	Eigen::Vector3f eye = Engine::camera().getEye();
	Eigen::Vector3f at = Engine::camera().getAt();

	at(2) = 0;
	m_pos = at;	


	Eigen::Affine3f translation(Eigen::Translation3f(m_pos(0), m_pos(1), m_pos(2)));
	Eigen::Affine3f rot = getTowardsCameraRotation();

	Eigen::Vector3f axis = Eigen::Vector3f(0.0, 1.0, 0.0);
	
	m_transform = (translation * rot).matrix();
	bgfx::setTransform(m_transform.data());

	bgfx::setVertexBuffer(m_vbh);		
	bgfx::setIndexBuffer(m_ibh);
	bgfx::setTexture(0, s_texColor,  m_textureColor);

	float atlas_offset[4];
	getAtlasOffset(atlas_offset);
	bgfx::setUniform(u_texOffset, atlas_offset, 1);
       
	// Submit primitive for rendering to view 0.
	bgfx::submit(0, m_program);


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
 
