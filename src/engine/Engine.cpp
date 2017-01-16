#include "Engine.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include <iostream>
#include <entities/Animation2D.hpp>
#include <common/entry/input.h>

uint32_t InputManager::m_debug = BGFX_DEBUG_TEXT;
uint32_t InputManager::m_reset = BGFX_RESET_VSYNC;
entry::MouseState InputManager::m_mouseState;
Mouse InputManager::m_mouse;

Camera Engine::m_camera;
bool Engine::m_debug = false;

float getDeltaTime() {
	int64_t now = bx::getHPCounter();
	static int64_t last = now;
	const int64_t frameTime = now - last;
	last = now;
	const double freq = double(bx::getHPFrequency() );
	return float(frameTime/freq);
}

void Engine::start(int _argc, char** _argv) {
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

//	createCamera();

	PosTexCoordVertex::init();

	init_engine();

	m_camera.init();

	//TODO: Create resources somewhere else

	Resource::Ptr res_entity = std::make_shared<Animation2D>();
	// Do the init when pushing the resource?
	res_entity->init();

	// TODO. Move this to an AnimationManager

	Animation2D *anim = static_cast<Animation2D*>(res_entity.get());
	anim->addFrame(AnimationId::DOWN, Animation2D::AnimationFrame(0.0, 0.25));
	anim->addFrame(AnimationId::DOWN, Animation2D::AnimationFrame(0.25, 0.25));
	anim->addFrame(AnimationId::DOWN, Animation2D::AnimationFrame(0.5, 0.25));
	anim->addFrame(AnimationId::DOWN, Animation2D::AnimationFrame(0.75, 0.25));
	anim->switchToAnim(AnimationId::DOWN);
	
	_resource_manager.pushResource(0, res_entity);

	Resource::Ptr res_tilemap = std::make_shared<TileMapChunk>(0, 0, 10, 10);
	res_tilemap->init();
	_resource_manager.pushResource(1, res_tilemap);
}

void Engine::run() {
	
	const float deltaTime = getDeltaTime();

	bgfx::setViewRect(0, 0, 0, m_width, m_height);

	//m_camera.update(deltaTime);

	// rename this functions and classes so that it makes more sense
	frame(deltaTime);

//	cameraUpdate(deltaTime, InputManager::m_mouseState);
	m_camera.update(deltaTime);
//	getCamera()->update(deltaTime);
//	m_world.update(deltaTime);	
	
	float view[16];
	float proj[16];
	m_camera.mtxLookAt(view);
//	getCamera()->mtxLookAt(view);
	bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
	bgfx::setViewTransform(0, view, proj);

}

void Engine::stop() {

//	destroyCamera()
//	cameraDestroy();
	
}
