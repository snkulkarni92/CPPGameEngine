/*
	This is an example of a fragment shader
*/

// Platform-specific setup
#include "shaders.inc"

//Uniform Declaration
uniform float3 g_MatColor;
uniform float3 g_LightDirection;
uniform float3 g_LightColor;
uniform float3 g_Ambient;
uniform float g_Shininess;
uniform float3 g_CameraPosition;

uniform sampler2D g_TexSampler;

// Entry Point
//============

#include "fragmentMain.inc" //void main()

{
	// Set the fragment to the interpolated color that originated as per-vertex data
	// (where color is represented by 4 floats representing "RGBA" == "Red/Green/Blue/Alpha")
	{
		o_color = i_color;
		
	}
	
}
