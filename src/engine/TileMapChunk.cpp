#include "TileMapChunk.hpp"

TileMapChunk::TileMapChunk(uint32_t row, uint32_t col,
			   uint16_t width, uint16_t height) : tsize(1.0f) {
	
	for (uint32_t r = row; r < height + row; r++) {
		for (uint32_t c = col; c < width + col; c++) {
			addTile(r, c, 0);
		}
	}


	s_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
	m_program  = loadProgram("vs_terrain", "fs_terrain");
	m_textureColor = loadTexture("textures/test.ktx");

}

void TileMapChunk::init() {}

TileMapChunk::~TileMapChunk() {
	bgfx::destroyDynamicIndexBuffer(m_dibh);
	bgfx::destroyDynamicVertexBuffer(m_dvbh);
	bgfx::destroyProgram(m_program);
	bgfx::destroyTexture(m_textureColor);
	bgfx::destroyUniform(s_texColor);
}

void TileMapChunk::addTile(uint32_t row, uint32_t col, uint32_t type) {

	// check neighbor tiles
	PosTexCoordVertex v_tile[4] = {
		{ row +  0.0f,  col + tsize,  0.0f, 0, 0x7fff },
		{ row + tsize,  col + tsize,  0.0f, 0x7fff, 0x7fff },
		{ row + tsize,  col +  0.0f,  0.0f, 0x7fff, 0 },
		{ row +  0.0f,  col +  0.0f,  0.0f, 0, 0 }
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

	int vertexCount = m_vertexPool.size();
	int indexCount = m_indices.size();
	
	const bgfx::Memory* mem;
	mem = bgfx::makeRef(&m_vertexPool[0], sizeof(PosTexCoordVertex) * vertexCount);
	m_dvbh = bgfx::createDynamicVertexBuffer(mem, PosTexCoordVertex::ms_decl);	
	mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * indexCount);	
	m_dibh = bgfx::createDynamicIndexBuffer(mem);
}

void TileMapChunk::update(float delta) {

	bgfx::setVertexBuffer(m_dvbh);
	bgfx::setIndexBuffer(m_dibh);
	bgfx::setTexture(0, s_texColor,  m_textureColor);				
	bgfx::submit(0, m_program);			
}
