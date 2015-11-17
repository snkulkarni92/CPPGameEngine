return
{
	{
		builder = "MeshBuilder.exe",
		assets = 
		{
			{source = "pyramid.msh", target = "pyramid.msh"},
			{source = "cylinder.msh", target = "cylinder.msh"},
			{source = "plane.msh", target = "plane.msh"},
		}
	},
	{
		builder = "EffectBuilder.exe",
		assets = 
		{
			{source = "opaque.effect", target = "opaque.bineffect"},
			{source = "transparent.effect", target = "transparent.bineffect"},
		}
	},
	{
		builder = "ShaderBuilder.exe",
		assets = 
		{
			{
				source = "vertex.shader", 
				target = "vertex.shader", 
				arguments = "vertex",
				dependencies = 
				{
					"shaders.inc",
					"vertexMain.inc",
				},
			},
			{
				source = "opaque.shader", 
				target = "opaque.shader", 
				arguments = "fragment",
				dependencies = 
				{
					"shaders.inc",
					"fragmentMain.inc"
				},
			},
			{
				source = "transparent.shader", 
				target = "transparent.shader", 
				arguments = "fragment",
				dependencies = 
				{
					"shaders.inc",
					"fragmentMain.inc"
				},
			},
		},
	},
}