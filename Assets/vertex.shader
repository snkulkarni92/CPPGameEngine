/*
	This is an example of a vertex shader
*/

// Platform-specific setup
#include "shaders.inc"

// Platform-independant Uniform declaration
//uniform float3 g_position_offset;
uniform float4x4 g_Local_To_World;
uniform float4x4 g_World_To_View;
uniform float4x4 g_View_To_Screen;

// Entry Point
//============

#include "vertexMain.inc" // void main()

{
	/*// Calculate position
	{
		out_position = vec4( i_position + g_position_offset, 1.0 );
	}*/
	// Calculate the position of this vertex on screen
	{
		// The position stored in the vertex is in "local space",
		// meaning that it is relative to the center (or "origin", or "pivot")
		// of the mesh.
		// The graphics hardware needs the position of the vertex
		// in normalized device coordinates,
		// meaning where the position of the vertex should be drawn
		// on the screen.
		// This position that we need to output, then,
		// is the result of taking the original vertex in local space
		// and transforming it into "screen space".

		// Any matrix transformations that include translation
		// will operate on a float4 position,
		// which _must_ have 1 for the w value
		
		float4 position_world = Transform( float4( i_position, 1.0 ), g_Local_To_World );
		float4 position_view = Transform( position_world, g_World_To_View );
		out_position = Transform( position_view, g_View_To_Screen );
	}
	// Pass the input color to the fragment shader unchanged:
	{
		o_color = i_color;
	}
}