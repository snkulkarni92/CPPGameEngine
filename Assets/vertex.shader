/*
	This is an example of a vertex shader
*/

// Platform-specific setup
#include "shaders.inc"

// Platform-independant Uniform declaration
uniform float2 g_position_offset;

// Entry Point
//============

#include "vertexMain.inc" // void main()

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