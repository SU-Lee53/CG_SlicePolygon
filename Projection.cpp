#include "pch.h"
#include "Projection.h"


Projection::Projection(float fovy, float aspect, float Near, float Far, float move)
{
	SetPerspectiveProjection(fovy, aspect, Near, Far, move);
}

Projection::Projection(float left, float right, float bottom, float top, float Near, float Far)
{
	SetOrthoProjection(left, right, bottom, top, Near, Far);
}

Projection::~Projection()
{
}

void Projection::SetPerspectiveProjection(float fovy, float aspect, float Near, float Far, float move)
{
	_fovy = fovy;
	_aspect = aspect;
	_near = Near;
	_far = Far;
	_move = move;

	_projMatrix = glm::perspective(_fovy, _aspect, _near, _far);
	// _projMatrix = glm::translate(_projMatrix, glm::vec3(0.0, 0.0, -2.0));
	_projMatrix = _projMatrix * GET_SINGLE(TransformManager).GetTranslateMatrix(glm::vec3(0.0f, 0.0f, _move));
}

void Projection::SetOrthoProjection(float left, float right, float bottom, float top, float Near, float Far)
{
	_left = left;
	_right = right;
	_bottom = bottom;
	_top = top;
	_near = Near;
	_far = Far;

	_projMatrix = glm::ortho(_left, _right, _bottom, _top, _near, _far);
}

void Projection::Bind(unsigned int ShaderID)
{
	unsigned int projectionLocation = glGetUniformLocation(ShaderID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &_projMatrix[0][0]);
}
