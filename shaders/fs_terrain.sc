$input v_wpos, v_wnormal, v_texcoord0

#include "../bgfx/examples/common/common.sh"

SAMPLER2D(s_texColor,  0);

uniform vec4 u_lightPosRadius;

void main()
{

	vec3 view_pos = mul(u_view, vec4(v_wpos, 1)).xyz;
	vec3 eye_dir_view = vec3(0,0,0) - view_pos;
	vec3 v_normal_view = mul(u_view, vec4(v_wnormal.xyz, 0) ).xyz;
	vec3 lightpos_view = mul(u_view, vec4(u_lightPosRadius.xyz, 1)).xyz;
	vec3 light_dir_view = lightpos_view - view_pos;// + eye_dir_view;
	
	vec3 n = normalize(v_normal_view);
	vec3 l = normalize(light_dir_view);

	float cosTheta = clamp( dot( n,l ), 0,1 );
	float dist = length(lightpos_view);
	
	gl_FragColor = texture2D(s_texColor, v_texcoord0) * cosTheta;
}
