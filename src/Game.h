#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

#ifdef __cplusplus
extern "C"
#endif
class Game{
public:
	
	// draw a text text on surface screen, starting from the point (x, y)
	void DrawString(SDL_Surface* screen, int x, int y, const char* text,
		SDL_Surface* charset);
	
	// draw a surface sprite on a surface screen in point (x, y)
	void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

	void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

	// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
	void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

	// draw a rectangle of size l by k
	void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
		Uint32 outlineColor, Uint32 fillColor);

	// draw a border of size l by k
	void DrawBorder(SDL_Surface* screen, Uint32 outlineColor, int gameWidth, int  gameHeight);
};

