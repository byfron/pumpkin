#include "GraphicsEngine.hpp"
#include "ResourceManager.hpp"
#include "TextureAtlas.hpp"
#include "Animation.hpp"
#include "DebugManager.hpp"
#include <utils/Mesh.hpp>
#include <utils/VertexUtils.hpp>
#include <utils/Camera.hpp>
#include <utils/Configuration.hpp>
#include <iostream>
#include <common/entry/input.h>

namespace pumpkin {


void GraphicsEngine::screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width = 1.0f, float _height = 1.0f)
{

	// const float minxx = -_width;
	// const float maxxx =  _width;
	// const float minyy = 0.0f;
	// const float maxyy = _height;

	// PosTexCoordVertex vb[] =
	// {
	// 	{minxx, minyy, 0, 0},
	// 	{minxx, maxyy, 0, 1.0},
	// 	{maxxx, maxyy, 1.0, 1.0},
	// 	{maxxx, minyy, 1.0, 0},
	// };

	// int ib[4] = {
	// 	0,3,1,2
	// };


	// m_vbh = bgfx::createVertexBuffer(
	// 	bgfx::makeRef(vb, sizeof(vb) )
	// 	, PosTexCoordVertex::ms_decl);


	// m_ibh = bgfx::createIndexBuffer(
	// 	// Static data can be passed with bgfx::makeRef
	// 	bgfx::makeRef(ib, sizeof(ib) )
	// 	);

	// bgfx::setVertexBuffer(m_vbh);
	// bgfx::setIndexBuffer(m_ibh);
	// return;


	if (3 == bgfx::getAvailTransientVertexBuffer(3, PosTexCoordVertex::ms_decl) ) //
	{

		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosTexCoordVertex::ms_decl);
		PosTexCoordVertex* vertex = (PosTexCoordVertex*)vb.data;

		const float minx = -_width;
		const float maxx =  _width;
		const float miny = 0.0f;
		const float maxy = _height*2.0f;

		const float texelHalfW = _texelHalf/_textureWidth;
		const float texelHalfH = _texelHalf/_textureHeight;
		const float minu = -1.0f + texelHalfW;
		const float maxu =  1.0f + texelHalfH;

		const float zz = 0.0f;

		float minv = texelHalfH;
		float maxv = 2.0f + texelHalfH;

		if (_originBottomLeft)
		{
			float temp = minv;
			minv = maxv;
			maxv = temp;

			minv -= 1.0f;
			maxv -= 1.0f;
		}

		vertex[0].m_x = minx;
		vertex[0].m_y = miny;
		vertex[0].m_z = zz;
		vertex[0].m_u = minu;
		vertex[0].m_v = minv;

		vertex[1].m_x = maxx;
		vertex[1].m_y = miny;
		vertex[1].m_z = zz;
		vertex[1].m_u = maxu;
		vertex[1].m_v = minv;

		vertex[2].m_x = maxx;
		vertex[2].m_y = maxy;
		vertex[2].m_z = zz;
		vertex[2].m_u = maxu;
		vertex[2].m_v = maxv;

		bgfx::setVertexBuffer(&vb);
	}
}

Camera GraphicsEngine::m_camera;
bool GraphicsEngine::m_debug = false;

float getDeltaTime() {
	int64_t now = bx::getHPCounter();
	static int64_t last = now;
	const int64_t frameTime = now - last;
	last = now;
	const double freq = double(bx::getHPFrequency() );
	return float(frameTime/freq);
}

void GraphicsEngine::start(int _argc, char** _argv) {
	Args args(_argc, _argv);
	bgfx::init(args.m_type, args.m_pciId);

	m_width = 1280;
	m_height = 720;

	bgfx::reset(m_width, m_height, InputManager::m_reset);
	bgfx::setDebug(InputManager::m_debug);

	// Set palette color for index 0
	bgfx::setPaletteColor(0, UINT32_C(0x00000000) );

	// Set palette color for index 1
	bgfx::setPaletteColor(1, UINT32_C(0x303030ff) );

	bgfx::setViewClear(RENDER_PASS_GEOMETRY
		, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
		, 1.0f
		, 0
		, 1);

	bgfx::setViewClear(RENDER_PASS_POSTPROCESS
		, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
		, 1.0f
		, 0
		, 1);

	PosColorVertex::init();
	PosNormalTexCoordVertex::init();
	PosNormalTangentTexcoordVertex::init();
	PosTexCoordVertex::init();

	initResources();

	init_engine();

	m_camera.init();
	m_input_manager.init();
}

