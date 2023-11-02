#pragma once


Coord AdjustMouseCoordinate(Coord coord);

char* filetobuf(const char* filename);

Coord GetRandomCoord();

RGB GetRandomRGB();

void DrawAxis(float length);

void DrawLine(Coord start, Coord end);

void SetBackground(RGB rgb);