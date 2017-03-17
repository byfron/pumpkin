#pragma once

#include "GraphicsObject.hpp"
#include "TextureAtlas.hpp"

namespace pumpkin {

	class SceneObject : public GraphicsObject<PosNormalTexCoordVertex> {

	public:

		typedef std::shared_ptr<SceneObject> Ptr;

		SceneObject();

		void destroyUniforms();
		void createUniforms();
		
		void loadFromFbxNode(FbxNode*);
		void init();
		void update(float d);

	protected:

		std::vector<FMesh> m_meshes;
		bgfx::UniformHandle u_texColor;		
		TextureAtlas::Ptr m_texture;
		//std::vector<Light> m_lights;

	};
}
