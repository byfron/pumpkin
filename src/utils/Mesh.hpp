#pragma once

#include <vector>
#include <mesh.pb.h>

namespace pumpkin {
	
template <typename T>
class Mesh {
public:
	void addVertex(const T & vertex) {
		m_vertex_pool.push_back(vertex);
	}

	void addIndex(int index) {
		m_index_pool.push_back(index);
	}

	int64_t getType() { return m_mesh_type; }

	std::vector<int> m_index_pool;
	std::vector<T> m_vertex_pool;
	int64_t m_mesh_type;
};

}
