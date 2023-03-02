#pragma once

#include <vector>
#include <iterator>
#include <iostream>


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

		mesh.reserve(1000000);

		for (unsigned int x = 0; x < 16; x++)
		{
			for (unsigned int z = 0; z < 16; z++)
			{
				for (unsigned int y = 0; y < 16; y++)
				{
					ChunkData[x][y][z] = 1;
				}
			}
		}
	};

	int generateChunkMesh() {
		int elemAdded = 0;

		for (float x = 0; x < 16; x++)
		{
			for (float z = 0; z < 16; z++)
			{
				for (float y = 0; y < 16; y++)
				{
					//per ogni blocco controllo le adiacenze (face culling, non aggiungo al buffer le facce che non vedo)
					for (unsigned int i = 1; i <= 6; i++)
					{
						addFaceToMesh(i, x, y, z);
						elemAdded++;
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
				(-0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
				(-0.5f + (chunkX)), ( 0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 1.0f,
				(0.5f + (chunkX)),  ( 0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 1.0f,
				(0.5f + (chunkX)),  ( 0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 1.0f,
				(0.5f + (chunkX)),  (-0.5f + chunkY), (-0.5f + (chunkZ)), 1.0f, 0.0f,
				(-0.5f + (chunkX)), (-0.5f + chunkY), (-0.5f + (chunkZ)), 0.0f, 0.0f,
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

};