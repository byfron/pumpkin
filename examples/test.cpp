#include <common/common.h>
#include <common/camera.h>
#include <common/bgfx_utils.h>
#include <vector>
#include <iostream>
#include <limits>


#define FLT_MIN std::numeric_limits<float>::min()


struct Mouse
{
	Mouse()
	{
		m_dx = 0.0f;
		m_dy = 0.0f;
		m_prevMx = 0.0f;
		m_prevMx = 0.0f;
		m_scroll = 0;
		m_scrollPrev = 0;
	}

	void update(float _mx, float _my, int32_t _mz, uint32_t _width, uint32_t _height)
	{
		const float widthf  = float(int32_t(_width));
		const float heightf = float(int32_t(_height));

		// Delta movement.
		m_dx = float(_mx - m_prevMx)/widthf;
		m_dy = float(_my - m_prevMy)/heightf;

		m_prevMx = _mx;
		m_prevMy = _my;

		// Scroll.
		m_scroll = _mz - m_scrollPrev;
		m_scrollPrev = _mz;
	}

	float m_dx; // Screen space.
	float m_dy;
	float m_prevMx;
	float m_prevMy;
	int32_t m_scroll;
	int32_t m_scrollPrev;
};


struct Camera
{
	Camera()
	{
		reset();
	}

	void reset()
	{
		m_target.curr[0] = 0.0f;
		m_target.curr[1] = 0.0f;
		m_target.curr[2] = 0.0f;
		m_target.dest[0] = 2.0f;
		m_target.dest[1] = 2.0f;
		m_target.dest[2] = 0.0f;

		m_pos.curr[0] =  0.0f;
		m_pos.curr[1] =  0.0f;
		m_pos.curr[2] = -2.0f;
		m_pos.dest[0] =  1.5f;
		m_pos.dest[1] =  1.5f;
		m_pos.dest[2] = -5.0f;

		m_orbit[0] = 0.0f;
		m_orbit[1] = 0.0f;
	}

	void mtxLookAt(float* _outViewMtx)
	{
		bx::mtxLookAt(_outViewMtx, m_pos.curr, m_target.curr);
	}

	void orbit(float _dx, float _dy)
	{
		m_orbit[0] += _dx;
		m_orbit[1] += _dy;
	}

	void dolly(float _dz)
	{
		const float cnear = 0.01f;
		const float cfar  = 10.0f;

		const float toTarget[3] =
		{
			m_target.dest[0] - m_pos.dest[0],
			m_target.dest[1] - m_pos.dest[1],
			m_target.dest[2] - m_pos.dest[2],
		};
		const float toTargetLen = bx::vec3Length(toTarget);
		const float invToTargetLen = 1.0f/(toTargetLen+FLT_MIN);
		const float toTargetNorm[3] =
		{
			toTarget[0]*invToTargetLen,
			toTarget[1]*invToTargetLen,
			toTarget[2]*invToTargetLen,
		};

		float delta = toTargetLen*_dz;
		float newLen = toTargetLen + delta;
		if ( (cnear < newLen || _dz < 0.0f)
		&&   (newLen < cfar  || _dz > 0.0f) )
		{
			m_pos.dest[0] += toTargetNorm[0]*delta;
			m_pos.dest[1] += toTargetNorm[1]*delta;
			m_pos.dest[2] += toTargetNorm[2]*delta;
		}
	}

	void consumeOrbit(float _amount)
	{
		float consume[2];
		consume[0] = m_orbit[0]*_amount;
		consume[1] = m_orbit[1]*_amount;
		m_orbit[0] -= consume[0];
		m_orbit[1] -= consume[1];

		const float toPos[3] =
		{
			m_pos.curr[0] - m_target.curr[0],
			m_pos.curr[1] - m_target.curr[1],
			m_pos.curr[2] - m_target.curr[2],
		};
		const float toPosLen = bx::vec3Length(toPos);
		const float invToPosLen = 1.0f/(toPosLen+FLT_MIN);
		const float toPosNorm[3] =
		{
			toPos[0]*invToPosLen,
			toPos[1]*invToPosLen,
			toPos[2]*invToPosLen,
		};

		float ll[2];
		latLongFromVec(ll[0], ll[1], toPosNorm);
		ll[0] += consume[0];
		ll[1] -= consume[1];
		ll[1] = bx::fclamp(ll[1], 0.02f, 0.98f);

		float tmp[3];
		vecFromLatLong(tmp, ll[0], ll[1]);

		float diff[3];
		diff[0] = (tmp[0]-toPosNorm[0])*toPosLen;
		diff[1] = (tmp[1]-toPosNorm[1])*toPosLen;
		diff[2] = (tmp[2]-toPosNorm[2])*toPosLen;

		m_pos.curr[0] += diff[0];
		m_pos.curr[1] += diff[1];
		m_pos.curr[2] += diff[2];
		m_pos.dest[0] += diff[0];
		m_pos.dest[1] += diff[1];
		m_pos.dest[2] += diff[2];
	}

