/*
	This is an example of a vertex shader
*/

// Platform-specific setup
#include "shaders.inc"

// Platform-independant Uniform declaration
uniform float2 g_position_offset;

#if defined( EAE6320_PLATFORM_D3D )

// Entry Point
//============

void main(

	// Input
	//======

	in const float2 i_position : POSITION,
	in const float4 i_color : COLOR,

	// Output
	//=======

	out float4 o_position : POSITION,
	out float4 o_color : COLOR

	)

#elif defined( EAE6320_PLATFORM_GL )


// Input
//======


layout( location = 0 ) in vec2 i_position;
layout( location = 1 ) in vec4 i_color;

// Output
//=======

layout( location = 0 ) out vec4 o_color;

// Entry Point
//============

void main()

#endif

{
	// Calculate position
	{
		out_position = vec4( i_position + g_position_offset, 0.0, 1.0 );
	}
	// Pass the input color to the fragment shader unchanged:
	{
		o_color = i_color;
	}
}