#include "GraphicsObject.hpp"
#include "GraphicsObjectFactory.hpp"
#include "GraphicsEngine.hpp"
#include <utils/MeshFactory.hpp>
#include <iostream>
#include <common/debugdraw/debugdraw.h>

namespace pumpkin {

	GraphicsObject::GraphicsObject(const GraphicsObject::Config & config) :
		m_transform(Eigen::MatrixXf(4,4)) {
	
		m_mesh_vector.push_back(VertexUtils::constructVPlane(0.5, 0.5));
		
		MeshState state(0,
				0,
				BGFX_STATE_PT_TRISTRIP |
				BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA ,
						      BGFX_STATE_BLEND_INV_SRC_ALPHA),
				0);

		m_mesh_state_vector.push_back(state);	       
		GraphicsObjectFactory gof(config);
		gof.generate(this);
	}
}
