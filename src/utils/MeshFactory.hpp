#pragma once

#include "Mesh.hpp"
#include <utils/VertexUtils.hpp>
#include <graphics/TextureAtlas.hpp>
#include <vector>

namespace pumpkin {

enum class MeshObjectType {
	TILE_MESH,
	WALL_MESH,
	PLANE_MESH
};
	
struct MeshProperties {
	MeshProperties(uint32_t r,
		       uint32_t c,
		       float s,
		       float w,
		       float h,
		       const std::vector<AtlasFrame> &af = std::vector<AtlasFrame>()) :
		row(r), col(c), scale(s),
		width(w), height(h), atlas_frames(af) {}

	uint32_t row;
	uint32_t col;
	float width;
	float height;
	float scale;
	std::vector<AtlasFrame> atlas_frames;
};
       
template <typename T>
class MeshFactory {
public:
	MeshFactory() {
	}

	//possibility T::Properties
	static Mesh<T> construct(MeshObjectType id, const MeshProperties & properties) {
		switch(id) {
		case MeshObjectType::TILE_MESH:
			return VertexUtils::constructTile(properties);
			break;
		case MeshObjectType::WALL_MESH:
			return  VertexUtils::constructWall(properties);
			break;
		case MeshObjectType::PLANE_MESH:
			return  VertexUtils::constructVPlane(properties);
			break;
		};
	}

};

}
