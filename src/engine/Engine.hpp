#ifndef ENGINE_H_HEADER_GUARD
#define ENGINE_H_HEADER_GUARD


#include <common/bgfx_utils.h>
#include <common/common.h>
#include "Camera.hpp"
#include "InputManager.hpp"
#include "ResourceManager.hpp"

class Engine : public entry::AppI {

public:
	Engine() {};

	void init(int _argc, char** _argv) BX_OVERRIDE {
		start(_argc, _argv);
	}

	virtual int shutdown() BX_OVERRIDE {
		stop();
	}

	bool update() BX_OVERRIDE
	{

		if (!InputManager::processEvents(m_width, m_height)) {

			bgfx::touch(0);

			run();

			//submit frame
			bgfx::frame();

			return true;
		}

		return false;
	}


	virtual void frame(const float) {};
	virtual void init_engine() {};

	void run();
	void start(int _argc, char** _argv);
	void stop();

	static Camera & camera() { return m_camera; }
	static bool debugEnabled() { return m_debug; }
protected:

	uint32_t m_width;
	uint32_t m_height;
//	World m_world;

	static Camera m_camera;
	static bool m_debug;

	ResourceManager _resource_manager;
};


#endif
