#pragma once

#include "../camera/camera.h"
#include "../shader/shaderHandler.h"

#include <string>

class Renderer
{
public:

	Camera* CameraObj = NULL;
	Shader* ShaderHandler = NULL;

	glm::mat4 projection;
	glm::mat4 view;
	unsigned int VBO, VAO;

	Renderer(Camera* camera, Shader* shader) {
		CameraObj = camera;
		ShaderHandler = shader;


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	};


	void renderMesh(int length) {

		//Creo le matrici di rendering
		projection = glm::perspective(glm::radians(60.0f), (float)1920 / (float)1080, 0.1f, 1000.0f);
		view = glm::lookAt(CameraObj->Position, CameraObj->Position + CameraObj->Front, CameraObj->Up);

		//applico le matrici alla shader
		ShaderHandler->setMat4("projection", projection);
		ShaderHandler->setMat4("view", view);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, (length*6));
	}


	void setBuffer(float mesh[], int length) {

		//Creo i buffer
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(mesh) * length * 36), mesh, GL_STATIC_DRAW);

		//std::cout << length << "\n";

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
	}

private:

};