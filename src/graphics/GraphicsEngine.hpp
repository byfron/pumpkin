#ifndef ENGINE_H_HEADER_GUARD
#define ENGINE_H_HEADER_GUARD

#include <common/bgfx_utils.h>
#include <common/common.h>
#include <utils/Camera.hpp>
#include <utils/InputManager.hpp>
#include "Shader.hpp"

namespace pumpkin {
		
// Views
#define RENDER_PASS_GEOMETRY 0
#define RENDER_PASS_POSTPROCESS 1	

class GraphicsEngine : public entry::AppI {

public:
	GraphicsEngine() {};

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

	void initResources();
	void loadShaders();
	void loadTextures();
	void loadMeshes();

	virtual void frame(const float) {};
	virtual void init_engine() {};

	void run();
	void start(int _argc, char** _argv);
	void stop();


	void  screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width, float _height);
	static Camera & camera() { return m_camera; }
	static bool debugEnabled() { return m_debug; }
protected:

	uint32_t m_width;
	uint32_t m_height;
	static Camera m_camera;
	static bool m_debug;

	InputManager m_input_manager;

	// Refactor this in a PostProcessor
	bgfx::TextureHandle m_gbufferTex[3];
	bgfx::FrameBufferHandle m_geometryBuffer;
	bgfx::UniformHandle u_postTex;
	Shader::Ptr m_postProcessProgram;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
};

}

#endif
