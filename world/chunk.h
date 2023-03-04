#pragma once

#include <vector>
#include <iostream>


class Chunk
{
public:

	std::vector<float> mesh;

	int xCoord;
	int zCoord;

	int ChunkData[16][64][16];

	Chunk(int x, int z) {
		xCoord = x;
		zCoord = z;

		for (unsigned int x = 0; x < 16; x++)
		{
			for (unsigned int z = 0; z < 16; z++)
			{
				int colHeight = 63; /*rand() % 44 + 20;*/
				for (unsigned int y = 0; y < 64; y++)
				{
					ChunkData[x][y][z] = (y < colHeight) ? 1 : 0;
				}
			}
		}
	};

	std::vector<float> generateChunkMesh() {
		mesh.clear();
		mesh.reserve(3000000);
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
							}
						}
					}
				}
			}
		}
		//std::copy(mesh.begin(), mesh.end(), ptrMeshBuffer);
		return mesh;
	}

private:

	void addFaceToMesh(int faceDirection, float chunkX, float chunkY, float chunkZ) {

		int xToAdd = xCoord * 16;
		int zToAdd = zCoord * 16;

		//per ognuno aggiungo la faccia più le coordinate del blocco nel mondo
		switch (faceDirection)
		{
			//  =====  BACK  =====
		case 1:
		{
			float BackVertices[36] = {
				(-0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.8f,
				(-0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 0.8f,
				( 0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.8f,
				( 0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.8f,
				( 0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 0.8f,
				(-0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.8f,
			};

			mesh.insert(mesh.end(), std::begin(BackVertices), std::end(BackVertices));
		}
			break;

		//  =====  TOP  =====
		case 2:
		{
			float TopVertices[36] = {
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 1.0f,
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 1.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 1.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 1.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 1.0f,
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 1.0f,
			};

			mesh.insert(mesh.end(), std::begin(TopVertices), std::end(TopVertices));
		}
			break;

		//  =====  FRONT  =====
		case 3:
		{
			float FrontVertices[36] = {
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.8f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.8f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 0.8f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.8f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.8f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 0.8f,
			};

			mesh.insert(mesh.end(), std::begin(FrontVertices), std::end(FrontVertices));
		}
			break;

		//  =====  BOTTOM  =====
		case 4:
		{
			float BottomVertices[36] = {
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.6f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 0.6f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.6f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.6f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 0.6f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.6f,
			};

			mesh.insert(mesh.end(), std::begin(BottomVertices), std::end(BottomVertices));
		}
			break;

		//  =====  LEFT  =====
		case 5:
		{
			float LeftVertices[36] = {
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.7f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 0.7f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.7f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.7f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 0.7f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.7f,
			};

			mesh.insert(mesh.end(), std::begin(LeftVertices), std::end(LeftVertices));
		}
			break;

		//  =====  RIGHT  =====
		case 6:
		{
			float RightVertices[36] = {
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 0.7f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 1.0f, 0.7f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 0.7f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 1.0f, 0.7f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), 0.0f, 0.0f, 0.7f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), 1.0f, 0.0f, 0.7f,
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