#include "DebugGraphicsObject.hpp"

namespace pumpkin {
	
DebugGraphicsObject::DebugGraphicsObject() : m_transform(Eigen::MatrixXf::Identity(4,4)),
					     m_rotation(Eigen::MatrixXf::Identity(4,4)){
       		
	m_aabb =
	{
		{  5.0f, 1.0f, 1.0f },
		{ 10.0f, 5.0f, 5.0f },
	};

	m_color = 0xff00ff00;
	
}

void DebugGraphicsObject::update(float delta) {

	Eigen::Affine3f translation(Eigen::Translation3f(m_position(0),
							 m_position(1),
							 m_position(2)));
	
	m_transform = (translation.matrix() * m_rotation);
	
	ddBegin(0);
	ddPush();

	ddSetTransform(m_transform.data());
	ddSetColor(m_color);
	ddSetWireframe(true);
	ddDraw(m_aabb);

	ddPop();
	ddEnd();
	
}

}
