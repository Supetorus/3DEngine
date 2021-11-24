#pragma once
#include "GraphicsComponent.h"
#include "Graphics/Material.h"
#include "Graphics/VertexIndexBuffer.h"

namespace nc
{
	class Renderer;

	class MeshComponent : public GraphicsComponent
	{
	public:
		virtual void Update() override;
		virtual void Draw(Renderer* renderer) override;

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		std::shared_ptr<VertexIndexBuffer> vertexBuffer;
		std::shared_ptr<Material> material;
		std::shared_ptr<Program> program;
	};
}