	void update(float _dt)
	{
		const float amount = bx::fmin(_dt/0.12f, 1.0f);

		consumeOrbit(amount);

		m_target.curr[0] = bx::flerp(m_target.curr[0], m_target.dest[0], amount);
		m_target.curr[1] = bx::flerp(m_target.curr[1], m_target.dest[1], amount);
		m_target.curr[2] = bx::flerp(m_target.curr[2], m_target.dest[2], amount);
		m_pos.curr[0] = bx::flerp(m_pos.curr[0], m_pos.dest[0], amount);
		m_pos.curr[1] = bx::flerp(m_pos.curr[1], m_pos.dest[1], amount);
		m_pos.curr[2] = bx::flerp(m_pos.curr[2], m_pos.dest[2], amount);
	}

	static inline void vecFromLatLong(float _vec[3], float _u, float _v)
	{
		const float phi   = _u * 2.0f*bx::pi;
		const float theta = _v * bx::pi;

		const float st = bx::fsin(theta);
		const float sp = bx::fsin(phi);
		const float ct = bx::fcos(theta);
		const float cp = bx::fcos(phi);

		_vec[0] = -st*sp;
		_vec[1] = ct;
		_vec[2] = -st*cp;
	}

	static inline void latLongFromVec(float& _u, float& _v, const float _vec[3])
	{
		const float phi = atan2f(_vec[0], _vec[2]);
		const float theta = acosf(_vec[1]);

		_u = (bx::pi + phi)*bx::invPi*0.5f;
		_v = theta*bx::invPi;
	}

	struct Interp3f
	{
		float curr[3];
		float dest[3];
	};

	Interp3f m_target;
	Interp3f m_pos;
	float m_orbit[2];
};


struct PosTexCoordVertex
{
	float m_x;
	float m_y;
	float m_z;
	int16_t m_u;
	int16_t m_v;
       

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosTexCoordVertex::ms_decl;


struct PosNormalTangentTexcoordVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_normal;
	uint32_t m_tangent;
	int16_t m_u;
	int16_t m_v;

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal,    4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::Tangent,   4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
			.end();
	}

	static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosNormalTangentTexcoordVertex::ms_decl;

uint32_t packUint32(uint8_t _x, uint8_t _y, uint8_t _z, uint8_t _w)
{
	union
	{
		uint32_t ui32;
		uint8_t arr[4];
	} un;

	un.arr[0] = _x;
	un.arr[1] = _y;
	un.arr[2] = _z;
	un.arr[3] = _w;

	return un.ui32;
}

uint32_t packF4u(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f)
{
	const uint8_t xx = uint8_t(_x*127.0f + 128.0f);
	const uint8_t yy = uint8_t(_y*127.0f + 128.0f);
	const uint8_t zz = uint8_t(_z*127.0f + 128.0f);
	const uint8_t ww = uint8_t(_w*127.0f + 128.0f);
	return packUint32(xx, yy, zz, ww);
}

/*
 0----1
 |    |
 |    |
 3----2 
 */


struct Tile {

	Tile() : m_isDefined(false) {}
	
	uint16_t m_indices[4];
	//triangles = 0,1,2 / 2,3,0

	bool isDefined () {
		return m_isDefined;
	}
	bool m_isDefined;
};


	
	static PosTexCoordVertex s_playerVertices[] =
	{
		{ 0.0f,  -1.0f,  1.0f, 0, 0x7fff },
		{ 0.0f,   1.0f,  1.0f, 0x7fff, 0x7fff },
		{ 0.0f,  -1.0f, -1.0f, 0, 0 },
		{ 0.0f,   1.0f, -1.0f, 0x7fff, 0 }
	};

	static const uint16_t s_playerTriStrip[] =
	{
		0, 1, 2,
		3
	};

struct Player {


