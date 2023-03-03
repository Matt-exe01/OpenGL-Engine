#pragma once

#include <vector>
#include <iterator>
#include <iostream>
#include <stdlib.h>


class Chunk
{
public:

	int xCoord;
	int zCoord;

    float* ptrMeshBuffer;

	std::vector<float> mesh;

	int ChunkData[16][64][16];

	Chunk(int x, int z, float* ptr) {
		xCoord = x;
		zCoord = z;

        ptrMeshBuffer = ptr;

		mesh.reserve(3000000);

		for (unsigned int x = 0; x < 16; x++)
		{
			for (unsigned int z = 0; z < 16; z++)
			{
				int colHeight = rand() % 24 + 40;
				for (unsigned int y = 0; y < 64; y++)
				{
					ChunkData[x][y][z] = (y < colHeight) ? 1 : 0;
				}
			}
		}
	};

	int generateChunkMesh() {
		int elemAdded = 0;

		for (int x = 0; x < 16; x++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int y = 0; y < 64; y++)
				{
					if (ChunkData[x][y][z] != 0) {
						for (int i = 1; i <= 6; i++)
						{
							if (!isFaceAdjacent(i, x, y, z)) {
								addFaceToMesh(i, x, y, z);
								elemAdded++;
							}
						}
					}
				}
			}
		}
		std::cout << (elemAdded * 30);
		std::copy(mesh.begin(), mesh.end(), ptrMeshBuffer);
		return elemAdded;
	}

private:

	void addFaceToMesh(int faceDirection, float chunkX, float chunkY, float chunkZ) {

		//per ognuno aggiungo la faccia più le coordinate del blocco nel mondo
		switch (faceDirection)
		{
			//  =====  BACK  =====
		case 1:
		{
			float BackVertices[30] = {
				(-0.5f + (chunkX)),  (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
				(-0.5f + (chunkX)),  ( 0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 1.0f,
				( 0.5f + (chunkX)),  ( 0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 1.0f,
				( 0.5f + (chunkX)),  ( 0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 1.0f,
				( 0.5f + (chunkX)),  (-0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 0.0f,
				(-0.5f + (chunkX)),  (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
			};

			mesh.insert(mesh.end(), std::begin(BackVertices), std::end(BackVertices));
		}
			break;

		//  =====  TOP  =====
		case 2:
		{
			float TopVertices[30] = {
				( 0.5f + (chunkX)),  (0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 0.0f,
				( 0.5f + (chunkX)),  (0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 1.0f,
				(-0.5f + (chunkX)),  (0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 1.0f,
				(-0.5f + (chunkX)),  (0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 1.0f,
				(-0.5f + (chunkX)),  (0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 0.0f,
				( 0.5f + (chunkX)),  (0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 0.0f,
			};

			mesh.insert(mesh.end(), std::begin(TopVertices), std::end(TopVertices));
		}
			break;

		//  =====  FRONT  =====
		case 3:
		{
			float FrontVertices[30] = {
				( 0.5f + (chunkX)), ( 0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 1.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 0.0f,
				( 0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 0.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 0.0f,
				( 0.5f + (chunkX)), ( 0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 1.0f,
				(-0.5f + (chunkX)), ( 0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 1.0f,
			};

			mesh.insert(mesh.end(), std::begin(FrontVertices), std::end(FrontVertices));
		}
			break;

		//  =====  BOTTOM  =====
		case 4:
		{
			float BottomVertices[30] = {
				(-0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
				( 0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 0.0f,
				( 0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 1.0f,
				( 0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 1.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 1.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
			};

			mesh.insert(mesh.end(), std::begin(BottomVertices), std::end(BottomVertices));
		}
			break;

		//  =====  LEFT  =====
		case 5:
		{
			float LeftVertices[30] = {
				(-0.5f + (chunkX)), ( 0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 1.0f,
				(-0.5f + (chunkX)), ( 0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 1.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 0.0f,
				(-0.5f + (chunkX)), ( 0.5f + chunkY), ( 0.5f + (chunkZ)), 1.0f, 1.0f,
			};

			mesh.insert(mesh.end(), std::begin(LeftVertices), std::end(LeftVertices));
		}
			break;

		//  =====  RIGHT  =====
		case 6:
		{
			float RightVertices[30] = {
				( 0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 0.0f,
				( 0.5f + (chunkX)), ( 0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 1.0f,
				( 0.5f + (chunkX)), ( 0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 1.0f,
				( 0.5f + (chunkX)), ( 0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 1.0f,
				( 0.5f + (chunkX)), (-0.5f + chunkY), ( 0.5f + (chunkZ)), 0.0f, 0.0f,
				( 0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 0.0f,
			};

			mesh.insert(mesh.end(), std::begin(RightVertices), std::end(RightVertices));
		}
			break;
		}

	}

	bool isFaceAdjacent(int face, int x, int y, int z) {
		switch (face)
		{
		case 1:
			if (z == 0) return false;
			if (ChunkData[x][y][z - 1] == 0) return false;
			break;
		case 2:
			if (y == 63) return false;
			if (ChunkData[x][y + 1][z] == 0) return false;
			break;
		case 3:
			if (z == 15) return false;
			if (ChunkData[x][y][z + 1] == 0) return false;
			break;
		case 4:
			if (y == 0) return false;
			if (ChunkData[x][y - 1][z] == 0) return false;
			break;
		case 5:
			if (x == 0) return false;
			if (ChunkData[x - 1][y][z] == 0) return false;
			break;
		case 6:
			if (x == 15) return false;
			if (ChunkData[x + 1][y][z] == 0) return false;
			break;
		}
		return true;
	}

};