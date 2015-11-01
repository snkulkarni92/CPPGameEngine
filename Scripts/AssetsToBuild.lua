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
		builder = "GenericBuilder.exe",
		assets = 
		{
			{source = "vertex.shader", target = "vertex.shader"},
			{source = "fragment.shader", target = "fragment.shader"},
		}
	},
}