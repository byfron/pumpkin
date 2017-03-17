$input v_texcoord0

#include "../../bgfx/examples/common/common.sh"

SAMPLER2D(u_postTex,  0);

void main()
{

	vec2 resolution = vec2(1280, 720);
	//float amount = 400.0;
	float amount = 10000.0;	
	float d = 1.0 / amount;
	float ar = resolution.x / resolution.y;
	float u = floor( v_texcoord0.x / d ) * d;
	d = ar / amount;
	float v = floor( v_texcoord0.y / d ) * d;

	gl_FragColor = texture2D(u_postTex, vec2(u,v));
}