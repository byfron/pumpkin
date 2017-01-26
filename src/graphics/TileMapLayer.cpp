#include "TileMapLayer.hpp"
#include "ResourceManager.hpp"
#include <utils/TileMapUtils.hpp>

namespace pumpkin {
	
TileMapLayer::TileMapLayer(const TileMapLayerProperties & properties)  :
	m_properties(properties) 
{
}

TileMapLayer::~TileMapLayer() {

	bgfx::destroyDynamicIndexBuffer(m_dibh);
	bgfx::destroyDynamicVertexBuffer(m_dvbh);

	destroyUniforms();       
}

void TileMapLayer::destroyUniforms() {
	
	bgfx::destroyUniform(u_texColor);
	bgfx::destroyUniform(u_lightPosRadius);
}

void TileMapLayer::init() {

	// refactor loadShader/loadTextureAtlas
	m_shader = ResourceManager::getResource<Shader>(m_properties.shader_id);
	m_texture_atlas = ResourceManager::getResource<TextureAtlas>(m_properties.atlas_id);
		
	// initialise graphics containers
	m_texture_atlas->init();
	m_shader->init();	

	initialiseBuffers();
	createUniforms();
}

void TileMapLayer::initialiseBuffers() {

	assert(m_vertexPool.size() > 0);

	int vertexCount = m_vertexPool.size();
	int indexCount = m_indices.size();
	
	const bgfx::Memory* mem;
	mem = bgfx::makeRef(&m_vertexPool[0], sizeof(PosNormalTexCoordVertex) * vertexCount);
	m_dvbh = bgfx::createDynamicVertexBuffer(mem, PosNormalTexCoordVertex::ms_decl);	
	mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * indexCount);	
	m_dibh = bgfx::createDynamicIndexBuffer(mem);
}

void TileMapLayer::createUniforms() {
	u_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
	u_lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, 1);

}

void TileMapLayer::addMeshObject(const TileProperties & tile) {

	switch(TileMapUtils::tileIdToMeshObjectType(tile.type)) {
	case MeshType::WALL_MESH:
		addWall(tile.row, tile.col, tile.type, m_tile_scale, tile.height);
		break;

	case MeshType::TILE_MESH:
		addTile(tile.row, tile.col, tile.type, m_tile_scale);
		break;
	}
}

template <typename T>
void TileMapLayer::addMeshToPool(const MeshObject<T> & mesh) {
	int starting_idx = m_vertexPool.size();
	for (auto v : mesh.m_vertex_pool) {
		m_vertexPool.push_back(v);
	}

	for (auto i : mesh.m_index_pool) {
		m_indices.push_back(i + starting_idx);
	}
}

void TileMapLayer::addWall(uint32_t row, uint32_t col, uint32_t type,
			   float scale, float height) {

	MeshObject<PosNormalTexCoordVertex> mesh = MeshFactory<PosNormalTexCoordVertex>::
		construct(MeshType::WALL_MESH,
			  MeshProperties(row, col, scale, 1.0, height));

	addMeshToPool(mesh);
}

void TileMapLayer::addTile(uint32_t row, uint32_t col, uint32_t type, float scale) {

	MeshObject<PosNormalTexCoordVertex> mesh = MeshFactory<PosNormalTexCoordVertex>::
		construct(MeshType::TILE_MESH,
			  MeshProperties(row, col, scale, 1.0, 0.0));
	addMeshToPool(mesh);
}

void TileMapLayer::update(float dt) {

	// LightingManager::getLights()
	// try light now
	float lightPosRadius[4] = { 4.0, 4.0, 1.0, 2.0};
	bgfx::setUniform(u_lightPosRadius, lightPosRadius, 1);
	
	bgfx::setVertexBuffer(m_dvbh);
	bgfx::setIndexBuffer(m_dibh);
	bgfx::setTexture(0, u_texColor,  m_texture_atlas->getHandle());				
	bgfx::submit(0, m_shader->getHandle());			
}

}
