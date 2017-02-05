#include "TileMapUtils.hpp"

namespace pumpkin {
namespace TileMapUtils {

	MeshObjectType tileIdToMeshObjectType(uint32_t tile_id) {
		switch(tile_id) {
		case 0:
			return pumpkin::MeshObjectType::TILE_MESH;
			break;
		case 1:
			return pumpkin::MeshObjectType::WALL_MESH;
			break;
		}
	}


	std::vector<Vec2i> tileIdToAtlasFrameCoords(uint32_t tile_id) {

		std::vector<Vec2i> coords;
		
		switch(tile_id) {
		case 0:
			coords.push_back(Vec2i(0,0));
			break;

		case 1:
			coords.push_back(Vec2i(0,2));
			coords.push_back(Vec2i(0,1));
			break;

		};

		return coords;
	}

	Vec3f projectCameraPointInGround(const Vec3f & point) {

		// cast a ray from camera and intersect with Z=0 plane
		return Vec3f(point(0), point(1), 0.0f);
	}
			
	Vec3f projectPointInGround(const Vec3f & point) {
		return Vec3f(point(0), point(1), 0.0f);
	}

}
}
