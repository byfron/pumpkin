$input v_texcoord0

uniform vec4 tex0_offset;

/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "../bgfx/examples/common/common.sh"

SAMPLER2D(s_texColor,  0);

void main()
{
	gl_FragColor = texture2D(s_texColor, v_texcoord0.xy + tex0_offset.xy);
}
