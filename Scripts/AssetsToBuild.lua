return
{
	{
		builder = "MeshBuilder.exe",
		assets = 
		{
			{source = "pyramid.msh", target = "pyramid.msh"},
			{source = "plane.msh", target = "plane.msh"},
		}
	},
	{
		builder = "EffectBuilder.exe",
		assets = 
		{
			{source = "shaders.effect", target = "shaders.bineffect"},
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
				source = "fragment.shader", 
				target = "fragment.shader", 
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