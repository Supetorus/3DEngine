#pragma once
#include "Math/MathTypes.h"
#include "Core/Serializable.h"

namespace nc
{
	struct Transform : public ISerializable
	{
		glm::vec3 position;
		float rotation = 0; // in radians
		glm::vec3 scale = { 1,1,1 };

		glm::vec3 localPosition;
		float localRotation = 0; // in radians
		glm::vec3 localScale = { 1,1,1 };

		glm::mat4 matrix;
		
		Transform() {}
		Transform(glm::vec3 position, float rotation = 0, float scale = 1) :
			position{ position }, rotation{ rotation }, scale{ scale } {}
		
		void Update();

		void Update(const glm::mat4& mx);

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	};


}