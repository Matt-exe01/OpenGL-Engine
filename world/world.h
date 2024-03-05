#include <unordered_map>
#include "../shader/shaderHandler.h"
#include <ctime>
#include "chunk.h"
#include "../camera/Raycaster.h"
#include "../renderer/renderer.h"
class World
{
public:

	Chunk *mappa[10][10];

	int size = 10;

	Camera* camera;
	Shader* shader;

	Renderer* skyboxRenderer;

	

	World(Camera* camera, Shader* shader) {
		this->camera = camera;
		this->shader = shader;
		int seed = time(NULL);
		for (int x = 0; x < size; x++) {
			for (int z = 0; z < size; z++) {
				mappa[x][z] = new Chunk(camera, shader, x, z, seed);
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

	void renderWorld() {
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				//TODO Controllare se il chunk è visibile dalla camera
				
				//if chunk visible
					mappa[i][j]->renderMesh();

				//render skybox

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
				if (ray->tmpp < 400&& RayEnd.y > 0 && RayEnd.y < 65) {
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