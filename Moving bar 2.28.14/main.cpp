#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <stdio.h>

//window dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//sdl variables
SDL_Window* mainWindow = NULL;
SDL_Renderer* globalRenderer = NULL;

//assets
//the bars
SDL_Texture* leftBar = NULL;
SDL_Texture* rightBar = NULL;
float leftBarRectPosX = 0.0;
float leftBarRectPosY = 0.0;
float rightBarRectPosX = SCREEN_WIDTH - 100;
float rightBarRectPosY = 0.0;

bool barMovingUp = false; //determine whether the AI should be moving up or down
//ball
SDL_Texture* ball = NULL;
float ballPosX = (SCREEN_WIDTH / 2) - 25;
float ballPosY = (SCREEN_HEIGHT / 2) - 25;
bool ballMovingRight = false;
bool ballMovingUp = false;

//forward declarations
bool init();
bool loadMedia();
SDL_Texture* loadTexture(std::string path);
void close();

bool init() {
	//initialization flag
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL! SDL ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			printf("Warning: texture filtering is not in 'linear' mode!\n");

		//create window
		mainWindow = SDL_CreateWindow("Bar mover", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (mainWindow == NULL) {
			printf("Failed to create window! SDL ERROR: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//create renderer
			globalRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);

			if (globalRenderer == NULL) {
				printf("Failed to create renderer! SDL ERROR: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//initialize draw color mode for renderer
				SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//imgFlags
				int imgFlags = IMG_INIT_PNG;

				if (!IMG_Init(imgFlags) & imgFlags) {
					printf("Failed to initialize SDL_Image! IMG ERROR: %s\n:", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	//initialization flag
	bool success = true;

	//left bar image
	leftBar = loadTexture("assets/bar_left.png");

	//right bar image
	rightBar = loadTexture("assets/bar_right.png");

	//ball
	ball = loadTexture("assets/ball.png");

	if (leftBar == NULL) {
		printf("Failed to load 'bar_left' image!\n");
		success = false;
	}

	if (rightBar == NULL) {
		printf("Failed to load 'bar_right' image!\n");
		success = false;
	}

	if (ball == NULL) {
		printf("Failed to load 'ball' image!\n");
		success = false;
	}

	return success;
}

SDL_Texture* loadTexture(std::string path) {
	//final texture to be returned
	SDL_Texture* finalTexture = NULL;

	//loaded image surface
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		printf("Failed to load surface! IMG ERROR: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//create texture from the surface
		finalTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);

		if (finalTexture == NULL) {
			printf("Failed to create texture from surface! ERROR: %s\n", SDL_GetError());
		}

		//free the surface because we don't need it
		SDL_FreeSurface(loadedSurface);
	}

	return finalTexture;
}

void close() {
	//destroy textures
	SDL_DestroyTexture(leftBar);
	leftBar = NULL;
	SDL_DestroyTexture(rightBar);
	rightBar = NULL;
	SDL_DestroyTexture(ball);
	ball = NULL;

	//destroy renderer
	SDL_DestroyRenderer(globalRenderer);
	globalRenderer = NULL;

	//destroy window
	SDL_DestroyWindow(mainWindow);
	mainWindow = NULL;

	//quit
	IMG_Quit();
	SDL_Quit();
}

//method for finding out if a bar is present in a certain coordinate
bool isBarPresent(float x, float y) {
	if (x <= leftBarRectPosX + 100 && (y >= leftBarRectPosY && y <= leftBarRectPosY + 299)) {
		return true;
	}
	else if (x + 50 >= rightBarRectPosX && (y >= rightBarRectPosY && y <= rightBarRectPosY + 299)) {
		return true;
	}

	return false;
}

//reset function for after a point is scored
void reset() {
	ballPosX = (SCREEN_WIDTH / 2) - 25;
	ballPosY = (SCREEN_HEIGHT / 2) - 25;
}

//method to update the score (0 = you, 1 = AI, 2 = reset)
void updateScore(int player) {
	SDL_Rect screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	for (float i = 255.0; i >= 0.0; i -= 0.5) {
		SDL_RenderSetViewport(globalRenderer, &screenRect);
		SDL_SetRenderDrawColor(globalRenderer, 0, i, 0, 0);
		SDL_RenderFillRect(globalRenderer, &screenRect);
		SDL_RenderPresent(globalRenderer);
	}

	reset();
}

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			//main loop flag
			bool quit = false;

			//sdl event handler
			SDL_Event e;

			//main loop
			while (!quit) {
				//while there are events on the queue
				while (SDL_PollEvent(&e) != 0) {
					//if user clicks 'x' to quit
					if (e.type == SDL_QUIT)
						quit = true;

					//if a key is pressed
					if (e.type == SDL_KEYDOWN) {
						//events based on what key is pressed
						switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_DOWN:
							leftBarRectPosY += 10;
							break;
						case SDLK_UP:
							leftBarRectPosY -= 10;
							break;
						}
					}
				}
				//set teh color
				SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//clear screen
				SDL_RenderClear(globalRenderer);

				//render to dimensions of the left bar
				SDL_Rect leftBarRect;
				leftBarRect.x = leftBarRectPosX;
				leftBarRect.y = leftBarRectPosY;
				leftBarRect.w = 100;
				leftBarRect.h = 300;

				//dont let the bar go off the screen
				if (leftBarRect.y < 0)
					leftBarRectPosY = 0;
				if (leftBarRect.y > SCREEN_HEIGHT - 300)
					leftBarRectPosY = SCREEN_HEIGHT - 300;

				//set viewport to leftBarRect
				SDL_RenderSetViewport(globalRenderer, &leftBarRect);

				//draw the left bar image
				SDL_RenderCopy(globalRenderer, leftBar, NULL, NULL);

				//rect for the AI bar
				SDL_Rect rightBarRect;
				rightBarRect.x = rightBarRectPosX;
				rightBarRect.y = rightBarRectPosY;
				rightBarRect.w = 100;
				rightBarRect.h = 300;

				//set viewport to rightBarRect
				SDL_RenderSetViewport(globalRenderer, &rightBarRect);

				//dont let the AI bar go off the screen
				if (rightBarRect.y < 0) {
					rightBarRectPosY = 0;
					barMovingUp = false;
				}
				if (rightBarRect.y > SCREEN_HEIGHT - 300) {
					rightBarRectPosY = SCREEN_HEIGHT - 300;
					barMovingUp = true;
				}

				//draw the AI image
				SDL_RenderCopy(globalRenderer, rightBar, NULL, NULL);

				//increment the ai bar so it keeps moving
				if (barMovingUp) {
					rightBarRectPosY -= 0.05;
				}
				else {
					rightBarRectPosY += 0.05;
				}

				//ball movement
				SDL_Rect ballRect;
				ballRect.x = ballPosX;
				ballRect.y = ballPosY;
				ballRect.w = 50;
				ballRect.h = 50;

				//set viewport to ballRect
				SDL_RenderSetViewport(globalRenderer, &ballRect);

				//collision physics
				if (isBarPresent(ballPosX, ballPosY)) {
					//change directions (left/right)
					if (ballMovingRight)
						ballMovingRight = false;
					else
						ballMovingRight = true;
				}
				if (ballPosY < 0.0) {
					ballMovingUp = false;
				}
				else if ((ballPosY + 50) > SCREEN_HEIGHT) {
					ballMovingUp = true;
				}
				
				//WIN CONDITIONS
				if (ballPosX < 0.0)
					updateScore(1); //give opponent a point
				else if (ballPosX + 50 > SCREEN_WIDTH)
					updateScore(0); //give yourself a point

				//draw the ball
				SDL_RenderCopy(globalRenderer, ball, NULL, NULL);

				//determine how the ball will move
				if (ballMovingRight) {
					ballPosX += 0.05;
				}
				else {
					ballPosX -= 0.05;
				}

				if (ballMovingUp) {
					ballPosY -= 0.05;
				}
				else {
					ballPosY += 0.05;
				}

				//update the screen
				SDL_RenderPresent(globalRenderer);
			}
		}
	}

	close();

	return 0;
}