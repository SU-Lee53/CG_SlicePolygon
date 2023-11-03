#pragma once


Coord AdjustMouseCoordinate(Coord coord);

char* filetobuf(const char* filename);

Coord GetRandomCoord();

RGB GetRandomRGB();

void DrawAxis(float length);

void DrawLine(Coord start, Coord end);

void DrawPoint(Coord point);

void SetBackground(RGB rgb);

glm::vec3 GetRandomVec3();