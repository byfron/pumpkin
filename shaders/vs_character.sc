$input a_position, a_normal, a_texcoord0
$output v_wpos, v_wnormal, v_texcoord0


uniform vec4 packed_input;
uniform int flip;

#define tex0_offset packed_input.xy
#define sprite_width packed_input.z


/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../bgfx/examples/common/common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );


	//we should only apply translation to go to world, so that
	//the model does not really rotate
	mat4 model = u_model[0];

	vec4 translation = u_model[0][3];
	
//	vec3 world_pos = mul(model[0], vec4(a_position, 1) ).xyz;
	vec3 world_pos = translation.xyz + a_position;

	// Do not rotate the normal of the character!
	vec3 wnormal = a_normal.xyz; //mul(u_model[0], vec4(a_normal.xyz, 0) ).xyz;	

	vec2 flipped_texcoord0;
	
	if (flip == 1) {
	     flipped_texcoord0.y = a_texcoord0.y + tex0_offset.y;
	     flipped_texcoord0.x = (sprite_width - a_texcoord0.x) + tex0_offset.x;	
	}
	else {
	     flipped_texcoord0 = a_texcoord0.xy + tex0_offset.xy;
	}
	
	v_texcoord0 = flipped_texcoord0;
        v_wpos = world_pos;
	v_wnormal = wnormal;	
}
