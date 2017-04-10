$input v_texcoord0

#include "../../bgfx/examples/common/common.sh"

SAMPLER2D(u_postTex,  0);
SAMPLER2D(u_visTex,  1);

void main()
{

	vec2 resolution = vec2(1280, 720);
	//float amount = 400.0;
	float amount = 600.0;
	float d = 1.0 / amount;
	float ar = resolution.x / resolution.y;
	float u = floor( v_texcoord0.x / d ) * d;
	d = ar / amount;
	float v = floor( v_texcoord0.y / d ) * d;

    // make this more efficient/sophisticated masking
	if (texture2D(u_visTex, vec2(u,v)).x == 0.0)
 	  gl_FragColor = texture2D(u_postTex, vec2(u,v));
	else
	  gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}