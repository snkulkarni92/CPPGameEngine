/*
	This is an example of a fragment shader
*/

// Platform-specific setup
#include "shaders.inc"

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
