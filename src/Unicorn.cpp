#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include"Unicorn.h"

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
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FLOOR 8
#define UNICORN_DIMENSION 90
#define BACKGROUND_WIDTH 4000
#define BACKGROUND_HEIGHT 960

void Unicorn::Up(SDL_Rect& camera, int& unicornYtoDisplay, int& unicornYtoCollision) {

	if (unicornYtoCollision <= SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2) {
		unicornYtoDisplay -= UNICORN_MOVE;

		if (unicornYtoDisplay <= 0)
			unicornYtoDisplay += UNICORN_MOVE;
	}
	else if(unicornYtoCollision > SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2 && 
		unicornYtoCollision <BACKGROUND_HEIGHT/2+ SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2)
	{
		camera.y -= UNICORN_MOVE;
	}
	else if (unicornYtoCollision >= BACKGROUND_HEIGHT / 2 + SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2) {
		unicornYtoDisplay -= UNICORN_MOVE;
	}

	if (unicornYtoDisplay > unicornYtoCollision)
		unicornYtoDisplay = unicornYtoCollision;

	unicornYtoCollision = camera.y + unicornYtoDisplay;
}

void Unicorn::Down(SDL_Rect& camera, int& unicornYtoDisplay, bool onPlatform, int& newGame, int& unicornYtoCollision) {
	if (onPlatform == 0) {
		if (unicornYtoCollision < SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2) {
			unicornYtoDisplay += UNICORN_MOVE;
		}
		else if (unicornYtoCollision >= SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2 && 
			unicornYtoCollision < BACKGROUND_HEIGHT / 2 + SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2)		{
			camera.y += UNICORN_MOVE;
		}
		else if (unicornYtoCollision >= BACKGROUND_HEIGHT / 2 + SCREEN_HEIGHT / 2 - UNICORN_DIMENSION / 2) {
			unicornYtoDisplay += UNICORN_MOVE;
			if (unicornYtoCollision > BACKGROUND_HEIGHT + UNICORN_DIMENSION)
				newGame = 1;
		}
		unicornYtoCollision = camera.y + unicornYtoDisplay;
	}	
}

void Unicorn::Left(SDL_Rect& camera) {
	camera.x -= 2* UNICORN_MOVE;
	if (camera.x <= 0)
		camera.x = BACKGROUND_WIDTH - SCREEN_WIDTH;
}

void Unicorn::Right(SDL_Rect& camera) {
	camera.x += 2* UNICORN_MOVE;
	if (camera.x >= BACKGROUND_WIDTH - SCREEN_WIDTH)
		camera.x = 0;
}

void Unicorn::Dash(SDL_Rect& camera, int speed) {
	camera.x += 2.5*speed;
	if (camera.x >= BACKGROUND_WIDTH - SCREEN_WIDTH)
		camera.x = 0;
}

void  Unicorn::Jump(SDL_Rect& camera, int& jumpLimit, int dash, 
	int& unicornYtoDisplay, int& unicornYtoCollision) {

	if (dash != 2) {
		if (camera.y == 0) {
			if (jumpLimit > 0 && unicornYtoDisplay>0) {
				unicornYtoDisplay -= UNICORN_MOVE;
				jumpLimit -= UNICORN_MOVE;
			}
			else		
				unicornYtoDisplay += UNICORN_MOVE;
			
		}
		else if(camera.y==480)
		{
			if (jumpLimit > 0 && unicornYtoDisplay > 0) {
				unicornYtoDisplay -= UNICORN_MOVE;
				jumpLimit -= UNICORN_MOVE;
			}
			else 			
				unicornYtoDisplay += UNICORN_MOVE;
			
		}
		else {
			if (jumpLimit > 0 && unicornYtoDisplay > 0) {
				camera.y -= UNICORN_MOVE;
				jumpLimit -= UNICORN_MOVE;
			}
			else 			
			    camera.y += UNICORN_MOVE;			
		}
	}
	unicornYtoCollision = camera.y + unicornYtoDisplay;
}

void  Unicorn::GravityForce(SDL_Rect& camera, int dash, int onPlatform,
	int& unicornYtoDisplay, int& unicornYtoCollision, int& newGame) {

	if (onPlatform == 0 && dash!=2) {		
		if (camera.y < BACKGROUND_HEIGHT/2)
			camera.y +=  UNICORN_MOVE;
		else {
			unicornYtoDisplay += UNICORN_MOVE;

			if (unicornYtoCollision > BACKGROUND_HEIGHT + UNICORN_DIMENSION)
				newGame = 1;
		}
	unicornYtoCollision = camera.y + unicornYtoDisplay;		
	}	
}