	uint16_t m_indices[6];
	bgfx::UniformHandle s_texColor;
	bgfx::TextureHandle m_textureColor;
	bgfx::ProgramHandle m_program;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	float m_transform[16];
	
	void init() {

		m_textureColor = loadTexture("maddog.ktx");
		m_program  = loadProgram("vs_terrain", "fs_terrain");

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
		
	}


	void update() {


		// Set render states.
		bgfx::setState(0
			       | BGFX_STATE_DEFAULT
			       | BGFX_STATE_PT_TRISTRIP
//			       | BGFX_STATE_RGB_WRITE
//			       | BGFX_STATE_ALPHA_WRITE
			       | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA , BGFX_STATE_BLEND_INV_SRC_ALPHA )
			);
		// bgfx::setState(0
		// 	       | BGFX_STATE_DEFAULT
		// 	       | BGFX_STATE_PT_TRISTRIP
		// 	);

		float mtx[16];

		// rotate the sprite so that it always faces the camera
		
		bx::mtxRotateZ(mtx, 2.4);
		mtx[12] = 2.0f;
		mtx[13] = 2.0f;
		mtx[14] = -1.0f;
		bgfx::setTransform(mtx);

		bgfx::setVertexBuffer(m_vbh);		
		bgfx::setIndexBuffer(m_ibh);
		bgfx::setTexture(0, s_texColor,  m_textureColor);

		// Submit primitive for rendering to view 0.
		bgfx::submit(0, m_program);
	}
	
};

class Test : public entry::AppI {

	void init(int _argc, char** _argv) BX_OVERRIDE {
		
		Args args(_argc, _argv);
		
		m_width  = 1280;
		m_height = 720;
		m_debug  = BGFX_DEBUG_TEXT;
		m_reset  = BGFX_RESET_VSYNC;
		
		bgfx::init(args.m_type, args.m_pciId);
		bgfx::reset(m_width, m_height, m_reset);
		
		s_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);

		// Enable debug text.
		bgfx::setDebug(m_debug);
		
		// Set view 0 clear state.
		bgfx::setViewClear(0
				   , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
				   , 0x303030ff
				   , 1.0f
				   , 0
			);

		// Create vertex stream declaration.
		PosTexCoordVertex::init();
		m_program  = loadProgram("vs_terrain", "fs_terrain");


		createTilemap();
		m_textureColor = loadTexture("test.ktx");


		m_player.init();
		
		bx::mtxSRT(m_transform, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		cameraCreate();
		
		const float initialPos[3] = { 1.0, 1.0, 5.0f };
		cameraSetPosition(initialPos);
		cameraSetVerticalAngle(-bx::pi/4.0f);
	}
	
	virtual int shutdown() BX_OVERRIDE {

		bgfx::destroyDynamicIndexBuffer(m_dibh);
		bgfx::destroyDynamicVertexBuffer(m_dvbh);
		bgfx::destroyProgram(m_program);
		bgfx::destroyTexture(m_textureColor);
		bgfx::destroyUniform(s_texColor);
	}

	void addTile(int r, int c) {

		uint16_t v_idx = m_vertexPool.size();
		uint16_t idx;
		float Z = 0.0f;

		Tile tile;
		tile.m_isDefined = true;
		
		// check neighbor tiles
		// PosColorVertex v_tile[4] = {
		// { r + 0.0f,  c + 0.0f,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0,      0,      0 },
		// { r + 0.0f,  c + 1.0f,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, 0x7fff,      0 },
		// { r + 1.0f,  c + 1.0f,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0,      0, 0x7fff },
		// { r + 1.0f,  c + 0.0f,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, 0x7fff, 0x7fff },
		// };

		//rows left
		//cols up

		// check neighbor tiles
		PosTexCoordVertex v_tile[4] = {
			{ r + 0.0f,  c + 1.0f,  0.0f, 0, 0x7fff },
			{ r + 1.0f,  c + 1.0f,  0.0f, 0x7fff, 0x7fff },
			{ r + 1.0f,  c + 0.0f,  0.0f, 0x7fff, 0 },
			{ r + 0.0f,  c + 0.0f,  0.0f, 0, 0 }
		};

		int tl = m_vertexPool.size();
		int tr = tl+1;
		int br = tr+1;
		int bl = br+1;
		
		// Create always four vertices per tile
		// So that we can texture tiles properly
		m_vertexPool.push_back(v_tile[0]); //tl
		m_vertexPool.push_back(v_tile[1]); //tr
		m_vertexPool.push_back(v_tile[2]); //br
		m_vertexPool.push_back(v_tile[3]); //bl		
		
		m_indices.push_back(tr);
		m_indices.push_back(tl);
		m_indices.push_back(bl);
		m_indices.push_back(tr);
		m_indices.push_back(bl);
		m_indices.push_back(br);
		
		tile.m_indices[0] = tl;
		tile.m_indices[1] = tr;
		tile.m_indices[2] = br;
		tile.m_indices[3] = bl;
		
		m_tilemap[r][c] = tile;
	}
	
