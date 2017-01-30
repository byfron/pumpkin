#ifndef VERTEXUTILS_H_HEADER_GUARD
#define VERTEXUTILS_H_HEADER_GUARD

#include <common/common.h>
#include <common/bgfx_utils.h>

namespace pumpkin {

#define NORMAL_POSX packF4u( 1.0f,  0.0f,  0.0f)
#define NORMAL_NEGX packF4u(-1.0f,  0.0f,  0.0f)
#define NORMAL_POSY packF4u( 0.0f,  1.0f,  0.0f)
#define NORMAL_NEGY packF4u( 0.0f, -1.0f,  0.0f)
#define NORMAL_POSZ packF4u( 0.0f,  0.0f,  1.0f)
#define NORMAL_NEGZ packF4u( 0.0f,  0.0f, -1.0f)

template <typename T>
class MeshObject;
struct MeshProperties;

uint32_t packUint32(uint8_t _x, uint8_t _y, uint8_t _z, uint8_t _w);
uint32_t packF4u(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f);

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


struct PosNormalTexCoordVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_normal;
	int16_t m_u;
	int16_t m_v;

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal,   4, bgfx::AttribType::Uint8, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};

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

namespace VertexUtils {

MeshObject<PosNormalTangentTexcoordVertex> constructTile(const MeshProperties & prop);	
MeshObject<PosNormalTangentTexcoordVertex> constructWall(const MeshProperties & prop);
MeshObject<PosNormalTangentTexcoordVertex> constructVPlane(const MeshProperties & prop);
}

}
#endif
