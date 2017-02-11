$input a_position, a_normal, a_tangent, a_texcoord0
$output v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0

uniform vec4 packed_input;
uniform int flip;

#define tex0_offset packed_input.xy
#define sprite_width packed_input.z


/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

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

	vec2 flipped_texcoord0;
	
	if (flip == 1) {
	     flipped_texcoord0.y = a_texcoord0.y + tex0_offset.y;
	     flipped_texcoord0.x = (sprite_width - a_texcoord0.x) + tex0_offset.x;	
	}
	else {
	     flipped_texcoord0 = a_texcoord0.xy + tex0_offset.xy;
	}
	
	v_texcoord0 = flipped_texcoord0;
	v_normal = viewNormal;
	v_tangent = viewTangent;	    
	v_bitangent = viewBitangent;
}
