#pragma once

#include "GraphicsObject.hpp"
#include "TextureAtlas.hpp"
#include <scene.pb.h>

namespace pumpkin {

	class Scene : public GraphicsObject {

	public:

		typedef voyage::SceneCfg Config;
		typedef std::shared_ptr<Scene> Ptr;

		Scene(const Config &);

		void destroyUniforms();
		void createUniforms();
		
		void loadFromFbxNode(FbxNode*);
		void update(float d);
		void render(float d);

	protected:

	};
}
