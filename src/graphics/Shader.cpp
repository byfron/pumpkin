#include "Shader.hpp"
#include "ShaderFactory.hpp"

namespace pumpkin {

Shader::Shader(const std::string & shader_cfg) {

	ShaderFactory factory(shader_cfg);
	factory.generate(this);
}
	
};
