#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

class Raycaster
{
public:

	glm::vec3 Start;
	glm::vec3 End;

	int tmpp = 0;

	float Yaw;
	float Pitch;

	Raycaster(glm::vec3 position, float yaw, float pitch) {
		Start = position;
		End = position;
		Yaw = glm::radians(yaw);
		Pitch = glm::radians(pitch);
	};

	~Raycaster() {}

	void step(float scale) {
		scale = 0.25f;

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
