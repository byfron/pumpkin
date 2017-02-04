#ifndef DEBUGGRAPHICSOBJECT_H_HEADER_GUARD
#define DEBUGGRAPHICSOBJECT_H_HEADER_GUARD

#include <string>
#include <Eigen/Dense>
#include <common/bgfx_utils.h>
#include <common/debugdraw/debugdraw.h>
#include <common/math.hpp>

namespace pumpkin {

class DebugGraphicsObject {

public:

	DebugGraphicsObject();

	void init();
	
	void update(float d);

	void destroyUniforms();
	void initialiseBuffers();
	void createUniforms();

	void setAABB(Vec3f mmin, Vec3f mmax) {
		m_aabb.m_min[0] = mmin(0);
		m_aabb.m_min[1] = mmin(1);
		m_aabb.m_min[2] = mmin(2);
		m_aabb.m_max[0] = mmax(0);
		m_aabb.m_max[1] = mmax(1);
		m_aabb.m_max[2] = mmax(2);
	}
	
	void setPosition(const Eigen::Vector3f & p) {
		m_position = p;
	}

	void setRotation(const Eigen::MatrixXf & rot) {
		m_rotation = rot;
	}
protected:

	// TODO: Put this into the body, and make the debug system run through
	// body,Debug
	Aabb m_aabb;
	Eigen::Vector3f m_position;
	Eigen::MatrixXf m_rotation;
	Eigen::MatrixXf m_transform;
	uint32_t m_color;

};

}
#endif
