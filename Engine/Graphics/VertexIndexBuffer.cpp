#include "VertexIndexBuffer.h"

namespace nc
{
	VertexIndexBuffer::~VertexIndexBuffer()
	{
		if (ibo != 0) glDeleteBuffers(1, &ibo);
	}

	bool VertexIndexBuffer::Load(const std::string& name, void* null)
	{
		VertexBuffer::Load(name, null);

		return true;
	}

	void VertexIndexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei indexCount, void* data)
	{
		this->indexType = indexType;
		this->indexCount = indexCount;

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		size_t indexSize = (indexType == GL_UNSIGNED_SHORT) ? sizeof(GLushort) : sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, data, GL_STATIC_DRAW);
	}

	void VertexIndexBuffer::Draw(GLenum primitiveType)
	{
		glBindVertexArray(vao);
		glDrawElements(primitiveType, indexCount, GL_UNSIGNED_INT, 0);
	}
}
