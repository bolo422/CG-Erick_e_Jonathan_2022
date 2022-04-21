#include <glm/glm.hpp>

#pragma once
class ModelTransform
{
private:

#pragma region main_atributes

	glm::vec3 position;
	glm::vec3 rotation;
	float scale;
	

	float angle[3];

#pragma endregion


public:

#pragma region constructors

	ModelTransform()
	{
		// renomear angle = rotation & apagar rotation
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = 1.0f;
		angle[0] = 0.0f; angle[1] = 0.0f; angle[2] = 0.0f;
	}
	ModelTransform(glm::vec3 _position, glm::vec3 _rotation, float _scale)
	{
		position = _position;
		rotation = _rotation;
		scale = _scale;
	}
	~ModelTransform() {}

#pragma endregion


#pragma region getters_setters

	void setPosition(glm::vec3 _position)
	{
		position = _position;
	}

	void setRotation(glm::vec3 _rotation)
	{
		rotation = _rotation;
	}

	void setScale(float _scale)
	{
		scale = _scale;
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	glm::vec3 getRotation()
	{
		return rotation;
	}

	float getScale()
	{
		return scale;
	}

	glm::vec3 getVecScale()
	{
		return glm::vec3(scale, scale, scale);
	}

	float getAngle(int index)
	{
		return angle[index];
	}

	glm::vec3 getVecAngle()
	{
		return glm::vec3(angle[0], angle[1], angle[2]);
	}

#pragma endregion

	//0 = X, 1 = Y, 2 = Z
	void addAngle(int index, float value)
	{
		angle[index] += value;
	}

	void addScale(float value)
	{
		scale += value;
		scale = glm::clamp(scale, 0.0f, 20.0f);
	}

	void addPosition(glm::vec3 pos)
	{
		position += pos;
	}

	void addPosition(int index, float value)
	{
		position[index] += value;
	}
};

