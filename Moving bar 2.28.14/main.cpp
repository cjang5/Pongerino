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
int ballPosX = (SCREEN_WIDTH / 2) - 25;
int ballPosY = (SCREEN_HEIGHT / 2) - 25;
bool ballMovingRight = false;
bool ballMovingUp = false;

//forward declarations
bool init();
bool loadMedia();
SDL_Texture* loadTexture(std::string path);
void close();
void updateScore(int x);

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
int isBarPresent(int x, int y) {
	
	//left bar collisions
	if (x == (leftBarRectPosX + 100) && ((y >= leftBarRectPosY && y < leftBarRectPosY + 300) ||
		(y + 50 > leftBarRectPosY && y + 50 <= leftBarRectPosY + 300))) {
		return 1;
	}
	else if (x < leftBarRectPosX + 100 && x >= leftBarRectPosX + 50 && y == leftBarRectPosY + 300) {
		return 2;
	}
	else if (x < leftBarRectPosX + 50 && y == leftBarRectPosY + 300) {
		return 3;
	}
	else if (x < leftBarRectPosX + 100 && x >= leftBarRectPosX + 50 && y + 50 == leftBarRectPosY) {
		return 4;
	}
	else if (x < leftBarRectPosX + 50 && y + 50 == leftBarRectPosY) {
		return 5;
	}
	
	//right bar collisions
	if (x + 50 == rightBarRectPosX && ((y >= rightBarRectPosY && y < rightBarRectPosY + 300) ||
		(y + 50 > rightBarRectPosY && y + 50 <= rightBarRectPosY + 300))) {
		return 6;
	}
	else if (x + 50 > rightBarRectPosX && x + 50 <= rightBarRectPosX + 50 && y == rightBarRectPosY + 300) {
		return 7;
	}
	else if (x + 50 > rightBarRectPosX && x + 50 <= rightBarRectPosX + 50 && y + 50 == rightBarRectPosY) {
		return 8;
	}
	else if (x + 50 > rightBarRectPosX && y + 50 == rightBarRectPosY) {
		return 9;
	}
	else if (x + 50 > rightBarRectPosY && y == rightBarRectPosY + 300) {
		return 10;
	}
	
	return 0;
}

bool isBetween(float pos, float lo, float hi) {
	if (pos > lo && pos < hi)
		return true;

	return false;
}

//reset function for after a point is scored
void reset() {
	ballPosX = (SCREEN_WIDTH / 2) - 100;
	ballPosY = (SCREEN_HEIGHT / 2) - 50;

	if (ballMovingRight)
		ballMovingRight = false;
	else
		ballMovingRight = true;

	if (ballMovingUp)
		ballMovingUp = false;
	else
		ballMovingUp = true;
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

bool running = true;
bool bDelay = false;

void pause() {
	if (running)
		running = false;
	else
		running = true;
}

void delay() {
	if (bDelay)
		bDelay = false;
	else
		bDelay = true;
}

void displayLeftBar() {
	printf(" LEFT BAR = (%i, %i)", leftBarRectPosX, leftBarRectPosY);
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
						case SDLK_SPACE:
							pause();
							break;
						case SDLK_d:
							delay();
							break;
						case SDLK_RETURN:
							displayLeftBar();
							break;
						case SDLK_r:
							reset();
							break;
						case SDLK_f:
							printf(" %i", isBarPresent(ballPosX, ballPosY));
							break;
						}
					}
				}
				if (running) {
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
						rightBarRectPosY -= 2;
					}
					else {
						rightBarRectPosY += 2;
					}


					//ball movement
					SDL_Rect ballRect;
					ballRect.x = ballPosX;
					ballRect.y = ballPosY;
					ballRect.w = 50;
					ballRect.h = 50;

					//set viewport to ballRect
					SDL_RenderSetViewport(globalRenderer, &ballRect);

					switch (isBarPresent(ballPosX, ballPosY)) {
					case 1:
						printf("1");
						ballMovingRight = true;
						break;
					case 2:
						printf("2");
						ballMovingRight = true;
						ballMovingUp = false;
						break;
					case 3:
						printf("3");
						ballMovingUp = false;
						break;
					case 4:
						printf("4");
						ballMovingRight = true;
						ballMovingUp = true;
						break;
					case 5:
						printf("5");
						ballMovingRight = false;
						ballMovingUp = true;
						break;
					case 6:
						printf("6");
						ballMovingRight = false;
						break;
					case 7:
						printf("7");
						ballMovingRight = false;
						ballMovingUp = false;
						break;
					case 8:
						ballMovingRight = true;
						ballMovingUp = false;
						break;
					case 9:
						ballMovingRight = true;
						ballMovingUp = true;
						break;
					case 10:
						ballMovingRight = true;
						ballMovingUp = false;
						break;
					}

					//wall collisions
					if (ballPosY < 0.0) {
						ballMovingUp = false;
					}
					else if ((ballPosY + 50) > SCREEN_HEIGHT) {
						ballMovingUp = true;
					}

					//determine how the ball will move
					if (ballMovingRight) {
						ballPosX += 1;
					}
					else {
						ballPosX -= 1;
					}

					if (ballMovingUp) {
						ballPosY -= 1;
					}
					else {
						ballPosY += 1;
					}

					//WIN CONDITIONS
					if (ballPosX < 0.0)
						updateScore(1); //give opponent a point
					else if (ballPosX + 50 > SCREEN_WIDTH)
						updateScore(0); //give yourself a point

					//draw the ball
					SDL_RenderCopy(globalRenderer, ball, NULL, NULL);

					//update the screen
					SDL_RenderPresent(globalRenderer);

					printf("\n(%i, %i)", ballPosX, ballPosY);

					if (bDelay) {
						SDL_Delay(200);
					}

					SDL_Delay(5);
				}
			}
		}
	}

	close();

	return 0;
}