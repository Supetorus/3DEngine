#include "VertexBuffer.h"

namespace nc
{
	VertexBuffer::VertexBuffer()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	VertexBuffer::~VertexBuffer()
	{
		if (vao) glDeleteVertexArrays(1, &vao);
		if (vbo) glDeleteBuffers(1, &vbo);
		if (ibo) glDeleteBuffers(1, &ibo);
	}

	bool VertexBuffer::Load(const std::string& name, void* null)
	{
		return true;
	}

	void VertexBuffer::CreateVertexBuffer(GLsizei size, GLsizei vertexCount, void* data)
	{
		this->vertexCount = vertexCount;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::SetAttribute(int index, GLint size, GLsizei stride, size_t offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
	}

	void VertexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei indexCount, void* data)
	{
		this->indexType = indexType;
		this->indexCount = indexCount;

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		size_t indexSize = (indexType == GL_UNSIGNED_SHORT) ? sizeof(GLushort) : sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::Draw(GLenum primitiveType)
	{
		glBindVertexArray(vao);
		if (ibo)
		{
			glDrawElements(primitiveType, indexCount, indexType, 0);
		}
		else if (vbo)
		{
			glDrawArrays(primitiveType, 0, vertexCount);
		}
	}
}
