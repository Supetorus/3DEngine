#pragma once
#include "VertexBuffer.h"

namespace nc
{
	class VertexIndexBuffer : public VertexBuffer
	{
	public:
		~VertexIndexBuffer();
		bool Load(const std::string& name, void* null = nullptr) override;

		void CreateIndexBuffer(GLenum indexType, GLsizei count, void* data);

		void Draw(GLenum primitiveType = GL_TRIANGLES) override;

	protected:
		GLuint ibo = 0; // index buffer object

		GLuint indexCount = 0;  	// number of indices in index buffer
		GLenum indexType = 0;	// data type of index (GLushort or GLuint)
	};
}
