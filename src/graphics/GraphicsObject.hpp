#ifndef GRAPHICSOBJECT_H_HEADER_GUARD
#define GRAPHICSOBJECT_H_HEADER_GUARD

#include "Shader.hpp"
#include "TextureAtlas.hpp"
#include "GraphicsEngine.hpp"
#include <common/math.hpp>
#include <utils/VertexUtils.hpp>
#include <utils/MeshFactory.hpp>
#include <string>
#include <Eigen/Dense>
#include <common/debugdraw/debugdraw.h>
#include <graphics.pb.h>

namespace pumpkin {

bool loadShader(MeshState *pass, uint32_t id);
bool loadTexture(MeshState *pass, uint32_t id);

struct Prefab {
	std::vector<MeshState> m_passes;
	Mesh m_mesh;
	Prefab(const std::vector<MeshState> & passes, const Mesh & mesh) : m_passes(passes),
																	   m_mesh(mesh) {}
};


/*
Every graphic object renders in two passes. One for the visible region
and one (darker) for the non visible. In the darker pass, a uniform checks
for visibility due to sound etc...
*/

class GraphicsObject {

public:

	typedef voyage::GraphicsObjectCfg Config;

	friend class GraphicsObjectFactory;

	//GraphicsObject() : m_transform(Eigen::MatrixXf::Identity(4,4)) {}
	//GraphicsObject(const Config & config);

	~GraphicsObject() {
		for (auto prefab : m_prefabs)
			prefab.m_mesh.unload();
	}

	virtual void init() {
		// debugging
		ddInit();
	}

	void addPrefab(const std::vector<MeshState> & passes, const Mesh & mesh) {
		m_prefabs.push_back(Prefab(passes, mesh));
	}

	void addPrefab(const Prefab & prefab) {
		m_prefabs.push_back(prefab);
	}

	void meshSubmit() {
		for (auto prefab : m_prefabs) {
			prefab.m_mesh.submit(prefab.m_passes,
								 (const float*)m_transform.data(),
								 (uint16_t) 1);
		}
	}

	virtual void draw(float d) {
	};

	virtual void render(float d) {

	}

	void setTransform(const Eigen::MatrixXf & transform) {
		m_transform = transform;
	}

protected:

	std::vector<Prefab> m_prefabs;
	Eigen::MatrixXf m_transform;
};

}
#endif
