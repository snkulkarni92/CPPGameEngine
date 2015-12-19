return
{
	{
		root = "Scenes",
		builder = "SceneBuilder.exe",
		assets = 
		{
			{source = "chess.scene", target = "chess.scene"},
		}
	},
	{
		root = "Meshes",
		builder = "MeshBuilder.exe",
		assets = 
		{
			{source = "chessboard.msh", target = "chessboard.msh"},
			{source = "king.msh", target = "king.msh"},
			{source = "queen.msh", target = "queen.msh"},
			{source = "bishop.msh", target = "bishop.msh"},
			{source = "knight.msh", target = "knight.msh"},
			{source = "rook.msh", target = "rook.msh"},
			{source = "pawn.msh", target = "pawn.msh"},
			{source = "box.msh", target = "box.msh"},
			{source = "smallbox.msh", target = "smallbox.msh"},
		}
	},
	{
		root = "Materials",
		builder = "MaterialBuilder.exe",
		assets = 
		{
			{source = "OpaqueDefault.material", target = "OpaqueDefault.material"},
			{source = "ChessBoard.material", target = "ChessBoard.material"},
			{source = "White.material", target = "White.material"},
			{source = "Black.material", target = "Black.material"},
			{source = "Selection.material", target = "Selection.material"},
		}
	},
	{
		root = "Textures",
		builder = "TextureBuilder.exe",
		assets =
		{
			{source = "chessboard.jpg", target = "chessboard.texture"},
			{source = "default.png", target = "default.texture"},
			{source = "wall.jpg", target = "wall.texture"},
			{source = "black.jpg", target = "black.texture"},
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