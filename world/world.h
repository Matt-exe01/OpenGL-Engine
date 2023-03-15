#include <unordered_map>
#include "../shader/shaderHandler.h"
#include <ctime>
#include "chunk.h"
#include "../camera/Raycaster.h"

class World
{
public:

	std::unordered_map<char*, Chunk> world;
	Chunk *mappa[25][25];
	float* ptr;

	unsigned int VAO2, VBO2;

	int size = 10;

	Camera* camera;
	Shader* shader;

	World(Camera* camera, Shader* shader) {
		this->camera = camera;
		this->shader = shader;
		this->ptr = ptr;
		int seed = time(NULL);
		for (int x = 0; x < size; x++) {
			for (int z = 0; z < size; z++) {
				mappa[x][z] = new Chunk(camera, shader, x, z, seed);
			}
		}

		float crossHairVert[36] = {
				-1.0f,  -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
				-1.0f,   1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
				 1.0f,   1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
				 1.0f,   1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
				 1.0f,  -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
				-1.0f,  -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		};


		/*TEMPORANEO - Serve un modo migliore per farlo*/
		//Creo i buffer
		glGenVertexArrays(1, &VAO2);
		glGenBuffers(1, &VBO2);

		glBindVertexArray(VAO2);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 6 * 36), crossHairVert, GL_STATIC_DRAW);

		// Attributi positione
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Attributi texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// Attributi luminosità
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	};

	void generateWorldMesh() {
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				mappa[i][j]->renderChunkMesh();
			}
		}
	}

	void renderWorld() {
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				mappa[i][j]->renderMesh();

				
				unbindMatrix();
				glBindVertexArray(VAO2);
				glDisable(GL_DEPTH_TEST);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glEnable(GL_DEPTH_TEST);
			}
		}
	}

	void LaunchCollisionRay() {

		Raycaster* ray = new Raycaster(camera->Position, camera->Front, camera->Yaw, camera->Pitch);

		bool searchEnded = false;

		while (!searchEnded)
		{
			glm::vec3 RayEnd = ray->getEnd();
			int chunkX = RayEnd.x / 16;
			int chunkZ = RayEnd.z / 16;
			if (mappa[chunkX][chunkZ]->getBlockAt(RayEnd) != 0) {
				mappa[chunkX][chunkZ]->breakBlockAt(RayEnd);
				searchEnded = true;
				std::cout << "Condizione di cubo trovato!!\n";
			}
			else {
				if (ray->tmpp < 100 && RayEnd.x > 0 && RayEnd.y > 0 && RayEnd.y < 65 && RayEnd.z > 0) {
					ray->step(10);
				}
				else {
					searchEnded = true;
					std::cout << "Condizione di Limite di step!!\n";
				}
			}
		}
		ray->~Raycaster();
	}

private:

	void unbindMatrix() {
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
	}

};