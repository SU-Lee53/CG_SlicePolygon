#include "pch.h"
#include "VBO.h"

VBO::VBO()
{	
}

VBO::~VBO()
{
	glDeleteBuffers(1, &this->vBufHandle);
	glDeleteBuffers(1, &this->cBufHandle);
	glDeleteBuffers(1, &this->eBufHandle);
}

void VBO::PushToVertexBuffer(float* buf, int bufSize)
{
	memset(this->vBuf, 0.0f, sizeof(float) * 256);
	memcpy(this->vBuf, buf, sizeof(float) * bufSize);
}

void VBO::PushToColorBuffer(float* buf, int bufSize)
{
	memset(this->cBuf, 0.0f, sizeof(float) * 256);
	memcpy(this->cBuf, buf, sizeof(float) * bufSize);
}

void VBO::PushToElementBuffer(unsigned int* eBuf, int ebufSize)
{
	memset(this->eBuf, 0, sizeof(unsigned int) * 256);
	memcpy(this->eBuf, eBuf, sizeof(unsigned int) * ebufSize);
}
