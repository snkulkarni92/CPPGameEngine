/*
	This file should be #included in every shader
	to set up platform-specific preprocessor directives
	so that the shader itself can be mostly platform-independent
*/

#if defined( EAE6320_PLATFORM_D3D )

// Translate GLSL variable types to HLSL
#define vec2 float2
#define vec3 float3
#define vec4 float4
#define mat2 float2x2
#define mat3 float3x3
#define mat4 float4x4

#define out_position o_position

#define Transform(i_vector, i_matrix) mul(i_vector, i_matrix)

#define Texture2D tex2D

#elif defined( EAE6320_PLATFORM_GL )

// The version of GLSL to use must come first
#version 330

// This extension is required in order to specify 
// explicit locations for shader inputs and outputs
#extension GL_ARB_separate_shader_objects : require

// Translate HLSL variable types to GLSL
#define float2 vec2
#define float3 vec3
#define float4 vec4
#define float2x2 mat2
#define float3x3 mat3
#define float4x4 mat4

#define out_position gl_Position

#define Transform(i_vector, i_matrix) i_vector * i_matrix

#define Texture2D texture2D

#endif

void CalculateDiffuseLighting( const in float3 i_normal_world,
    const in float3 i_light_direction, const in float3 i_light_color,
    out float3 o_lighting_diffuse )
{
    // Calculate how parallel the light and the normal are
    float lambertTerm;
    {
        float dotProduct;
        {
            // Reverse the light direction so that it represents the unit vector from the fragment to the light
            float3 lightVector = -i_light_direction;
            dotProduct = dot( i_normal_world, lightVector );
        }
        // The dot product will be negative if the normal is facing away from the light
        // (i.e. in shadow)
        // and so it should be clamped to 0
        // (i.e. the light won't affect it)
        lambertTerm = max( 0.0, dotProduct );
    }
    // The diffuse lighting is a combination of the lambert term and the light's color
    o_lighting_diffuse = lambertTerm * i_light_color;
}
void CalculateSpecularLighting( const in float3 i_position_world, const in float3 i_normal_world, const in float3 i_camera_position,
    const in float3 i_light_direction, const in float3 i_light_color,
    const in float i_shininess,
    out float3 o_lighting_specular )
{
    float specularAmount;
    {
        // Calculate how parallel the reflected light and the camera are
        float dotProduct;
        {
            // Direction of the reflected light
            float3 reflectedDirection = reflect( i_light_direction, i_normal_world );
            // Direction of the view ray
            float3 viewDirection = normalize( i_camera_position - i_position_world );
            // How close (parallel) the view ray is to the reflected light
            dotProduct = dot( reflectedDirection, viewDirection );
            // See the diffuse function for why this is clamped (the reasoning is similar)
            dotProduct = max( 0.0, dotProduct );
        }
        // A pow() function is used as a hack to approximate how big or small the specular highlight is
        // (which makes an object seem more "shiny", or smooth as opposed to rough)
        specularAmount = pow( dotProduct, i_shininess );
    }
    // The specular lighting is a combination of the specular amount and the light's color
    o_lighting_specular = i_light_color * specularAmount;
}