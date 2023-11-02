#pragma once

class VBO
{
public:
	VBO();
	~VBO();

	// VBO객체 내부 버퍼에 생성된 도형의 버퍼를 집어넣습니다.
	void PushToVertexBuffer(float* buf, int bufSize);
	void PushToColorBuffer(float* buf, int bufSize);

	// VBO 객체 내부 eBuf(element)에 인덱스를 넣습니다.
	void PushToElementBuffer(unsigned int* buf, int ebufSize);

	unsigned int* GetVertexHandle() { return &vBufHandle; }
	unsigned int* GetColorHandle() { return &cBufHandle; }
	unsigned int* GetElementHandle() { return &eBufHandle; }
	float* GetVertexBuf() { return vBuf; }
	float* GetColorBuf() { return cBuf; }
	unsigned int* GetElementBuf() { return eBuf; }

private:
	unsigned int vBufHandle = 0;
	unsigned int cBufHandle = 0;
	unsigned int eBufHandle = 0;
	float vBuf[256];
	float cBuf[256];
	unsigned int eBuf[256];
};

