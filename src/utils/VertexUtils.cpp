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

bgfx::VertexDecl PosTexCoordVertex::ms_decl;
bgfx::VertexDecl PosNormalTexCoordVertex::ms_decl;
bgfx::VertexDecl PosNormalTangentTexcoordVertex::ms_decl;


namespace VertexUtils {

MeshObject<PosNormalTexCoordVertex> constructTile(const MeshProperties & prop) {

	MeshObject<PosNormalTexCoordVertex> mesh;
	float row = prop.row * prop.scale;
	float col = prop.col * prop.scale;
	float s_tsize = prop.width * prop.scale;
	float z_pos = 0.0; //prop.z_pos
	
	PosNormalTexCoordVertex v_tile[4] = {
		{row, col + s_tsize, z_pos, NORMAL_NEGZ, 0, 0x7fff},
		{row + s_tsize, col + s_tsize, z_pos, NORMAL_NEGZ, 0x7fff, 0x7fff},
		{row + s_tsize, col, z_pos, NORMAL_NEGZ, 0x7fff, 0},
		{row, col, z_pos, NORMAL_NEGZ, 0, 0},
	};
	
	int i_tile[6] = {
		3,2,1,
		1,0,3
	};			

	for (int i = 0; i < 4; i++) {
		mesh.addVertex(v_tile[i]);
	}

	for (int i = 0; i < 6; i++) {
		mesh.addIndex(i_tile[i]);
	}

	return mesh;
}
	
// Move this to a static function in vertex_utils
MeshObject<PosNormalTexCoordVertex> constructWall(const MeshProperties & prop) {

	MeshObject<PosNormalTexCoordVertex> mesh;
	float row = prop.row * prop.scale;
	float col = prop.col * prop.scale;
	float s_tsize = prop.width * prop.scale;
	float h = prop.height;

	// 20 vertices. 4 per each side and 4 for the top
	PosNormalTexCoordVertex v_wall[20] = {
		//top -z normal
		{row, col + s_tsize, -h, NORMAL_NEGZ, 0, 0x7fff},
		{row + s_tsize, col + s_tsize, -h, NORMAL_NEGZ, 0x7fff, 0x7fff},
		{row + s_tsize, col, -h, NORMAL_NEGZ, 0x7fff, 0},
		{row, col, -h, NORMAL_NEGZ, 0, 0},

		//side A
		{row, col + s_tsize, -h, NORMAL_POSX, 0x7fff, 0x7fff},     // A(0,1,-1)---B(0,0,-1)
		{row, col,           -h, NORMAL_POSX, 0x7fff,0},           //  |             |
		{row, col + s_tsize,  0, NORMAL_POSX, 0, 0x7fff},          //  |             |
		{row, col,            0, NORMAL_POSX, 0, 0},               // C(0,1,0) ---D(0,0,0)

		//side B
		{row,           col, -h, NORMAL_POSY, 0x7fff, 0x7fff},
		{row + s_tsize, col, -h, NORMAL_POSY, 0x7fff, 0},
		{row,           col,  0, NORMAL_POSY, 0, 0x7fff},
		{row + s_tsize, col,  0, NORMAL_POSY, 0, 0},

		//side C
		{row + s_tsize, col + s_tsize, -h, NORMAL_NEGX, 0x7fff, 0x7fff}, //A(1,1,1)---B(1,0,1)
		{row + s_tsize, col,           -h, NORMAL_NEGX, 0x7fff, 0},      //C(1,1,0)---D(1,0,0)
		{row + s_tsize, col + s_tsize,  0, NORMAL_NEGX, 0, 0x7fff},
		{row + s_tsize, col,            0, NORMAL_NEGX, 0, 0},
			
		//side D
		{row,           col + s_tsize, -h, NORMAL_NEGY, 0x7fff, 0x7fff}, //A(0,1,1)---B(1,1,1)
		{row + s_tsize, col + s_tsize, -h, NORMAL_NEGY, 0x7fff, 0},      //C(0,1,0)---D(1,1,0)
		{row          , col + s_tsize,  0, NORMAL_NEGY, 0, 0x7fff},
		{row + s_tsize, col + s_tsize,  0, NORMAL_NEGY, 0, 0}
	};

	int i_wall[30] = {
		3,2,1, //top
		1,0,3,
			
		4,6,7, //side A
		7,5,4,
			
		8,10,11, //side B
		11,9,8,

		13,15,14, //side C
		14,12,13,

		17,19,18, //side D
		18,16,17
	};
		
	for (int i = 0; i < 20; i++) {
		mesh.addVertex(v_wall[i]);
	}

	for (int i = 0; i < 30; i++) {
		mesh.addIndex(i_wall[i]);
	}


	return mesh;
}


MeshObject<PosNormalTexCoordVertex> constructVPlane(const MeshProperties & prop) {

	MeshObject<PosNormalTexCoordVertex> mesh;

	float meshsize = prop.width * prop.scale;	
	int16_t tsize = 0x7fff/4;//prop.texture_gridsize;		
	float height = prop.height;
	float width = prop.width;
	
	PosNormalTexCoordVertex v_plane[] =
	{
		// Horizonally aligned	
		{ 0.0f,    width/2.0f, 0.0f, packF4u( 0.0f, 0.0f, 1.0f), tsize, tsize},
		{ 0.0f,   -width/2.0f, 0.0f, packF4u( 0.0f, 0.0f, 1.0f),  0, tsize},
		{-height,  width/2.0f, 0.0f, packF4u( 0.0f, 0.0f, 1.0f), tsize, 0},
		{-height, -width/2.0f, 0.0f, packF4u( 0.0f, 0.0f, 1.0f), 0, 0}
	};
	
	int i_plane[4] = {
		0,1,2,3
	};

	for (int i = 0; i < 4; i++) {
		mesh.addVertex(v_plane[i]);
	}

	for (int i = 0; i < 4; i++) {
		mesh.addIndex(i_plane[i]);
	}

	return mesh;			
}
	
}
}
