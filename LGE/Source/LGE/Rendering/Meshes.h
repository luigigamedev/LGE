#pragma once

namespace LGE::Meshes
{
	// --- QUAD ---
	constexpr float QUAD_VERTICES[] = {
		// pos                // normal           // uv
		 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 0 top right
		-0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // 1 top left
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 2 bottom left
		 0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f  // 3 bottom right
	};

	constexpr unsigned int QUAD_INDICES[] = {
		0, 1, 2,
		0, 2, 3
	};

	constexpr unsigned int QUAD_INDEX_COUNT = sizeof(QUAD_INDICES) / sizeof(unsigned int);

	// --- CUBE ---
	constexpr float CUBE_VERTICES[] = {
		// Front face (+Z)       -- vertices 0-3
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 0 bottom left
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 1 bottom right
		 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 2 top right
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // 3 top left

		// Back face (-Z)        -- vertices 4-7
		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f, // 4 bottom left
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f, // 5 bottom right
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f, // 6 top right
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f, // 7 top left

		 // Left face (-X)        -- vertices 8-11
		 -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 8  bottom left
		 -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // 9  bottom right
		 -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 10 top right
		 -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 11 top left

		 // Right face (+X)       -- vertices 12-15
		  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 12 bottom left
		  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // 13 bottom right
		  0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 14 top right
		  0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 15 top left

		  // Top face (+Y)         -- vertices 16-19
		  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 16 bottom left
		   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 17 bottom right
		   0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 18 top right
		  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 19 top left

		  // Bottom face (-Y)      -- vertices 20-23
		  -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f, // 20 bottom left
		   0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f, // 21 bottom right
		   0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f, // 22 top right
		  -0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f  // 23 top left
	};

	constexpr unsigned int CUBE_INDICES[] = {
		 0,  1,  2,   0,  2,  3, // Front
		 4,  5,  6,   4,  6,  7, // Back
		 8,  9, 10,   8, 10, 11, // Left
		12, 13, 14,  12, 14, 15, // Right
		16, 17, 18,  16, 18, 19, // Top
		20, 21, 22,  20, 22, 23  // Bottom
	};

	constexpr unsigned int CUBE_INDEX_COUNT = sizeof(CUBE_INDICES) / sizeof(unsigned int);
}
