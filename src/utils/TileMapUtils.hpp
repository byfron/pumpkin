#pragma once

namespace pumpkin {
namespace TileMapUtils {

	MeshType tileIdToMeshObjectType(uint32_t tile_id) {
		switch(tile_id) {
		case 0:
			return MeshType::TILE_MESH;
			break;
		case 1:
			return MeshType::WALL_MESH;
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

}
}
