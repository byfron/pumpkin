#ifndef TILEMAPCHUNK_H_HEADER_GUARD
#define TILEMAPCHUNK_H_HEADER_GUARD

#include <utils/VertexUtils.hpp>
#include <engine/ResourceManager.hpp>
#include <vector>
#include <memory>

/*
  Chunks serve for dividing the space in smaller regions to draw
  and to divide the space per shader programs.
*/

class TileMapChunk : public Resource {

public:
	TileMapChunk(uint32_t row, uint32_t col,
		     uint16_t width, uint16_t height);
	~TileMapChunk();
	void init();
	void addTile(uint32_t row, uint32_t col, uint32_t type);
	void update(float delta);

	typedef std::shared_ptr<TileMapChunk> Ptr;
	
protected:
	
	bgfx::DynamicVertexBufferHandle m_dvbh;
	bgfx::DynamicIndexBufferHandle m_dibh;
	bgfx::ProgramHandle m_program;
	bgfx::TextureHandle m_textureColor;
	bgfx::UniformHandle s_texColor;

	std::vector<PosTexCoordVertex> m_vertexPool;
	std::vector<uint16_t> m_indices;

	float tsize;
	
	// Texture m_texture;
	// Shader m_shader;
	
};

#endif
