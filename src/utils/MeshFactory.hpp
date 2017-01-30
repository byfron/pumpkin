#pragma once

#include <utils/VertexUtils.hpp>
#include <vector>

namespace pumpkin {

enum class MeshType {
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
		       int16_t ts_x = 0x7fff,
		       int16_t ts_y = 0x7fff) :
		row(r), col(c), scale(s), width(w), height(h),
		tsize_x(ts_x), tsize_y(ts_y) {}

	uint32_t row;
	uint32_t col;
	float width;
	float height;
	float scale;
	int16_t tsize_x;
	int16_t tsize_y;
};

template <typename T>
class MeshObject {
public:
	void addVertex(const T & vertex) {
		m_vertex_pool.push_back(vertex);
	}

	void addIndex(int index) {
		m_index_pool.push_back(index);
	}

	std::vector<int> m_index_pool;
	std::vector<T> m_vertex_pool;
};

template <typename T>
class MeshFactory {
public:
	MeshFactory() {
	}

	//possibility T::Properties
	static MeshObject<T> construct(MeshType id, const MeshProperties & properties) {
		switch(id) {
		case MeshType::TILE_MESH:
			return VertexUtils::constructTile(properties);
			break;
		case MeshType::WALL_MESH:
			return  VertexUtils::constructWall(properties);
			break;
		case MeshType::PLANE_MESH:
			return  VertexUtils::constructVPlane(properties);
			break;
		};
	}

};

}
