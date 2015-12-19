/*
	This is an example of a fragment shader
*/

// Platform-specific setup
#include "shaders.inc"

//Uniform Declaration
uniform float3 g_MatColor;
uniform float g_Alpha;
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
		o_color *= Texture2D( g_TexSampler, i_texcoord );
		o_color.rgb *= g_MatColor;
		o_color.a *= g_Alpha;
	}
	// Calculate the lighting
	{
		// Normalize the light direction
		// (This should be done once in C code; if you do that you can leave this expensive per-fragment operation out of the shader)
		float3 light_direction = normalize( g_LightDirection );
		float3 lighting_diffuse;
		{
			CalculateDiffuseLighting( i_normal,
				light_direction, g_LightColor,
				lighting_diffuse );
		}
		float3 lighting_specular;
		{
			CalculateSpecularLighting( i_position, i_normal, g_CameraPosition,
				light_direction, g_LightColor,
				g_Shininess,
				lighting_specular );
		}
		// Modify the diffuse color by the view-independent lighting
		o_color.rgb *= ( lighting_diffuse + g_Ambient );
		// Add the view-dependent lighting
		o_color.rgb += lighting_specular;
	}
}
