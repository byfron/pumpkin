#include "GraphicsObject.hpp"
#include "GraphicsObjectFactory.hpp"
#include "GraphicsEngine.hpp"
#include <utils/MeshFactory.hpp>
#include <iostream>
#include <common/debugdraw/debugdraw.h>

namespace pumpkin {

	GraphicsObject::GraphicsObject(const GraphicsObject::Config & config) :
		m_transform(Eigen::MatrixXf::Identity(4,4)) {

		// push a default mesh state in slot 0
		// This is quite ugly though
		m_mesh_state_vector.push_back(MeshState());
		
		GraphicsObjectFactory gof(config);
		gof.generate(this);
	}
}
