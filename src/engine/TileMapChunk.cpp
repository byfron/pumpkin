#include "TileMapChunk.hpp"

TileMapChunk::TileMapChunk(uint32_t row, uint32_t col,
			   uint16_t width, uint16_t height) : tsize(1.0f) {


	// Create vertex stream declaration.
	//bgfx::PosNormalTexcoordVertex::init();
	
	float scale = 0.5;
	for (uint32_t r = row; r < height + row; r++) {
		for (uint32_t c = col; c < width + col; c++) {
			addTile(r, c, 0, scale);
		}
	}

	for (uint32_t r = 0; r < 5; r++)
		addWall(0, r, 0, scale, 0.5);

	int vertexCount = m_vertexPool.size();
	int indexCount = m_indices.size();


	u_lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, 1);
	
	const bgfx::Memory* mem;
	mem = bgfx::makeRef(&m_vertexPool[0], sizeof(PosNormalTexCoordVertex) * vertexCount);
	m_dvbh = bgfx::createDynamicVertexBuffer(mem, PosNormalTexCoordVertex::ms_decl);	
	mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * indexCount);	
	m_dibh = bgfx::createDynamicIndexBuffer(mem);
	
	s_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
	m_program  = loadProgram("vs_terrain", "fs_terrain");
	m_textureColor = loadTexture("textures/test.ktx");
}

void TileMapChunk::init() {


}

TileMapChunk::~TileMapChunk() {
	bgfx::destroyDynamicIndexBuffer(m_dibh);
	bgfx::destroyDynamicVertexBuffer(m_dvbh);
	bgfx::destroyProgram(m_program);
	bgfx::destroyTexture(m_textureColor);
	bgfx::destroyUniform(s_texColor);
	bgfx::destroyUniform(u_lightPosRadius);
}

