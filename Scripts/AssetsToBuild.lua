return
{
	{
		root = "Meshes",
		builder = "MeshBuilder.exe",
		assets = 
		{
			{source = "pyramid.msh", target = "pyramid.msh"},
			{source = "sphere.msh", target = "sphere.msh"},
			{source = "plane.msh", target = "plane.msh"},
			{source = "plane1.msh", target = "plane1.msh"},
		}
	},
	{
		root = "Effects",
		builder = "EffectBuilder.exe",
		assets = 
		{
			{source = "opaque.effect", target = "opaque.bineffect"},
			{source = "transparent.effect", target = "transparent.bineffect"},
		}
	},
	{
		root = "Materials",
		builder = "MaterialBuilder.exe",
		assets = 
		{
			{source = "OpaqueDefault.material", target = "OpaqueDefault.material"},
			{source = "OpaqueRed.material", target = "OpaqueRed.material"},
			{source = "OpaqueBlue.material", target = "OpaqueBlue.material"},
			{source = "TransparentGreen.material", target = "TransparentGreen.material"},
			{source = "TransparentYellow.material", target = "TransparentYellow.material"},
			{source = "Test1.material", target = "Test1.material"},
			{source = "Test2.material", target = "Test2.material"},
		}
	},
	{
		root = "Textures",
		builder = "TextureBuilder.exe",
		assets =
		{
			{source = "wall.jpg", target = "wall.texture"},
			{source = "eae6320.png", target = "eae6320.texture"},
			{source = "alpha.png", target = "alpha.texture"},
			{source = "default.png", target = "default.texture"},
		}
	},
	{
		root = "Shaders",
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