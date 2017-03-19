#pragma once

#include <bgfx/bgfx.h>
#include <vector>


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
		m_vbh.idx = bgfx::invalidHandle;
		m_ibh.idx = bgfx::invalidHandle;
//		m_prims.clear();
	}

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	// Sphere m_sphere;
	// Aabb m_aabb;
	// Obb m_obb;
	// PrimitiveArray m_prims;
};

struct MeshState
{
	struct Texture
	{
		uint32_t            m_flags;
		bgfx::UniformHandle m_sampler;
		bgfx::TextureHandle m_texture;
		uint8_t             m_stage;
	};

	Texture             m_textures[4];
	uint64_t            m_state;
	bgfx::ProgramHandle m_program;
	uint8_t             m_numTextures;
	uint8_t             m_viewId;
};

class Mesh {

public:

	typedef std::vector<Group> GroupArray;

	void unload()
		{
			for (GroupArray::const_iterator it = m_groups.begin(), itEnd = m_groups.end(); it != itEnd; ++it)
			{
				const Group& group = *it;
				bgfx::destroyVertexBuffer(group.m_vbh);

				if (bgfx::isValid(group.m_ibh) )
				{
					bgfx::destroyIndexBuffer(group.m_ibh);
				}
			}
			m_groups.clear();
		}

	void submit(const MeshState*const* _state, uint8_t _numPasses, const float* _mtx, uint16_t _numMatrices) const
		{
			uint32_t cached = bgfx::setTransform(_mtx, _numMatrices);

			for (uint32_t pass = 0; pass < _numPasses; ++pass)
			{
				bgfx::setTransform(cached, _numMatrices);

				const MeshState& state = *_state[pass];
				bgfx::setState(state.m_state);

				for (uint8_t tex = 0; tex < state.m_numTextures; ++tex)
				{
					const MeshState::Texture& texture = state.m_textures[tex];
					bgfx::setTexture(texture.m_stage
									 , texture.m_sampler
									 , texture.m_texture
									 , texture.m_flags
						);
				}

				for (GroupArray::const_iterator it = m_groups.begin(), itEnd = m_groups.end(); it != itEnd; ++it)
				{
					const Group& group = *it;

					bgfx::setIndexBuffer(group.m_ibh);
					bgfx::setVertexBuffer(group.m_vbh);
					bgfx::submit(state.m_viewId, state.m_program, 0, it != itEnd-1);
				}
			}
		}

	bgfx::VertexDecl m_decl;
	GroupArray m_groups;

};

}
