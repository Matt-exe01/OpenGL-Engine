#include <unordered_map>
#include "chunk.h"

class World
{
public:

	std::unordered_map<char*, Chunk> world;
	Chunk *mappa[16][16];

	float* ptr;

	World(float* ptrMesh) {
		ptr = ptrMesh;
		for (int x = 0; x < 15; x++) {
			for (int z = 0; z < 15; z++) {
				mappa[x][z] = new Chunk(x, z);
			}
		}
	};

	int getWorldMesh() {
		std::vector<float> finalMesh;
		int faceAdded = 0;

		for (int x = 0; x < 15; x++) {
			for (int z = 0; z < 15; z++) {
				std::vector<float> tmp = mappa[x][z]->generateChunkMesh();
				faceAdded += (tmp.size()/36);
				finalMesh.insert(finalMesh.end(), tmp.begin(), tmp.end());
			}
		}

		std::copy(finalMesh.begin(), finalMesh.end(), ptr);
		return faceAdded;
	}

private:

};