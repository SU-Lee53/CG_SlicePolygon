#include "pch.h"
#include "VAO.h"



VAO::VAO(float* vertex, float* color, int bufSize) : _vBufSize(bufSize), _eBufSize(0)
{
	this->_vbo.PushToVertexBuffer(vertex, _vBufSize);
	this->_vbo.PushToColorBuffer(color, _vBufSize);
	InitVAO(NON_INDEXED);
}

VAO::VAO(float* vertex, float* color, unsigned int* element, int v_bufSize, int e_bufSize) : _vBufSize(v_bufSize), _eBufSize(e_bufSize)
{
	this->_vbo.PushToVertexBuffer(vertex, _vBufSize);
	this->_vbo.PushToColorBuffer(color, _vBufSize); 
	this->_vbo.PushToElementBuffer(element, _eBufSize);
	InitVAO(INDEXED);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &this->h_VAO);
}

void VAO::ChangeElementBuffer(unsigned int* element, int eBufSize)
{
	_vbo.PushToElementBuffer(element, eBufSize);
	_eBufSize = eBufSize;
	InitVAO(INDEXED);
}

void VAO::InitVAO(VAO_TYPE type)	// elementMode가 true면 EBO를 사용하므로 버퍼 3개 생성함
{
	bool GenBufs = false;	// vbo를 생성할지 안할지 결정하는 플래그

	if (!h_VAO)
	{
		glGenVertexArrays(1, &h_VAO);
		GenBufs = true;
	}

	glBindVertexArray(h_VAO);

	if (GenBufs == true)
	{
		glGenBuffers(1, _vbo.GetVertexHandle());
		glGenBuffers(1, _vbo.GetColorHandle());
		if (type == INDEXED)
			glGenBuffers(1, _vbo.GetElementHandle());
	}


	glBindBuffer(GL_ARRAY_BUFFER, *_vbo.GetVertexHandle());
	glBufferData(GL_ARRAY_BUFFER, _vBufSize * sizeof(float), _vbo.GetVertexBuf(), GL_DYNAMIC_DRAW);
	if (type == INDEXED)	// element 버퍼 사용시에는 vbo 하나 더 사용
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_vbo.GetElementHandle());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _eBufSize * sizeof(unsigned int), _vbo.GetElementBuf(), GL_DYNAMIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, *_vbo.GetColorHandle());
	glBufferData(GL_ARRAY_BUFFER, _vBufSize * sizeof(float), _vbo.GetColorBuf(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	
}


