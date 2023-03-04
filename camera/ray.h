#pragma once

#include "glm/glm.hpp"

class Ray
{
public:
	Ray(const glm::vec3& position, const glm::vec3& direction) {
        m_rayStart = position;
        m_rayEnd = position;
        m_direction = direction;
    }

    void step() {
        float yaw = glm::radians(m_direction.y + 90);
        float pitch = glm::radians(m_direction.x);

        auto& p = m_rayEnd;

        p.x -= glm::cos(yaw);
        p.z -= glm::sin(yaw);
        p.y -= glm::tan(pitch);
    }
    

    const glm::vec3& getEnd() const
    {
        return m_rayEnd;
    }

    float getLength() const
    {
        return glm::distance(m_rayStart, m_rayEnd);
    }

private:
    glm::vec3 m_rayStart;
    glm::vec3 m_rayEnd;
    glm::vec3 m_direction;
};