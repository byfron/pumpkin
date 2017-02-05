#ifndef SPRITEOBJECT_H_HEADER_GUARD
#define SPRITEOBJECT_H_HEADER_GUARD

#include "GraphicsObject.hpp"
#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include <utils/VertexUtils.hpp>
#include <string>
#include <Eigen/Dense>

namespace pumpkin {

class SpriteObjectFactory;
class AnimationFactory;	
	
class SpriteObject : public GraphicsObject {

public:

	friend class SpriteObjectFactory;
	friend class AnimationFactory;

	SpriteObject();
	SpriteObject(const std::string & config_file);
	
	~SpriteObject();
	void init();
	
	void update(float d);

	void destroyUniforms();
	void initialiseBuffers();
	void createUniforms();
	bool loadTextureAtlas(uint32_t);
	       
	static Eigen::MatrixXf getTowardsCameraRotation(Vec3f);
	

protected:
	
	bgfx::UniformHandle s_texColor;
	
	bgfx::UniformHandle u_flip;
	bgfx::UniformHandle u_texOffset;
	bgfx::UniformHandle u_lightPosRadius;

	int m_flipped;
	float m_atlas_offset[4];

	std::string m_texture;
	TextureAtlas::Ptr m_texture_atlas;

};

}
#endif
