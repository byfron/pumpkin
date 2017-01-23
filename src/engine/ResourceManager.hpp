#pragma once

#include <memory>
#include <unordered_map>
#include <assert.h>
#include <Eigen/Dense>

class Resource {
public:
	virtual void init() = 0;
	virtual void update(float delta) = 0;
	typedef std::shared_ptr<Resource> Ptr;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

class ResourceManager {
public:

	void pushResource(uint32_t index, Resource::Ptr r) {
		_resource_map[index] = r;
	}
		
	Resource::Ptr getResource(uint32_t index) {
		assert(_resource_map.count(index)); //TODO: make this more efficient!
		return _resource_map[index];
	}

private:

	std::unordered_map<uint32_t, Resource::Ptr> _resource_map;
};
