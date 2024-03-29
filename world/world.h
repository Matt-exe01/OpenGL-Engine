#include <unordered_map>
#include "../shader/shaderHandler.h"
#include <ctime>
#include "chunk.h"
#include "../camera/Raycaster.h"
#include "../renderer/renderer.h"
class World
{
public:

	Chunk *mappa[1][1];

	int size = 1;

	Camera* camera;
	Shader* shader;

	Renderer* skyboxRenderer;

	

	World(Camera* camera, Shader* shader, Shader* depth, Shader* debug) {
		this->camera = camera;
		this->shader = shader;
		int seed = time(NULL);
		seed = 1;
		for (int x = 0; x < size; x++) {
			for (int z = 0; z < size; z++) {
				mappa[x][z] = new Chunk(camera, shader, depth, debug, x, z, seed);
			}
		}
	};

	void generateWorldMesh() {
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				mappa[i][j]->setChunkMeshBuffer();
			}
		}
	}

	void renderWorld(glm::vec3 lightDir) {
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				//TODO Controllare se il chunk � visibile dalla camera
				
				//if chunk visible
				mappa[i][j]->renderMesh(lightDir);
			}
		}
	}

	void LaunchCollisionRay() {

		Raycaster* ray = new Raycaster(camera->Position, camera->Yaw, camera->Pitch);

		bool searchEnded = false;

		while (!searchEnded)
		{
			glm::vec3 RayEnd = ray->getEnd();
			int chunkX = RayEnd.x / 16;
			int chunkZ = RayEnd.z / 16;
			if (mappa[chunkX][chunkZ]->getBlockAt(RayEnd) != 0) {
				mappa[chunkX][chunkZ]->breakBlockAt(RayEnd);
				searchEnded = true;
			}
			else {
				if (ray->tmpp < 400 && RayEnd.y > 0 && RayEnd.y < 129) {
					ray->step(10);
				}
				else {
					searchEnded = true;
				}
			}
		}
		ray->~Raycaster();
	}

private:

};