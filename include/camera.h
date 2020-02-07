#pragma once
#include <glm.hpp>
#include <glew.h>
#include <movable.h>

class Camera : public MovableObject
{
public:
	virtual const glm::mat4& projection() const = 0;

	void lookAt(glm::vec3 point, float duration = -1.0f)
	{
		glm::vec3 translation = this->translation();
		glm::mat4 transform = glm::lookAt(translation, point, glm::vec3(0.0f, 1.0f, 0.0f));

		this->transformTo(transform, duration);
	}

private:
	void initMovable(const GLuint& vao, const GLuint& vbo) { }
	void updateMovable(const float& totalTime, const float& frameTime) { }
};

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(
		float left,
		float right,
		float bottom,
		float top,
		float verticalFov,
		float nearClipPlane,
		float farClipPlane)
	{
		m_projection = glm::ortho(left, right, bottom, top, nearClipPlane, farClipPlane);
	}

	const glm::mat4& projection() const
	{
		return m_projection;
	}

private:
	glm::mat4 m_projection;
};

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(
		unsigned int width,
		unsigned int height,
		float verticalFov,
		float nearClipPlane,
		float farClipPlane)
	{
		m_projection = glm::perspective(glm::radians(verticalFov), width / (float)height, nearClipPlane, farClipPlane);
	}

	const glm::mat4& projection() const
	{
		return m_projection;
	}

private:
	glm::mat4 m_projection;
};