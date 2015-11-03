return
{
	{
		builder = "MeshBuilder.exe",
		assets = 
		{
			{source = "rectangle.msh", target = "rectangle.msh"},
			{source = "triangle.msh", target = "triangle.msh"},
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