#pragma once

#include "Mesh.hpp"
#include <utils/VertexUtils.hpp>
#include <utils/Configuration.hpp>
#include <graphics/TextureAtlas.hpp>
#include <vector>
#include <fbxsdk.h>
#include <mesh.pb.h>

namespace pumpkin {

enum class MeshObjectType {
	TILE_MESH,
	WALL_MESH,
	PLANE_MESH,
	FBX_MESH
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

Mesh generateMesh(FbxNode* pNode);
std::vector<Mesh> loadMeshes(FbxNode *pRootNode);

template <typename T>
class MeshFactory {
public:

	typedef Configuration<voyage::MeshCfg> Config;

	MeshFactory(const std::string & config_file) :
		m_config(Config(config_file)) {
	}

	//possibility T::Properties
	static Mesh construct(MeshObjectType id, const MeshProperties & properties) {
		switch(id) {
		case MeshObjectType::PLANE_MESH:
			return  VertexUtils::constructVPlane(0.5, 0.5);
			break;
		};
	}

private:

	Config m_config;

};

}
