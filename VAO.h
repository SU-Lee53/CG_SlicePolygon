#pragma once

enum VAO_TYPE
{
	NON_INDEXED = 0,
	INDEXED
};


class VAO
{
public:
	// 정점, 색상 버퍼만 받는 생성자
	VAO(float* vertex, float* color, int bufSize);
	// 정점, 색상, 인덱스 버퍼를 모두 받는 생성자
	VAO(float* vert, float* color, unsigned int* element, int vBufSize, int eBufSize);
	~VAO();

	// VAO_TYPE: NON_INDEXED or INDEXED
	void InitVAO(VAO_TYPE type);

	void ChangeElementBuffer(unsigned int* element, int eBufSize);

	unsigned int GetVAOHandle() { return h_VAO; }
	int GetVertexCount() { return _vBufSize; }
	int GetElementCount() { return _eBufSize; }
	float* GetVertexBuffer() { return _vbo.GetVertexBuf(); }
	float* GetColorBuffer() { return _vbo.GetColorBuf(); }
	unsigned int* GetElementBuffer() { return _vbo.GetElementBuf(); }

private:

	unsigned int h_VAO = 0;		// VAO 핸들
	VBO _vbo;				
	int _vBufSize;	// == cBufSize
	int _eBufSize;

};

