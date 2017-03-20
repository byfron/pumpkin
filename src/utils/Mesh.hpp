#pragma once

#include <bgfx/bgfx.h>
#include <vector>
#include <graphics/TextureAtlas.hpp>
#include <graphics/Shader.hpp>
#include <common/debugdraw/debugdraw.h>
#include "VertexUtils.hpp"

namespace pumpkin {

// struct Aabb
// {
// 	float m_min[3];
// 	float m_max[3];
// };

// struct Obb
// {
// 	float m_mtx[16];
// };

// struct Sphere
// {
// 	float m_center[3];
// 	float m_radius;
// };

// struct Primitive
// {
// 	uint32_t m_startIndex;
// 	uint32_t m_numIndices;
// 	uint32_t m_startVertex;
// 	uint32_t m_numVertices;

// 	Sphere m_sphere;
// 	Aabb m_aabb;
// 	Obb m_obb;
// };

//typedef std::vector<Primitive> PrimitiveArray;

struct Group
{
	Group()
	{
		reset();
	}

	void reset()
	{
		m_dvbh.idx = bgfx::invalidHandle;
		m_dibh.idx = bgfx::invalidHandle;
//		m_prims.clear();
	}

	bgfx::DynamicVertexBufferHandle m_dvbh;
	bgfx::DynamicIndexBufferHandle m_dibh;

	// TODO: we need a way to keep vertices/indices somewhere
	std::vector<PosNormalTexCoordVertex> vertices;
	std::vector<uint16_t> indices;

//	bgfx::VertexBufferHandle m_dvbh;
//	bgfx::IndexBufferHandle m_dibh;
	
	// Sphere m_sphere;
	Aabb m_aabb;
	// Obb m_obb;
	// PrimitiveArray m_prims;
};

class MeshState
{
public:

	MeshState() {}	
	
	TextureAtlas::Ptr   m_texture;
	Shader::Ptr         m_shader;
	uint64_t            m_state = BGFX_STATE_DEFAULT;
	uint8_t             m_viewId = 0;
};

class Mesh {

public:

	typedef std::vector<Group> GroupArray;

	void unload()
		{
			for (GroupArray::const_iterator it = m_groups.begin(), itEnd = m_groups.end(); it != itEnd; ++it)
			{
				const Group& group = *it;
				bgfx::destroyDynamicVertexBuffer(group.m_dvbh);
//				bgfx::destroyVertexBuffer(group.m_dvbh);

				if (bgfx::isValid(group.m_dibh) )
				{
					bgfx::destroyDynamicIndexBuffer(group.m_dibh);
//					bgfx::destroyIndexBuffer(group.m_dibh);
				}
			}
			m_groups.clear();
		}

	void submit(const std::vector<MeshState> & stateVec, const float* mtx, uint16_t numMatrices) const
		{
			uint32_t cached = bgfx::setTransform(mtx, numMatrices);

			for (uint32_t pass = 0; pass < stateVec.size(); ++pass)
			{
				bgfx::setTransform(cached, numMatrices);

				const MeshState& state = stateVec[pass];
				bgfx::setState(state.m_state);

				const TextureAtlas::Ptr texture = state.m_texture;
				
				for (uint8_t tex = 0; tex < texture->m_num_textures; ++tex)
				{
					bgfx::setTexture(texture->m_stage[tex]
							 , texture->u_sampler[tex]
							 , texture->m_texture_handle[tex]
							 , texture->m_flags[tex]);
				}

				texture->setUniforms();

				for (GroupArray::const_iterator it = m_groups.begin(), itEnd = m_groups.end(); it != itEnd; ++it)
				{
					const Group& group = *it;
					bgfx::setIndexBuffer(group.m_dibh);
					bgfx::setVertexBuffer(group.m_dvbh);				
				
					bgfx::submit(state.m_viewId,
						     state.m_shader->getHandle(),
						     0, it != itEnd-1);

					// ddBegin(0);
					// ddDraw(group.m_aabb);
					// ddEnd();
				}
			}			
		}

	bgfx::VertexDecl m_decl;
	GroupArray m_groups;

};

}
