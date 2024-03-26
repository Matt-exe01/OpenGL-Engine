#pragma once

#include "../camera/camera.h"
#include "../shader/shaderHandler.h"
#include "../utils/vertexData.h"

#include <string>
#include <glm/glm.hpp>

class Renderer
{
public:

	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

	Camera* CameraObj = NULL;
	Shader* ShaderHandler = NULL;
	Shader* depthBufferShader = NULL;

	glm::mat4 projection;
	glm::mat4 view;
	unsigned int VBO, VAO;
	unsigned int depthMapFBO;
	unsigned int depthMap;

	glm::mat4 lightSpaceMatrix;

	Renderer(Camera* camera, Shader* shader) {
		CameraObj = camera;
		ShaderHandler = shader;


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	};


	void renderMesh(int shader, int length) {

		Shader* shaders = ((shader == 1) ? depthBufferShader : ShaderHandler);
		shaders->use();

		//Creo le matrici di rendering
		projection = glm::perspective(glm::radians(60.0f), (float)1920 / (float)1080, 0.1f, 1000.0f);
		view = CameraObj->GetViewMatrix();

		//applico le matrici alla shader
		shaders->setMat4("projection", projection);
		shaders->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		shaders->setMat4("model", model);

		shaders->setVec3("viewPos", CameraObj->Position);
		shaders->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		if (shader == 2) {
			ShaderHandler->use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, shaders->texture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
		}

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, (length*8));

		glBindVertexArray(0);
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
		glBufferData(GL_ARRAY_BUFFER, (sizeof(mesh) * length * 48), mesh, GL_STATIC_DRAW);

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

	void setLightningAttrib(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 color) {
		
		ShaderHandler->setVec3("light.direction", direction);

		// light properties
		ShaderHandler->setVec3("light.ambient", ambient);
		ShaderHandler->setVec3("light.diffuse", diffuse);
		ShaderHandler->setVec3("light.color", color);
	}


	void configureDepthBuffer(Shader* depthShader) {

		depthBufferShader = depthShader;

		// configure depth map FBO
		// -----------------------
		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ShaderHandler->setInt("shadowMap", 1);
	}

	void renderDepthBuffer(int lenght, glm::vec3 lightDir) {
		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		float near_plane = 0.1f, far_plane = 250.0f;
		lightProjection = glm::ortho(-15.0f, 25.0f, -10.0f, 40.0f, near_plane, far_plane);
		lightView = glm::lookAt((glm::vec3(64, 128, 64) * (-lightDir)), glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		
		/*
		//---------\\
		    "ciao"
				cit. Emilio Leonardi
		\\---------//		
		*/

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		this->renderMesh(1, lenght);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// reset viewport
		glViewport(0, 0, 1920, 1080);
	}

	void renderQuadDebug(Shader* shad, int lenght) {
		shad->use();
		shad->setFloat("near_plane", 0.1f);
		shad->setFloat("far_plane", 200.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		if (quadVAO == 0) {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

private:

};