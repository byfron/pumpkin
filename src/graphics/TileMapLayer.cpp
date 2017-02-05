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

	bgfx::destroyUniform(u_texNormal);
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

	createUniforms();
}

void TileMapLayer::initialiseBuffers() {

	assert(m_vertexPool.size() > 0);

	int vertexCount = m_vertexPool.size();
	int indexCount = m_indices.size();

	const bgfx::Memory* mem;
	mem = bgfx::makeRef(&m_vertexPool[0], sizeof(PosNormalTangentTexcoordVertex) * vertexCount);
	m_dvbh = bgfx::createDynamicVertexBuffer(mem, PosNormalTangentTexcoordVertex::ms_decl);
	mem = bgfx::makeRef(&m_indices[0], sizeof(uint16_t) * indexCount);
	m_dibh = bgfx::createDynamicIndexBuffer(mem);
}

void TileMapLayer::createUniforms() {
	u_texNormal = bgfx::createUniform("s_texNormal",  bgfx::UniformType::Int1);
	u_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
	u_lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, 1);

}

void TileMapLayer::addMeshObject(uint32_t row, uint32_t col, uint32_t type,
				 float scale, float height) {
	
	std::vector<Vec2i> atlas_frame_coords = TileMapUtils::tileIdToAtlasFrameCoords(type);

	switch(TileMapUtils::tileIdToMeshObjectType(type)) {
	case MeshObjectType::WALL_MESH:
		addWall(row, col, scale, height, m_texture_atlas->getAtlasFrames(atlas_frame_coords));
		break;

	case MeshObjectType::TILE_MESH:
		addTile(row, col, scale, m_texture_atlas->getAtlasFrames(atlas_frame_coords));
		break;
	}
}

template <typename T>
void TileMapLayer::addMeshToPool(const Mesh<T> & mesh) {
	int starting_idx = m_vertexPool.size();
	for (auto v : mesh.m_vertex_pool) {
		m_vertexPool.push_back(v);
	}

	for (auto i : mesh.m_index_pool) {
		m_indices.push_back(i + starting_idx);
	}
}

void TileMapLayer::addWall(uint32_t row, uint32_t col,
			   float scale, float height,
			   const std::vector<AtlasFrame> & frame_list) {

	Mesh<PosNormalTangentTexcoordVertex> mesh = MeshFactory<PosNormalTangentTexcoordVertex>::
		construct(MeshObjectType::WALL_MESH,
			  MeshProperties(row, col, scale, 1.0, height,
					 frame_list));

	addMeshToPool(mesh);
}

void TileMapLayer::addTile(uint32_t row, uint32_t col, float scale,
			   const std::vector<AtlasFrame> & frame_list) {

	Mesh<PosNormalTangentTexcoordVertex> mesh = MeshFactory<PosNormalTangentTexcoordVertex>::
		construct(MeshObjectType::TILE_MESH,
			  MeshProperties(row, col, scale, 1.0, 0.0,
					 frame_list));

	addMeshToPool(mesh);
}

void TileMapLayer::update(float dt) {

	// LightingManager::getLights()
	// try light now
	float lightPosRadius[4] = { 2.0, 2.0, 5.0, 10.0};
	bgfx::setUniform(u_lightPosRadius, lightPosRadius, 1);

	bgfx::setVertexBuffer(m_dvbh);
	bgfx::setIndexBuffer(m_dibh);
	bgfx::setTexture(0, u_texColor,  m_texture_atlas->getColorHandle());
	if (m_texture_atlas->hasNormalMap())
		bgfx::setTexture(1, u_texNormal, m_texture_atlas->getNormalHandle());
	bgfx::submit(0, m_shader->getHandle());			
}

}
