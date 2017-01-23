$input a_position, a_normal, a_texcoord0
$output v_wpos, v_wnormal, v_texcoord0


#include "../bgfx/examples/common/common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1) );
	vec3 world_pos = mul(u_model[0], vec4(a_position, 1) ).xyz;
	v_wpos = world_pos;

	// Transform normal into world space
	vec3 wnormal = mul(u_model[0], vec4(a_normal.xyz, 0) ).xyz;	

	v_texcoord0 = a_texcoord0;
	v_wnormal = wnormal;
	
}
