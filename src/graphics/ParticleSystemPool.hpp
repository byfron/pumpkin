#pragma once

#include <Pool.hpp>

class ParticleGraphicsSystem {
public:
	ParticleGraphicsSystem() {

	}
	
protected:

	int m_id;
};

class ParticleSystemPool : Pool<ParticleGraphicsSystem> {

	ParticleSystemPool(int size) {

	}
};


