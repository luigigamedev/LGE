#pragma once

namespace LGE::Meshes {

	// --- QUAD ---
	constexpr float QUAD_VERTICES[] = {
		// pos                  // normal           // uv
		-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 0 Bottom Left
		 0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 1 Bottom Right
		 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 2 Top Right
		-0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f  // 3 Top Left
	};

	constexpr unsigned int QUAD_INDICES[] = {
		0, 1, 2,
		0, 2, 3
	};

	constexpr unsigned int QUAD_INDEX_COUNT = sizeof(QUAD_INDICES) / sizeof(unsigned int);

	// --- CUBE ---
	constexpr float CUBE_VERTICES[] = {
		// Front face (+Z)
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 0 BL
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 1 BR
		 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 2 TR
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // 3 TL

		// Back face (-Z)
		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f, // 4 BL
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f, // 5 BR
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f, // 6 TR
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f, // 7 TL

		 // Left face (-X)
		 -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 8 BL
		 -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // 9 BR
		 -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 10 TR
		 -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 11 TL

		 // Right face (+X)
		  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 12 BL
		  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // 13 BR
		  0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 14 TR
		  0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 15 TL

		  // Top face (+Y)
		  -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 16 BL
		   0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 17 BR
		   0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 18 TR
		  -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 19 TL

		  // Bottom face (-Y)
		  -0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f, // 20 BL
		   0.5f, -0.5f, -0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f, // 21 BR
		   0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f, // 22 TR
		  -0.5f, -0.5f,  0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f  // 23 TL
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
