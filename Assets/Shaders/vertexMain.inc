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

	// Output
	//=======

	out float4 o_position : POSITION,
	out float4 o_color : COLOR,
	out float2 o_texcoord : TEXCOORD0

	)

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======


layout( location = 0 ) in vec3 i_position;
layout( location = 1 ) in vec4 i_color;
layout( location = 2 ) in vec2 i_texcoord;

// Output
//=======

layout( location = 0 ) out vec4 o_color;
layout( location = 1 ) out vec2 o_texcoord;

// Entry Point
//============

void main()

#endif