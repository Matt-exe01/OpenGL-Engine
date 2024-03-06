#pragma once

#include "../camera/camera.h"
#include "../shader/shaderHandler.h"
#include "../utils/vertexData.h"

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

		glm::mat4 model = glm::mat4(1.0f);
		ShaderHandler->setMat4("model", model);

		ShaderHandler->setVec3("viewPos", CameraObj->Position);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, (length*8));
	}

	void renderSkybox(unsigned int cubemapTexture) {
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		ShaderHandler->use();
		ShaderHandler->setMat4("view", glm::mat4(glm::mat3(CameraObj->GetViewMatrix())));
		ShaderHandler->setMat4("projection", glm::perspective(glm::radians(CameraObj->Zoom), (float)1920 / (float)1080, 0.1f, 100.0f));
		// skybox cube
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}


	void setMeshBuffer(float* mesh, int length) {

		//Creo i buffer
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(mesh) * length * 64), mesh, GL_STATIC_DRAW);

		// Attributi positione
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Attributi texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// Attributi normali
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setSkyboxBuffer() {
		// skybox VAO
		unsigned int skyboxVAO, skyboxVBO;
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		VAO = skyboxVAO;
		VBO = skyboxVBO;
	}

	void setLightningAttrib(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse) {
		
		ShaderHandler->setVec3("light.direction", direction);

		// light properties
		ShaderHandler->setVec3("light.ambient", ambient);
		ShaderHandler->setVec3("light.diffuse", diffuse);
	}

private:

};