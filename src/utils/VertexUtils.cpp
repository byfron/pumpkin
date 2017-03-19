#include "VertexUtils.hpp"
#include "MeshFactory.hpp"

namespace pumpkin {

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

uint32_t packF4u(float _x, float _y, float _z, float _w)
{
	const uint8_t xx = uint8_t(_x*127.0f + 128.0f);
	const uint8_t yy = uint8_t(_y*127.0f + 128.0f);
	const uint8_t zz = uint8_t(_z*127.0f + 128.0f);
	const uint8_t ww = uint8_t(_w*127.0f + 128.0f);
	return packUint32(xx, yy, zz, ww);
}

bgfx::VertexDecl PosColorVertex::ms_decl;
bgfx::VertexDecl PosTexCoordVertex::ms_decl;
bgfx::VertexDecl PosNormalTexCoordVertex::ms_decl;
bgfx::VertexDecl PosNormalTangentTexcoordVertex::ms_decl;


namespace VertexUtils {

	Mesh constructVPlane(float width, float height) {

	AtlasFrame::Corner top_left = AtlasFrame::Corner(0,0);
	AtlasFrame::Corner bottom_right = AtlasFrame::Corner(0x7fff,0x7fff);
		 
	PosNormalTangentTexcoordVertex v_plane[] =
	{
	// Horizonally aligned
	{ -height/2.0f,width/2.0f, 0.0f,packF4u(0.0f,0.0f, 1.0f),0, bottom_right(0), bottom_right(1)},
	{ -height/2.0f,-width/2.0f, 0.0f,packF4u(0.0f,0.0f, 1.0f),0, top_left(0), bottom_right(1)},
	{ height/2.0f,width/2.0f,0.0f,packF4u(0.0f,0.0f, 1.0f),0, bottom_right(0), top_left(1)},
	{ height/2.0f,-width/2.0f,0.0f,packF4u(0.0f,0.0f, 1.0f),0, top_left(0), top_left(1)},

	};

	int i_plane[4] = {
		0,1,2,3
	};

	Mesh mesh;
	Group g;
	mesh.m_decl = PosNormalTangentTexcoordVertex::ms_decl;
	g.m_dvbh = bgfx::createDynamicVertexBuffer(bgfx::makeRef(v_plane, sizeof(v_plane) ),
						   mesh.m_decl);
	g.m_dibh = bgfx::createDynamicIndexBuffer(bgfx::makeRef(i_plane, sizeof(i_plane) ) );
	mesh.m_groups.push_back(g);

	return mesh;
}

}
}
