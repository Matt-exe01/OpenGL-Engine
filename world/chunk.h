#pragma once

#include <vector>
#include <iostream>
#include "../math/Noises.h"
#include "../renderer/renderer.h"


class Chunk
{
public:

	std::vector<float> mesh;

	int xCoord;
	int zCoord;

	int waterLevel = 14;
	int meshLenght = 0;

	Renderer* renderer = NULL;

	int ChunkData[16][64][16];

	Chunk(Camera* camera, Shader* shader, int x, int z, int seed) {

		renderer = new Renderer(camera, shader);

		FastNoiseLite gen(seed);
		gen.SetNoiseType(FastNoiseLite::NoiseType_Value);
		gen.SetFrequency(0.03);
		gen.SetFractalOctaves(10);

		xCoord = x;
		zCoord = z;

		for (unsigned int x = 0; x < 16; x++)
		{
			for (unsigned int z = 0; z < 16; z++)
			{
				float tmp = gen.GetNoise((x + 0.5 + xCoord * 16), (z + 0.5 + zCoord * 16));
				tmp = (tmp / 2) + 0.5;
				tmp = pow(tmp, 3);
				tmp = round(tmp * 54) + 10;
				for (unsigned int y = 0; y < 64; y++)
				{
					ChunkData[x][y][z] = (y == tmp) ? 2 : (y < tmp - 2) ? 1 : (y < tmp) ? 3 : 0;
					if (ChunkData[x][y][z] == 0 && y < waterLevel) {
						ChunkData[x][y][z] = 4;
					}
				}
			}
		}

		std::cout << "CHUNK " << x << "-" << z << "\tDONE!\n";
	};

	std::vector<float> generateChunkMesh() {
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
								addFaceToMesh(i, ChunkData[x][y][z], x, y, z);
							}
						}
					}
				}
			}
		}
		return mesh;
	}

	void setChunkMeshBuffer() {
		float *mesh = new float[4000000];

		std::vector<float> vecMesh = generateChunkMesh();
		this->mesh.resize(0);
		meshLenght = vecMesh.size();
		std::copy(vecMesh.begin(), vecMesh.end(), mesh);

		renderer->setMeshBuffer(mesh, (meshLenght / 36));
		delete[] mesh;
	}

	void renderMesh() {
		renderer->renderMesh((meshLenght / 36));
	}

	int getBlockAt(glm::vec3 posInChunk) {
		int x = posInChunk.x;
		int y = posInChunk.y;
		int z = posInChunk.z;
		int tmp = ChunkData[x % 16][y][z % 16];
		return tmp;
	}

	void breakBlockAt(glm::vec3 posInChunk) {
		int x = posInChunk.x;
		int y = posInChunk.y;
		int z = posInChunk.z;
		ChunkData[x % 16][y][z % 16] = 0;
		setChunkMeshBuffer();
	}

private:

	void addFaceToMesh(int faceDirection, int type, float chunkX, float chunkY, float chunkZ) {

		int xToAdd = xCoord * 16;
		int zToAdd = zCoord * 16;

		int nType = 4;
		float xOffset = type-1;
		float yOffset = faceDirection-1;

		//per ognuno aggiungo la faccia più le coordinate del blocco nel mondo
		switch (faceDirection)
		{
			//  =====  BACK  =====
		case 1:
		{
			float BackVertices[36] = {
				(-0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.8f,
				(-0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.8f,
				( 0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.8f,
				( 0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.8f,
				( 0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.8f,
				(-0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.8f,
			};

			mesh.insert(mesh.end(), std::begin(BackVertices), std::end(BackVertices));
		}
			break;

		//  =====  TOP  =====
		case 2:
		{
			float TopVertices[36] = {
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 1.0f,
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 1.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 1.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 1.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 1.0f,
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 1.0f,
			};

			mesh.insert(mesh.end(), std::begin(TopVertices), std::end(TopVertices));
		}
			break;

		//  =====  FRONT  =====
		case 3:
		{
			float FrontVertices[36] = {
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.8f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.8f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.8f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.8f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.8f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.8f,
			};

			mesh.insert(mesh.end(), std::begin(FrontVertices), std::end(FrontVertices));
		}
			break;

		//  =====  BOTTOM  =====
		case 4:
		{
			float BottomVertices[36] = {
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.6f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.6f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.6f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.6f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.6f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.6f,
			};

			mesh.insert(mesh.end(), std::begin(BottomVertices), std::end(BottomVertices));
		}
			break;

		//  =====  LEFT  =====
		case 5:
		{
			float LeftVertices[36] = {
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.7f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.7f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.7f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.7f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.7f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.7f,
			};

			mesh.insert(mesh.end(), std::begin(LeftVertices), std::end(LeftVertices));
		}
			break;

		//  =====  RIGHT  =====
		case 6:
		{
			float RightVertices[36] = {
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.7f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.7f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.7f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.7f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.7f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.7f,
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