#pragma once

#include <utils/MeshFactory.hpp>
#include "TextureAtlas.hpp"
#include "Shader.hpp"
#include <Eigen/Dense>
#include <memory>

namespace pumpkin {

struct TileMapLayerProperties {

	TileMapLayerProperties(int w, int h, uint32_t atlas, uint32_t shader) :
		width(w), height(h), atlas_id(atlas), shader_id(shader) {}
	int width;
	int height;
	uint32_t shader_id;
	uint32_t atlas_id;
};

class TileMapLayer {

public:
	typedef std::shared_ptr<TileMapLayer> Ptr;

	TileMapLayer(const TileMapLayerProperties & properties);
	~TileMapLayer();

	void addMeshObject(uint32_t row, uint32_t col, uint32_t type,
			   float scale, float height);

	template <typename T>
	void addMeshToPool(const Mesh<T> & mesh);

	void addWall(uint32_t row, uint32_t col,
		     float scale, float height,
		     const std::vector<AtlasFrame> &);

	void addTile(uint32_t row, uint32_t col,
		     float scale, const std::vector<AtlasFrame> &);

	void init();
	void createUniforms();
	void destroyUniforms();
	void initialiseBuffers();

	void update(float dt);

protected:

	TextureAtlas::Ptr m_texture_atlas;
	Shader::Ptr m_shader;

	bgfx::DynamicVertexBufferHandle m_dvbh;
	bgfx::DynamicIndexBufferHandle m_dibh;

	bgfx::UniformHandle u_texColor;
	bgfx::UniformHandle u_texNormal;
	bgfx::UniformHandle u_lightPosRadius;

	std::vector<PosNormalTangentTexcoordVertex> m_vertexPool;
	std::vector<uint16_t> m_indices;

	TileMapLayerProperties m_properties;

};

}
