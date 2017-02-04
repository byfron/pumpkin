#pragma once

#include <common/math.hpp>
#include "MeshFactory.hpp"

namespace pumpkin {
namespace TileMapUtils {
	MeshType tileIdToMeshObjectType(uint32_t tile_id);
	std::vector<Vec2i> tileIdToAtlasFrameCoords(uint32_t tile_id);	
	Vec3f projectCameraPointInGround(const Vec3f & point);
	Vec3f projectPointInGround(const Vec3f & point);
}
}
