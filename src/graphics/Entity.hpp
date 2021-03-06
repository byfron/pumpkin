#ifndef ENTITY_H_HEADER_GUARD
#define ENTITY_H_HEADER_GUARD

#include <memory>

namespace pumpkin {
	
class Entity {

public:
	Entity() {};

	virtual void init() = 0;
	virtual void update(float delta) = 0;	

	typedef std::shared_ptr<Entity> Ptr;


	void setPosition(const Eigen::Vector3f & p) {
		m_position = p;
	}

protected:

	Eigen::Vector3f m_position;
};

}

#endif