	void createTilemap() {
				
	       	for (int i = 1; i < 4; i++)
		{
			for (int j = 1; j < 4; j++)
			{
				addTile(i,j);
			}
		}

		int vertexCount = m_vertexPool.size();
		int indexCount = m_indices.size();

		const bgfx::Memory* mem;
		mem = bgfx::makeRef(&m_vertexPool[0], sizeof(PosTexCoordVertex) * vertexCount);
		m_dvbh = bgfx::createDynamicVertexBuffer(mem, PosTexCoordVertex::ms_decl);

		mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * indexCount);
				
		m_dibh = bgfx::createDynamicIndexBuffer(mem);
		
	}

	bool update() BX_OVERRIDE {

		if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
		{
			int64_t now = bx::getHPCounter();
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency() );
			const float deltaTime = float(frameTime/freq);
			
			float at[3]  = { 0.0f, 0.0f,  0.0f };
			float eye[3] = { 5.0f, 5.0f, -10.0f };
			
			bgfx::touch(0);

			m_mouse.update(float(m_mouseState.m_mx), float(m_mouseState.m_my), m_mouseState.m_mz, m_width, m_height);

			if (m_mouseState.m_buttons[entry::MouseButton::Left])
			{
				m_camera.orbit(m_mouse.m_dx, m_mouse.m_dy);
			}
			else if (m_mouseState.m_buttons[entry::MouseButton::Right])
			{
				m_camera.dolly(m_mouse.m_dx + m_mouse.m_dy);
			}
			else if (0 != m_mouse.m_scroll)
			{
				m_camera.dolly(float(m_mouse.m_scroll)*0.1f);
			}
						
//			cameraUpdate(deltaTime, m_mouseState);

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, m_width, m_height);

//			cameraGetViewMtx(m_viewMtx);
//			bx::mtxProj(m_projMtx, 60.0f, float(m_width) / float(m_height), 0.1f, 2000.0f);

			float view[16];
			float proj[16];
			m_camera.update(deltaTime);
			//memcpy(m_uniforms.m_camPos, m_camera.m_pos.curr, 3*sizeof(float));
			m_camera.mtxLookAt(view);
			bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
			bgfx::setViewTransform(0, view, proj);

			
			// float view[16];
			// bx::mtxLookAt(view, eye, at);

			// float proj[16];
			// bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);
			// bgfx::setViewTransform(0, view, proj);

				
			bgfx::setViewTransform(0, view, proj);
			bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
				
				
			bgfx::setTransform(m_transform);
		
			bgfx::setVertexBuffer(m_dvbh);
			bgfx::setIndexBuffer(m_dibh);
			bgfx::setTexture(0, s_texColor,  m_textureColor);

			// Set render states.
			// bgfx::setState(0
			// 	       | BGFX_STATE_DEFAULT
			// 	       | BGFX_STATE_PT_TRISTRIP
			// 	);
			
			bgfx::submit(0, m_program);


//			m_player.setCamRotVec();
			m_player.update();
			
			// Advance to next frame.
			// Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

			return true;
		}

		return true;

	}

	Camera m_camera;
	Mouse m_mouse;
	
	float m_viewMtx[16];
	float m_projMtx[16];
	bgfx::DynamicVertexBufferHandle m_dvbh;
	bgfx::DynamicIndexBufferHandle m_dibh;
	bgfx::ProgramHandle m_program;
	bgfx::TextureHandle m_textureColor;
	bgfx::UniformHandle s_texColor;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	float                m_transform[16];

	Player m_player;
	
	Tile m_tilemap[5][5];
	std::vector<PosTexCoordVertex> m_vertexPool;
	std::vector<uint16_t> m_indices;

	entry::MouseState m_mouseState;
	
};

ENTRY_IMPLEMENT_MAIN(Test);
