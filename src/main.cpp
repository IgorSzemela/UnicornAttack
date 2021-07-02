#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include "Game.h"
#include "Unicorn.h"

#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH 640 
#define SCREEN_HEIGHT 480
#define FLOOR 8
#define ACCELERATION 0.01
#define FPS 30
#define UNICORN_DIMENSION 90
#define JUMP_HEIGHT 100
#define UNICORN_MOVE 5
#define BACKGROUND_WIDTH 4000
#define BACKGROUND_HEIGHT 960

struct Obstacle {
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};
struct Platform {
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};
//detects collision comparing unicorn and obstacle x,y,w,h dimensions
int ObstacleDetection(int unicornX, int  unicornY, Obstacle* obstacle)
{
	int unicornW{ UNICORN_DIMENSION }, unicornH{ UNICORN_DIMENSION };
	if (unicornY >= obstacle->y + obstacle->h)
		return 0;
	if (unicornX >= obstacle->x + obstacle->w)
		return 0;
	if (unicornY + unicornH <= obstacle->y)
		return 0;
	if (unicornX + unicornW <= obstacle->x)
		return 0;
	return 1;
}
//detects collision comparing unicorn and platfrom x,y,w,h dimensions
int ObstacleDetection(int unicornX, int  unicornY, Platform* platform)
{
	int unicornW{ UNICORN_DIMENSION }, unicornH{ UNICORN_DIMENSION };
	if (unicornY >= platform->y + platform->h)
		return 0;
	if (unicornX >= platform->x + platform->w)
		return 0;
	if (unicornY + unicornH <= platform->y)
		return 0;
	if (unicornX + unicornW <= platform->x)
		return 0;
	return 1;
}
//detects being on the platform based on x,y,w,h dimensions
int PlatformDetection(int unicornX, int  unicornY, Platform* platform)
{
	int unicornW{ UNICORN_DIMENSION }, unicornH{ UNICORN_DIMENSION };
	for (int i = 0; i < 7; i++) {
		if (unicornY + unicornH == (platform + i)->y && (unicornX <= (platform + i)->x + (platform + i)->w && unicornX >= (platform + i)->x
			|| unicornX + unicornW >= (platform + i)->x && unicornX + unicornW <= (platform + i)->x + (platform + i)->w)) {
			return 1;
		}
	}
	return 0;
}

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {
	Unicorn unicorn;
	Game game;
	int t1, t2, quit{ 0 }, frames, rc, newGame{ 1 }, newSteering{ 0 }, canDash{ 1 }, points{ 0 };
	double delta, worldTime, fpsTimer, fps, distance, speed{ 10 };
	SDL_Event event;
	SDL_Surface* screen, * charset, * background, * unicorn_bmp, *life1, * life2, * life3;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;
	int unicornYToDisplay{ 0 }, jumpLimit{ 0 }, jumpCounter{ 0 }, unicornYToCollision{ 0 }, lifes{ 3 };
	bool firstJump{ 1 }, secondJump{ 0 }, canJump{ 1 }, onPlatform{ 0 };
	double dashDistance{ 200 };

	Obstacle* obstacle;
	obstacle = (Obstacle*)malloc(5 * sizeof(Obstacle));
	obstacle->x = 1045; obstacle->y = 655; obstacle->w = 105; obstacle->h = 45;
	(obstacle + 1)->x = 2445; (obstacle + 1)->y = 355; (obstacle + 1)->w = 105; (obstacle + 1)->h = 45;
	(obstacle + 2)->x = 945; (obstacle + 2)->y = 0; (obstacle + 2)->w = 35; (obstacle + 2)->h = 135;
	(obstacle + 3)->x = 1195; (obstacle + 3)->y = 0; (obstacle + 3)->w = 35; (obstacle + 3)->h = 145;
	(obstacle + 4)->x = 3145; (obstacle + 4)->y = 0; (obstacle + 4)->w = 35; (obstacle + 4)->h = 140;

	Platform* platform;
	platform = (Platform*)malloc(7 * sizeof(Platform));
	platform->x = 0; platform->y = 400 ; platform->w = 640; platform->h = 40;
	(platform + 1)->x = 700; (platform + 1)->y = 250; (platform + 1)->w = 802; (platform + 1)->h = 40;
	(platform + 2)->x = 750; (platform + 2)->y = 700; (platform + 2)->w = 650; (platform + 2)->h = 40;
	(platform + 3)->x = 1600; (platform + 3)->y = 600; (platform + 3)->w = 400; (platform + 3)->h = 40;
	(platform + 4)->x = 2200; (platform + 4)->y = 400; (platform + 4)->w = 600; (platform + 4)->h = 45;
	(platform + 5)->x = 3000; (platform + 5)->y = 250; (platform + 5)->w = 350; (platform + 5)->h = 40;
	(platform + 6)->x = 3362; (platform + 6)->y = 400; (platform + 6)->w = 637; (platform + 6)->h = 40;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,&window, &renderer);
	//rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Robot Unicorn Attack Igor Szemela");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	charset = SDL_LoadBMP("./images/cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	unicorn_bmp = SDL_LoadBMP("./images/uni.bmp");	
	background = SDL_LoadBMP("./images/background.bmp");
	life1 = SDL_LoadBMP("./images/1life.bmp");	
	life2 = SDL_LoadBMP("./images/2life.bmp");
	life3 = SDL_LoadBMP("./images/3life.bmp");

	char text[128];
	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int white = SDL_MapRGB(screen->format, 255, 255, 255);

	t1 = SDL_GetTicks();

	while (!quit) {
		if (newGame == 1) {
			camera.x = 0;
			camera.y = 0;
			camera.w = SCREEN_WIDTH;
			camera.h = SCREEN_HEIGHT;
			frames = 0;
			fpsTimer = 0;
			fps = 0;
			worldTime = 0;
			points = 0;
			distance = 0;
			newGame = 0;

			speed = 10;
			unicornYToDisplay = SCREEN_HEIGHT/2 -UNICORN_DIMENSION/2;
			unicornYToCollision = unicornYToDisplay;
			jumpLimit = 0;
			firstJump = 1;
			secondJump = 0;
			canJump = 0;
			canDash = 1;
			jumpCounter = 0;
			dashDistance = 200;
			onPlatform = 0;
		}
		if (newGame == 0) {
			t2 = SDL_GetTicks();

			delta = (t2 - t1) * 0.001;
			t1 = t2;

			worldTime += delta;
			distance += speed * delta;

			if (newSteering) {
				points += 3;
				speed += ACCELERATION;
				camera.x += speed;
				if (camera.x >= BACKGROUND_WIDTH - SCREEN_WIDTH)
					camera.x = 0;
			}

			SDL_FillRect(screen, NULL, black);

			SDL_BlitSurface(background, &camera, screen, NULL);

			for (int i = 0; i < 5; i++)
			{
				newGame = ObstacleDetection(camera.x, unicornYToCollision, (obstacle + i));
				if (newGame == 1)
					break;
			}
			
			onPlatform = PlatformDetection(camera.x, unicornYToCollision, platform);

			for (int i = 0; i < 7; i++) {//platfrom collision detection
			
				if (newGame == 0 && onPlatform == 0)
					newGame = ObstacleDetection(camera.x, unicornYToCollision, (platform + i));
			
				if (newGame || onPlatform)
					break;
			}
			
			game.DrawSurface(screen, unicorn_bmp, UNICORN_DIMENSION / 2 + 1, unicornYToDisplay + UNICORN_DIMENSION / 2);

			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};

			game.DrawBorder(screen, red, SCREEN_WIDTH, SCREEN_HEIGHT);

			game.DrawRectangle(screen, SCREEN_WIDTH/1.9 , 4, SCREEN_WIDTH /2.1 -4, 20, red, blue);

			sprintf(text, "Points: %d Elapsed time = %.1lf s", points, worldTime);
			game.DrawString(screen, screen->w /1.33 - strlen(text) * 8 / 2, 10, text, charset);

		     if (lifes == 2)
				game.DrawSurface(screen, life2, SCREEN_WIDTH - 105, 53);
			else if (lifes == 1)
				game.DrawSurface(screen, life1, SCREEN_WIDTH - 105, 53);
			else 				
				 game.DrawSurface(screen, life3, SCREEN_WIDTH - 105, 53);
			
		    	
			game.DrawRectangle(screen, 4, 4, SCREEN_WIDTH / 2.1 - 4, 20, red, blue);

			if (!newSteering)sprintf(text, "steering:\032\030\031\033, \144 - default steering");
			else sprintf(text, "\172 - jump, \170 - dash, \156 - new game");

			game.DrawString(screen, screen->w /4.2 - strlen(text) * 8 / 2, 10, text, charset);

			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);

			while (SDL_PollEvent(&event)) {
				if (!newSteering) {
					switch (event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
						else if (event.key.keysym.sym == SDLK_n) newGame = 1;
						else if (event.key.keysym.sym == SDLK_d) {
							newSteering = 1;
							newGame = 1;
						}
						else if (event.key.keysym.sym == SDLK_UP)unicorn.Up(camera, unicornYToDisplay, unicornYToCollision);
						else if (event.key.keysym.sym == SDLK_DOWN)unicorn.Down(camera, unicornYToDisplay, onPlatform, newGame, unicornYToCollision);
						else if (event.key.keysym.sym == SDLK_LEFT)unicorn.Left(camera);
						else if (event.key.keysym.sym == SDLK_RIGHT)unicorn.Right(camera);
						break;
					case SDL_KEYUP:
						break;
					case SDL_QUIT:
						quit = 1;
						break;
					}
				}
				else if (newSteering) {
					switch (event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
						else if (event.key.keysym.sym == SDLK_n) newGame = 1;
						else if (event.key.keysym.sym == SDLK_d) {
							newSteering = 0;
							newGame = 1;
						}
						else if (event.key.keysym.sym == SDLK_z)
						{
							if (firstJump) {
								firstJump = 0;
								jumpCounter = 1;
								jumpLimit = JUMP_HEIGHT;
							}
							if (secondJump) {
								secondJump = 0;
								jumpCounter++;
								jumpLimit = JUMP_HEIGHT;
							}
							canJump = 1;
							unicorn.Jump(camera, jumpLimit, canDash, unicornYToDisplay, unicornYToCollision);
						}
						else if (event.key.keysym.sym == SDLK_x)
						{
							dashDistance -= speed;

							if (canDash) {	
								canDash = 2;
								secondJump = 1;							
							}
							if(canDash==2)
								unicorn.Dash(camera, speed);

							if (dashDistance < 0)
								canDash = 0;
						}
						break;
					case SDL_KEYUP:
						if (event.key.keysym.sym == SDLK_z) {
							if ( jumpCounter == 1) {
								secondJump = 1;
							}
							canJump = 0;
						}
						break;
					case SDL_QUIT:
						quit = 1;
						break;
					}
				}
			};
		};

		if (!canJump && newSteering) {	
			unicorn.GravityForce(camera, canDash, onPlatform, unicornYToDisplay, unicornYToCollision, newGame);

			if (onPlatform) {//if unicorn touches the platfrom		
				firstJump = 1;
				secondJump = 0;
				canDash = 1;
				dashDistance = 200;
			}
		}
		
		if (newGame == 1) {
			if (lifes == 0)
				lifes = 3;
			else
				lifes--;
		}
			
		frames++;
		if (1000 / FPS > SDL_GetTicks() - t1)
			SDL_Delay(1000 / FPS - (SDL_GetTicks() - t1));

	};

	SDL_FreeSurface(life1);
	SDL_FreeSurface(life2);
	SDL_FreeSurface(life3);
	SDL_FreeSurface(background);
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};
