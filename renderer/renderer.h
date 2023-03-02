#pragma once

#include "../camera/camera.h"
#include "../shader/shaderHandler.h"

#include <string>

class Renderer
{
public:

	Camera* CameraObj = NULL;
	Shader* ShaderHandler = NULL;

	Renderer(Camera* camera, Shader* shader) {
		CameraObj = camera;
		ShaderHandler = shader;


		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
	};


	void renderMesh(float mesh[], int length) {

		//Creo le matrici di rendering
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(CameraObj->Position, CameraObj->Position + CameraObj->Front, CameraObj->Up);

		//applico le matrici alla shader
		ShaderHandler->setMat4("projection", projection);
		ShaderHandler->setMat4("view", view);



		//Creo i buffer
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(mesh) * length * 30), mesh, GL_STATIC_DRAW);

		//std::cout << length << "\n";

		// Attributi positione
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Attributi colore
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(VAO);

		glm::mat4 model = glm::mat4(1.0f);
		ShaderHandler->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, (length*6));
	}

private:

};