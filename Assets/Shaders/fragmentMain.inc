/*
	This file should be #included in every shader
	to account for platform independent main function
*/

#if defined( EAE6320_PLATFORM_D3D )

void main(

	// Input
	//======

	// Whatever arbitrary data (i.e. everything excluding position) was output from the vertex shader
	// will be interpolated across the triangle and given as input to the fragment shader

	in float4 i_color : COLOR0,
	in float2 i_texcoord : TEXCOORD0,
	in float3 i_normal : NORMAL,
	in float3 i_position : TEXCOORD1,

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : COLOR0

	)

#elif defined( EAE6320_PLATFORM_GL )


// Input
//======

// Whatever arbitrary data (i.e. everything excluding position) was output from the vertex shader
// will be interpolated across the triangle and given as input to the fragment shader

layout( location = 0 ) in vec4 i_color;
layout( location = 1 ) in vec2 i_texcoord;
layout( location = 2 ) in vec3 i_normal;
layout( location = 3 ) in vec3 i_position;

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

void main()

#endif