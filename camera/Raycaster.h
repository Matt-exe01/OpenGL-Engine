#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

class Raycaster
{
public:

	glm::vec3 Start;
	glm::vec3 End;

	glm::vec3 Direction;

	int tmpp = 0;

	float Yaw;
	float Pitch;

	Raycaster(glm::vec3 position, glm::vec3 direction, float yaw, float pitch) {
		Start = position;
		End = position;
		Direction = direction;
		Yaw = glm::radians(yaw);
		Pitch = glm::radians(pitch);
	};

	~Raycaster() {
		std::cout << "Raycaster Distrutto!!\n";
	}

	void step(float scale) {
		scale = 0.5f;

		End.x += glm::cos(Yaw)		* scale;
		End.z += glm::sin(Yaw)		* scale;
		End.y += glm::tan(Pitch)	* scale;
		tmpp++;
	}

	float getLength() const
	{
		return glm::distance(Start, End);
	}

	glm::vec3 getEnd() const
	{
		return glm::vec3(floor(End.x), floor(End.y), floor(End.z));
	}

private:

};
