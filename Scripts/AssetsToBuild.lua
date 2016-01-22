return
{
	{
		root = "Meshes",
		builder = "MeshBuilder.exe",
		assets = 
		{
			{source = "CTF0.msh", target = "CTF0.msh"},
			{source = "CTF1.msh", target = "CTF1.msh"},
			{source = "CTF2.msh", target = "CTF2.msh"},
			{source = "CTF3.msh", target = "CTF3.msh"},
			{source = "CTF4.msh", target = "CTF4.msh"},
			{source = "CTF5.msh", target = "CTF5.msh"},
			{source = "CTF6.msh", target = "CTF6.msh"},
			{source = "CTF7.msh", target = "CTF7.msh"},
		}
	},
	{
		root = "Materials",
		builder = "MaterialBuilder.exe",
		assets = 
		{
			{source = "OpaqueDefault.material", target = "OpaqueDefault.material"},
			{source = "Floor.material", target = "Floor.material"},
			{source = "Railing.material", target = "Railing.material"},
			{source = "Cement.material", target = "Cement.material"},
			{source = "Metal.material", target = "Metal.material"},
			{source = "Walls.material", target = "Walls.material"},
		}
	},
	{
		root = "Textures",
		builder = "TextureBuilder.exe",
		assets =
		{
			{source = "cement_wall_D.png", target = "cement.texture"},
			{source = "default.png", target = "default.texture"},
			{source = "floor_D.png", target = "floor.texture"},
			{source = "metal_brace_D.png", target = "metal.texture"},
			{source = "railing_D.png", target = "railing.texture"},
			{source = "wall_D.png", target = "walls.texture"},
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