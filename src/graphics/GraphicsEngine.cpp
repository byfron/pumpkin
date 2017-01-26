#include "GraphicsEngine.hpp"
#include "ResourceManager.hpp"
#include "TextureAtlas.hpp"
#include "Animation.hpp"
#include <utils/VertexUtils.hpp>
#include <utils/Camera.hpp>
#include <iostream>
#include <common/entry/input.h>

namespace pumpkin {
		
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

	bgfx::setViewClear(0
			   , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			   , 0x303030ff
			   , 1.0f
			   , 0);

	PosNormalTexCoordVertex::init();
	PosTexCoordVertex::init();

	initResources();
	
	init_engine();

	m_camera.init();
	m_input_manager.init();
}

void GraphicsEngine::initResources() {

	// this should eventually be put together in a global cfg file
	ResourceManager::pushResource<TextureAtlas>("main_character_atlas.cfg");
	ResourceManager::pushResource<TextureAtlas>("terrain_atlas.cfg");
	ResourceManager::pushResource<Shader>("main_character_shader.cfg");
	ResourceManager::pushResource<Shader>("terrain_shader.cfg");	
}
	
void GraphicsEngine::run() {
	
	const float deltaTime = getDeltaTime();

	bgfx::setViewRect(0, 0, 0, m_width, m_height);

	// rename this functions and classes so that it makes more sense
	frame(deltaTime);
	
	m_camera.update(deltaTime);
	
	float view[16];
	float proj[16];
	m_camera.mtxLookAt(view);
	bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
	bgfx::setViewTransform(0, view, proj);

}

void GraphicsEngine::stop() {

//	destroyCamera()
//	cameraDestroy();
	
}

}
