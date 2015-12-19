/*
	This file should be #included in every shader
	to account for platform independent main function
*/

#if defined( EAE6320_PLATFORM_D3D )

void main(

	// Input
	//======

	in const float3 i_position : POSITION,
	in const float4 i_color : COLOR,
	in const float2 i_texcoord : TEXCOORD0,
	in const float3 i_normal : NORMAL,

	// Output
	//=======

	out float4 o_position : POSITION,
	out float4 o_color : COLOR,
	out float2 o_texcoord : TEXCOORD0,
	out float3 o_normal : NORMAL,
	out float3 o_world_position : TEXCOORD1
	)

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======


layout( location = 0 ) in vec3 i_position;
layout( location = 1 ) in vec4 i_color;
layout( location = 2 ) in vec2 i_texcoord;
layout( location = 3 ) in vec3 i_normal;

// Output
//=======

layout( location = 0 ) out vec4 o_color;
layout( location = 1 ) out vec2 o_texcoord;
layout( location = 2 ) out vec3 o_normal;
layout( location = 3 ) out vec3 o_world_position;

// Entry Point
//============

void main()

#endif