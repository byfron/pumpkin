#pragma once

#include <memory>
#include <unordered_map>
#include <assert.h>
#include <Eigen/Dense>

namespace pumpkin {
class Resource {
public:
	virtual void init() = 0;
	typedef std::shared_ptr<Resource> Ptr;

	uint32_t getId() { return m_id; }

protected:
	uint32_t m_id;
	
};

class ResourceManager {

public:

	template <typename T>
	static typename T::Ptr getResource(uint32_t id) {
		//TODO: make sure that the type corresponds!
		if (resourceExists(id)) {
			return std::static_pointer_cast<T>(getResourcePtr(id));
		}
		return nullptr;
	}

	static bool resourceExists(uint32_t id) {
		return _resource_map.count(id) > 0;
	}

	template <typename T>
	static void pushResource(std::string cfg) {
		Resource::Ptr r = std::make_shared<T>(cfg);
		uint32_t res_id = r->getId();
		if (_resource_map.count(res_id) == 0) {				
			_resource_map[res_id] = r;
		}
		else {
			//log std::cout << "error id existing" <<std::endl;
			assert(false);
		}
	}
	
	static Resource::Ptr getResourcePtr(uint32_t index) {
		assert(_resource_map.count(index)); //TODO: make this more efficient!
		return _resource_map[index];
	}
	
private:
	static std::unordered_map<uint32_t, Resource::Ptr> _resource_map;
};

}
