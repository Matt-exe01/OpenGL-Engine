#pragma once

#include <vector>
#include <iostream>
#include "../math/Noises.h"
#include "../math/noise3d.h"
#include "../renderer/renderer.h"


class Chunk
{
public:

	std::vector<float> mesh;

	int xCoord;
	int zCoord;

	int waterLevel = 78;
	int meshLenght = 0;

	Renderer* renderer = NULL;

	int ChunkData[16][128][16];

	Chunk(Camera* camera, Shader* shader, int x, int z, int seed) {

		renderer = new Renderer(camera, shader);
		

		FastNoiseLite gen(seed);
		gen.SetNoiseType(FastNoiseLite::NoiseType_Value);
		gen.SetFrequency(0.03);
		gen.SetFractalOctaves(10);

		initializeNoise3d(seed);


		xCoord = x;
		zCoord = z;

		for (unsigned int x = 0; x < 16; x++) {

			for (unsigned int z = 0; z < 16; z++) {

				float tmp = gen.GetNoise((x + 0.5 + xCoord * 16), (z + 0.5 + zCoord * 16));
				tmp = (tmp / 2) + 0.5;
				tmp = pow(tmp, 3);
				tmp = round(tmp * 54) + 74;
				for (unsigned int y = 64; y < 128; y++) {

					ChunkData[x][y][z] = (y == tmp) ? 2 : (y < tmp - 2) ? 1 : (y < tmp) ? 3 : 0;
					if (ChunkData[x][y][z] == 0 && y < waterLevel) {
						ChunkData[x][y][z] = 4;
					}
				}

				for (unsigned int y = 0; y < 64; y++) {

					if (getNoise3d((x + 0.5 + xCoord * 16), y, (z + 0.5 + zCoord * 16)) <= 0.1) {
						ChunkData[x][y][z] = 1;
					} else {
						ChunkData[x][y][z] = 0;
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
				for (int y = 0; y < 128; y++)
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
		float* mesh = new float[6000000];

		std::vector<float> vecMesh = generateChunkMesh();
		this->mesh.resize(0);
		meshLenght = vecMesh.size();
		std::copy(vecMesh.begin(), vecMesh.end(), mesh);

		renderer->setMeshBuffer(mesh, (meshLenght / 48));
		delete[] mesh;
	}

	void renderMesh() {
		renderer->setLightningAttrib(glm::vec3(0.05, -1.0f, -1.2f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.6f, 0.6f, 0.6f));
		renderer->renderMesh((meshLenght / 48));
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
		if (ChunkData[x % 16][y][z % 16] != 4) {
			ChunkData[x % 16][y][z % 16] = 0;
			setChunkMeshBuffer();
		}
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
			float BackVertices[48] = {
				(-0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, 0.0f, -1.0f,
				(-0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, 0.0f, -1.0f,
				( 0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, 0.0f, -1.0f,
				( 0.5f + (chunkX) + xToAdd),  ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, 0.0f, -1.0f,
				( 0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, 0.0f, -1.0f,
				(-0.5f + (chunkX) + xToAdd),  (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, 0.0f, -1.0f,
			};

			mesh.insert(mesh.end(), std::begin(BackVertices), std::end(BackVertices));
		}
			break;

		//  =====  TOP  =====
		case 2:
		{
			float TopVertices[48] = {
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, 1.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, 1.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, 1.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, 1.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, 1.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd),  (0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, 1.0f, 0.0f,
			};

			mesh.insert(mesh.end(), std::begin(TopVertices), std::end(TopVertices));
		}
			break;

		//  =====  FRONT  =====
		case 3:
		{
			float FrontVertices[48] = {
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f,  0.0f, 1.0f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f,  0.0f, 1.0f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f,  0.0f, 1.0f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f,  0.0f, 1.0f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f,  0.0f, 1.0f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f,  0.0f, 1.0f,
			};

			mesh.insert(mesh.end(), std::begin(FrontVertices), std::end(FrontVertices));
		}
			break;

		//  =====  BOTTOM  =====
		case 4:
		{
			float BottomVertices[48] = {
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, -1.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, -1.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, -1.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, -1.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 0.0f, -1.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 0.0f, -1.0f, 0.0f,
			};

			mesh.insert(mesh.end(), std::begin(BottomVertices), std::end(BottomVertices));
		}
			break;

		//  =====  LEFT  =====
		case 5:
		{
			float LeftVertices[48] = {
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, -1.0f, 0.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, -1.0f, 0.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, -1.0f, 0.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, -1.0f, 0.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, -1.0f, 0.0f, 0.0f,
				(-0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, -1.0f, 0.0f, 0.0f,
			};

			mesh.insert(mesh.end(), std::begin(LeftVertices), std::end(LeftVertices));
		}
			break;

		//  =====  RIGHT  =====
		case 6:
		{
			float RightVertices[48] = {
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 1.0f, 0.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (1.0f + yOffset)/6, 1.0f, 0.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 1.0f, 0.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), ( 0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (1.0f + yOffset)/6, 1.0f, 0.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), ( 0.5f + (chunkZ) + zToAdd), (0.0f + xOffset)/nType, (0.0f + yOffset)/6, 1.0f, 0.0f, 0.0f,
				( 0.5f + (chunkX) + xToAdd), (-0.5f + chunkY), (-0.5f + (chunkZ) + zToAdd), (1.0f + xOffset)/nType, (0.0f + yOffset)/6, 1.0f, 0.0f, 0.0f,
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
			if (y == 127) return false;
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