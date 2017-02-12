$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0

#include "../../bgfx/examples/common/common.sh"

SAMPLER2D(s_texColor,  0);
SAMPLER2D(s_texNormal, 1);

uniform vec4 u_lightPosRadius;

vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir)
{
	float ndotl = dot(_normal, _lightDir);
	vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
	float rdotv = dot(reflected, _viewDir);
	return vec2(ndotl, rdotv);
}

vec4 lit(float _ndotl, float _rdotv, float _m)
{
	float diff = max(0.0, _ndotl);
	float spec = step(0.0, _ndotl) * max(0.0, _rdotv * _m);
	return vec4(1.0, diff, spec, 1.0);
}

vec3 calcLight(mat3 _tbn, vec3 _wpos, vec3 _normal, vec3 _view)
{
	vec3 lp = u_lightPosRadius.xyz - _wpos;
	float attn = 1.0 - smoothstep(1.0, 1.0, length(lp) / u_lightPosRadius.w);
	vec3 lightDir = mul(_tbn, normalize(lp) );
	vec2 bln = blinn(lightDir, _normal, _view);
	vec4 lc = lit(bln.x, bln.y, 1.0);
	vec3 rgb = vec3(1.0,1.0,1.0) * saturate(lc.y) * attn;
	return rgb;
}

void main()
{


	mat3 tbn = mat3(
			normalize(v_tangent),
			normalize(v_bitangent),
			normalize(v_normal)
			);

// If we have no normal map, use the geometry normal!

	vec3 normal = v_normal;
	normal.xy = texture2D(s_texNormal, v_texcoord0).xy * 2.0 - 1.0;
	normal.z = sqrt(1.0 - dot(normal.xy, normal.xy) );
	
	vec3 view = -normalize(v_view);

	vec3 lightColor;
	lightColor =  calcLight(tbn, v_wpos, normal, view);
//	lightColor += calcLight(1, tbn, v_wpos, normal, view);
//	lightColor += calcLight(2, tbn, v_wpos, normal, view);
//	lightColor += calcLight(3, tbn, v_wpos, normal, view);

	vec4 color = toLinear(texture2D(s_texColor, v_texcoord0) );

//	vec3 out = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;
//	out.xyz = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;
//	out.w = 1.0;
	
	gl_FragData[0].xyz = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;
	gl_FragData[0].w = 1.0;
	//gl_FragColor = toGamma(gl_FragColor);
//	gl_FragData[0] = out;


	// vec3 view_pos = mul(u_view, vec4(v_wpos, 1)).xyz;
	// vec3 eye_dir_view = vec3(0,0,0) - view_pos;
	// vec3 v_normal_view = mul(u_view, vec4(v_wnormal.xyz, 0) ).xyz;
	// vec3 lightpos_view = mul(u_view, vec4(u_lightPosRadius.xyz, 1)).xyz;
	// vec3 light_dir_view = lightpos_view - view_pos;// + eye_dir_view;
	
	// vec3 n = normalize(v_normal_view);
	// vec3 l = normalize(light_dir_view);

	// float cosTheta = clamp( dot( n,l ), 0,1 );
	// float dist = length(lightpos_view);
	
	// gl_FragColor = texture2D(s_texColor, v_texcoord0) * cosTheta;
}