void TileMapChunk::addWall(uint32_t row, uint32_t col, uint32_t type,
			   float scale, float height) {

	PosNormalTexCoordVertex v_wall[48] = {
		{ scale*row +  0.0f,  scale*col + scale*tsize,  0.0f,          packF4u( 0.0f,  0.0f,  1.0f),  0, 0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  0.0f,    packF4u( 0.0f,  0.0f,  1.0f),0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  0.0f,          packF4u( 0.0f,  0.0f,  1.0f),0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  0.0f,                packF4u( 0.0f,  0.0f,  1.0f), 0, 0 },
		{ scale*row +  0.0f,  scale*col + scale*tsize,  -height,       packF4u( 0.0f,  0.0f,  1.0f), 0,  0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  -height, packF4u( 0.0f,  0.0f,  1.0f), 0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  -height,       packF4u( 0.0f,  0.0f,  1.0f), 0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  -height,             packF4u( 0.0f,  0.0f,  1.0f), 0, 0 },

		{ scale*row +  0.0f,  scale*col + scale*tsize,  0.0f,          packF4u( 0.0f,  0.0f, -1.0f),  0, 0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  0.0f,    packF4u( 0.0f,  0.0f, -1.0f),0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  0.0f,          packF4u( 0.0f,  0.0f, -1.0f),0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  0.0f,                packF4u( 0.0f,  0.0f, -1.0f), 0, 0 },
		{ scale*row +  0.0f,  scale*col + scale*tsize,  -height,       packF4u( 0.0f,  0.0f, -1.0f), 0,  0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  -height, packF4u( 0.0f,  0.0f, -1.0f), 0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  -height,       packF4u( 0.0f,  0.0f, -1.0f), 0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  -height,             packF4u( 0.0f,  0.0f, -1.0f), 0, 0 },
		
		{ scale*row +  0.0f,  scale*col + scale*tsize,  0.0f,          packF4u( 0.0f,  1.0f,  0.0f),  0, 0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  0.0f,    packF4u( 0.0f,  1.0f,  0.0f),0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  0.0f,          packF4u( 0.0f,  1.0f,  0.0f),0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  0.0f,                packF4u( 0.0f,  1.0f,  0.0f), 0, 0 },
		{ scale*row +  0.0f,  scale*col + scale*tsize,  -height,       packF4u( 0.0f,  1.0f,  0.0f), 0,  0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  -height, packF4u( 0.0f,  1.0f,  0.0f), 0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  -height,       packF4u( 0.0f,  1.0f,  0.0f), 0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  -height,             packF4u( 0.0f,  1.0f,  0.0f), 0, 0 },

		{ scale*row +  0.0f,  scale*col + scale*tsize,  0.0f,          packF4u( 0.0f, -1.0f,  1.0f),  0, 0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  0.0f,    packF4u( 0.0f, -1.0f,  1.0f),0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  0.0f,          packF4u( 0.0f, -1.0f,  1.0f),0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  0.0f,                packF4u( 0.0f, -1.0f,  1.0f), 0, 0 },
		{ scale*row +  0.0f,  scale*col + scale*tsize,  -height,       packF4u( 0.0f, -1.0f,  1.0f), 0,  0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  -height, packF4u( 0.0f, -1.0f,  1.0f), 0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  -height,       packF4u( 0.0f, -1.0f,  1.0f), 0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  -height,             packF4u( 0.0f, -1.0f,  1.0f), 0, 0 },

		{ scale*row +  0.0f,  scale*col + scale*tsize,  0.0f,          packF4u( 1.0f,  0.0f,  0.0f),  0, 0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  0.0f,    packF4u( 1.0f,  0.0f,  0.0f),0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  0.0f,          packF4u( 1.0f,  0.0f,  0.0f),0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  0.0f,                packF4u( 1.0f,  0.0f,  0.0f), 0, 0 },
		{ scale*row +  0.0f,  scale*col + scale*tsize,  -height,       packF4u( 1.0f,  0.0f,  0.0f), 0,  0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  -height, packF4u( 1.0f,  0.0f,  0.0f), 0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  -height,       packF4u( 1.0f,  0.0f,  0.0f), 0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  -height,             packF4u( 1.0f,  0.0f,  0.0f), 0, 0 },

		{ scale*row +  0.0f,  scale*col + scale*tsize,  0.0f,          packF4u(-1.0f,  0.0f,  1.0f),  0, 0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  0.0f,    packF4u(-1.0f,  0.0f,  1.0f),0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  0.0f,          packF4u(-1.0f,  0.0f,  1.0f),0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  0.0f,                packF4u(-1.0f,  0.0f,  1.0f), 0, 0 },
		{ scale*row +  0.0f,  scale*col + scale*tsize,  -height,       packF4u(-1.0f,  0.0f,  1.0f), 0,  0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  -height, packF4u(-1.0f,  0.0f,  1.0f), 0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  -height,       packF4u(-1.0f,  0.0f,  1.0f), 0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  -height,             packF4u(-1.0f,  0.0f,  1.0f), 0, 0 }
	};

	int tl1 = m_vertexPool.size();
	int tr1 = tl1+1;
	int br1 = tr1+1;
	int bl1 = br1+1;
	int tl2 = bl1+1;
	int tr2 = tl2+1;
	int br2 = tr2+1;
	int bl2 = br2+1;
	
	m_vertexPool.push_back(v_wall[0]); //tl1
	m_vertexPool.push_back(v_wall[1]); //tr1
	m_vertexPool.push_back(v_wall[2]); //br1
	m_vertexPool.push_back(v_wall[3]); //bl1
	m_vertexPool.push_back(v_wall[4]); //tl2
	m_vertexPool.push_back(v_wall[5]); //tr2
	m_vertexPool.push_back(v_wall[6]); //br2
	m_vertexPool.push_back(v_wall[7]); //bl2

	//top side
	m_indices.push_back(tl2);
	m_indices.push_back(tr2);
	m_indices.push_back(tr1);

	m_indices.push_back(tr1);
	m_indices.push_back(tl1);
	m_indices.push_back(tl2);

	//back side
	m_indices.push_back(br2); 
	m_indices.push_back(bl2);
	m_indices.push_back(br1);

	m_indices.push_back(bl1);
	m_indices.push_back(br1);
	m_indices.push_back(bl2);

	// left side
	m_indices.push_back(bl2); 
	m_indices.push_back(tl2);
	m_indices.push_back(tl1);

	m_indices.push_back(tl1); 
	m_indices.push_back(bl1);
	m_indices.push_back(bl2);

	// right side
	m_indices.push_back(tr2); 
	m_indices.push_back(br2);
	m_indices.push_back(tr1);

	m_indices.push_back(br1); 
	m_indices.push_back(tr1);
	m_indices.push_back(br2);

	// bottom side
	m_indices.push_back(tr2);
	m_indices.push_back(tl2);
	m_indices.push_back(bl2);
	
	m_indices.push_back(tr2);
	m_indices.push_back(bl2);
	m_indices.push_back(br2);

}

void TileMapChunk::addTile(uint32_t row, uint32_t col, uint32_t type, float scale) {

	// check neighbor tiles
	PosNormalTexCoordVertex v_tile[4] = {
		{ scale*row +  0.0f,  scale*col + scale*tsize,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, 0x7fff },
		{ scale*row + scale*tsize,  scale*col + scale*tsize,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0x7fff, 0x7fff },
		{ scale*row + scale*tsize,  scale*col +  0.0f,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0x7fff, 0 },
		{ scale*row +  0.0f,  scale*col +  0.0f,  0.0f, packF4u( 0.0f,  0.0f,  1.0f), 0, 0 }
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
}

void TileMapChunk::update(float delta) {

//	LightingManager::getLights()

	// try light now
	float lightPosRadius[4] = { 4.0, 4.0, 1.0, 2.0};
	bgfx::setUniform(u_lightPosRadius, lightPosRadius, 1);

	
	bgfx::setVertexBuffer(m_dvbh);
	bgfx::setIndexBuffer(m_dibh);
	bgfx::setTexture(0, s_texColor,  m_textureColor);				
	bgfx::submit(0, m_program);			
}
