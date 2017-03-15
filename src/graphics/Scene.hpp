#pragma once

#include <utils/FbxLoader.hpp>
#include "GraphicsObject.hpp"

namespace pumpkin {

	class SceneObject : public GraphicsObject<PosNormalTexCoordVertex> {

	public:

		SceneObject();
		void loadFromFbxFile(std::string file);

		void init();
		void update(float d);

	protected:

		std::vector<FbxLoader::FMesh> m_meshes;
		//std::vector<Light> m_lights;

	};
}