void GraphicsEngine::initResources() {
	// Move this to voyage engine?
	loadTextures();
	loadShaders();

	// Refactor in PostProcessor::init()
	u_postTex  = bgfx::createUniform("u_postTex",  bgfx::UniformType::Int1);
	m_geometryBuffer.idx = bgfx::invalidHandle;
	m_gbufferTex[0].idx = bgfx::invalidHandle;
	m_gbufferTex[1].idx = bgfx::invalidHandle;
	m_gbufferTex[2].idx = bgfx::invalidHandle;
	m_postProcessProgram = ResourceManager::getResource<Shader>(4);
	m_postProcessProgram->init();
}

void GraphicsEngine::loadMeshes() {

	// Configuration<voyage::MeshListCfg> mesh_list(
	// 	std::string(CONFIG_FILE_PATH) + "meshes.cfg");
	// for (int i = 0; i < mesh_list.config().mesh().size(); i++) {
	// 	ResourceManager::pushResource<Mesh, voyage::MeshCfg>
	// 		 (mesh_list.config().mesh(i));
	// }
}

void GraphicsEngine::loadTextures() {

	Configuration<voyage::AtlasListCfg> texture_list(
		std::string(CONFIG_FILE_PATH) + "textures.cfg");
	for (int i = 0; i < texture_list.config().texture().size(); i++) {
		ResourceManager::pushResource<TextureAtlas, voyage::AtlasCfg>
			 (texture_list.config().texture(i));
	}
}

void GraphicsEngine::loadShaders() {

	Configuration<voyage::ShaderListCfg> shader_list(
		std::string(CONFIG_FILE_PATH) + "shaders.cfg");
	for (int i = 0; i < shader_list.config().shader().size(); i++) {
		ResourceManager::pushResource<Shader, voyage::ShaderCfg>
			 (shader_list.config().shader(i));
	}
}

void GraphicsEngine::run() {

	const float deltaTime = getDeltaTime();

	// If the size of the window changes, update the size of framebuffers
	if (m_oldWidth  != m_width  ||  m_oldHeight != m_height) {
		m_oldWidth = m_width;
		m_oldHeight = m_height;

		if (bgfx::isValid(m_geometryBuffer) )
		{
			bgfx::destroyFrameBuffer(m_geometryBuffer);
		}


		const uint32_t samplerFlags = 0
			| BGFX_TEXTURE_RT
			| BGFX_TEXTURE_MIN_POINT
			| BGFX_TEXTURE_MAG_POINT
			| BGFX_TEXTURE_MIP_POINT
			| BGFX_TEXTURE_U_CLAMP
			| BGFX_TEXTURE_V_CLAMP;

		m_gbufferTex[0] = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx::TextureFormat::BGRA8, samplerFlags);
		m_gbufferTex[1] = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx::TextureFormat::BGRA8, samplerFlags);
		m_gbufferTex[2] = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx::TextureFormat::D24, samplerFlags);

		m_geometryBuffer = bgfx::createFrameBuffer(BX_COUNTOF(m_gbufferTex), m_gbufferTex, true);

	}

	// Default view
	bgfx::setViewRect(RENDER_PASS_GEOMETRY, 0, 0, m_width, m_height);

	// Post-process
	bgfx::setViewRect(RENDER_PASS_POSTPROCESS, 0, 0, m_width, m_height);

	// rename this functions and classes so that it makes more sense
//	m_particle_system_pool.update(deltaTime);

	// Set geometry view
	float view[16];
	float proj[16];
	m_camera.mtxLookAt(view);
	bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
	bgfx::setViewFrameBuffer(RENDER_PASS_GEOMETRY, m_geometryBuffer);
	bgfx::setViewTransform(RENDER_PASS_GEOMETRY, view, proj);

	// Set post processing view
	bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f);
	bgfx::setViewTransform(RENDER_PASS_POSTPROCESS, NULL, proj);

	DebugManager::update(deltaTime);


	frame(deltaTime);
	m_camera.update(deltaTime);

	// Pass geometry buffer as a post-processor shader texture
	bgfx::setTexture(0, u_postTex,  bgfx::getTexture(m_geometryBuffer, 0) );

	// Create a quad geometry to display the post-processed texture
	const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
	float texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;

	bgfx::setState(0
		       | BGFX_STATE_RGB_WRITE
		       | BGFX_STATE_ALPHA_WRITE
		);

	screenSpaceQuad( (float)m_width, (float)m_height, texelHalf, true);

	bgfx::submit(RENDER_PASS_POSTPROCESS, m_postProcessProgram->getHandle());


}

void GraphicsEngine::stop() {

	bgfx::destroyFrameBuffer(m_geometryBuffer);
	bgfx::destroyUniform(u_postTex);
//	destroyCamera()
//	cameraDestroy();

}

}
