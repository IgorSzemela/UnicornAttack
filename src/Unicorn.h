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

#define UNICORN_MOVE 5

class Unicorn
{
public:
	void Up(SDL_Rect& camera, int& unicornYtoDisplay, int& unicornYtoCollision);
	
	void Down(SDL_Rect& camera, int& unicornYtoDisplay, bool onPlatform, int& newGame, int& unicornYtoCollision);
	
	void Left(SDL_Rect& camera);
	
	void Right(SDL_Rect& camera);

	void Dash(SDL_Rect& camera, int speed);

	void Jump(SDL_Rect& camera, int& jumpLimit, int dash, 
		int& unicornYtoDisplay, int& unicornYtoCollision);

	void GravityForce(SDL_Rect& camera, int dash, int onPlatform,//unicorn is falling to the ground
		int& unicornYtoDisplay, int& unicornYtoCollision, int& newGame);
};  