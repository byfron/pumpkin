#ifndef ENTITY_H_HEADER_GUARD
#define ENTITY_H_HEADER_GUARD

#include <memory>

class Entity {

public:
	Entity() {};

	virtual void init() = 0;
	virtual void update(float delta) = 0;	

	typedef std::shared_ptr<Entity> Ptr;
};



#endif
