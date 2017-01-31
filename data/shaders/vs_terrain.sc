$input a_position, a_normal, a_tangent, a_texcoord0
$output v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0


#include "../../bgfx/examples/common/common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1) );
	vec3 world_pos = mul(u_model[0], vec4(a_position, 1) ).xyz;
	v_wpos = world_pos;

	// Transform normal into world space
	vec4 normal = a_normal * 2.0 - 1.0;	
	vec3 wnormal = mul(u_model[0], vec4(normal.xyz, 0) ).xyz;	

	// Transform tangent into world space
	vec4 tangent = a_tangent * 2.0 - 1.0;
	vec3 wtangent = mul(u_model[0], vec4(tangent.xyz, 0.0) ).xyz;
	
	vec3 viewNormal = normalize(mul(u_view, vec4(wnormal, 0.0) ).xyz);
	vec3 viewTangent = normalize(mul(u_view, vec4(wtangent, 0.0) ).xyz);
	vec3 viewBitangent = cross(viewNormal, viewTangent) * tangent.w;
	mat3 tbn = mat3(viewTangent, viewBitangent, viewNormal);

	vec3 view = mul(u_view, vec4(v_wpos, 0.0) ).xyz;
	v_view = mul(view, tbn);
	
	v_texcoord0 = a_texcoord0;
	v_normal = viewNormal;
	v_tangent = viewTangent;	    
	v_bitangent = viewBitangent;
}
