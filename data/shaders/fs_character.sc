$input v_wpos, v_normal, v_texcoord0

uniform vec4 tex0_offset;

/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../../bgfx/examples/common/common.sh"

SAMPLER2D(s_texColor,  0);

uniform vec4 u_lightPosRadius;


void main()
{
	vec3 pos_view = mul(u_view, vec4(v_wpos, 1)).xyz;
	vec3 eye_dir_view = vec3(0,0,0) - pos_view;
	vec3 v_normal_view = mul(u_view, vec4(v_normal.xyz, 0) ).xyz;
	vec3 lightpos_view = mul(u_view, vec4(u_lightPosRadius.xyz, 1)).xyz;
	vec3 light_dir_view = lightpos_view - pos_view;// + eye_dir_view;
	
	vec3 n = normalize(v_normal_view);
	vec3 l = normalize(light_dir_view);

	float cosTheta = clamp( dot( n,l ), 0,1 );
	float dist = length(lightpos_view);

	vec4 color = texture2D(s_texColor, v_texcoord0);
//	gl_FragColor =  vec4(color.xyz * cosTheta, color.w);
	gl_FragData[0] = vec4(color.xyz * cosTheta, color.w);
}
