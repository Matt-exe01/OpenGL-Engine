#include <unordered_map>
#include "../shader/shaderHandler.h"
#include <ctime>
#include "chunk.h"

class World
{
public:

	std::unordered_map<char*, Chunk> world;
	Chunk *mappa[25][25];

	float* ptr;

	World(Camera* camera, Shader* shader) {
		this->ptr = ptr;
		int seed = time(NULL);
		for (int x = 0; x < 25; x++) {
			for (int z = 0; z < 25; z++) {
				mappa[x][z] = new Chunk(camera, shader, x, z, seed);
			}
		}
	};

	void generateWorldMesh() {
		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 25; j++)
			{
				mappa[i][j]->renderChunkMesh();
			}
		}
	}

	void renderWorld() {
		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 25; j++)
			{
				mappa[i][j]->renderMesh();
			}
		}
	}

private:

};